#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>  // Para DBL_MAX
#include "mpmos.h"
#include <omp.h>

//-----------------------------------------------mpsolvels--------------------------------------------------
void mlufact(int *N, double *A, int *LDA, int *IPIV, int *INFO);
void tsolve(int *N, int *NRHS, double *A, int *LDA, double *B, int *LDB, int *IPIV);


void mlufact(int *N, double *A, int *LDA, int *IPIV, int *INFO) {
    *INFO = 0;
    int n=*N; int lda=*LDA;
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < n; i++) {
        // Buscar el pivote máximo en la columna actual
        int max_index = i;
        double max_value = fabs(A[i + i * lda]);
        for (int k = i + 1; k < n; k++) {
            double abs_val = fabs(A[k + i * lda]);
            if (abs_val > max_value) {
                max_value = abs_val;
                max_index = k;
            }
        }

        // Verificar si el pivote es cero (matriz singular)
        if (max_value == 0.0) {
            *INFO = i + 1;
            //return;
        }

        // Guardar el índice de pivote
        IPIV[i] = max_index;

        // Intercambiar filas si es necesario
        if (max_index != i) {
            for (int j = 0; j < n; j++) {
                double temp = A[i + j * lda];
                A[i + j * lda] = A[max_index + j * lda];
                A[max_index + j * lda] = temp;
            }
        }

        // Factorización LU
        for (int j = i + 1; j < n; j++) {
            A[j + i * lda] /= A[i + i * lda];
            for (int k = i + 1; k < n; k++) {
                A[j + k * lda] -= A[j + i * lda] * A[i + k * lda];
            }
        }
    }
}

// Resolución del sistema triangular
void tsolve(int *N, int *NRHS, double *A, int *LDA, double *B, int *LDB, int *IPIV) {
    int n=*N; int lda=*LDA; int nrhs=*NRHS; int ldb=*LDB;
    // Aplicar las permutaciones a B
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        if (IPIV[i] != i) {
            for (int j = 0; j < nrhs; j++) {
                double temp = B[i + j * ldb];
                B[i + j * ldb] = B[IPIV[i] + j * ldb];
                B[IPIV[i] + j * ldb] = temp;
            }
        }
    }

    // Sustitución hacia adelante para resolver L * Y = B
    #pragma omp parallel for
    for (int j = 0; j < nrhs; j++) {
        for (int i = 0; i < n; i++) {
            for (int k = 0; k < i; k++) {
                B[i + j * ldb] -= A[i + k * lda] * B[k + j * ldb];
            }
        }
    }

    // Sustitución hacia atrás para resolver U * X = Y
    #pragma omp parallel for collapse(2)
    for (int j = 0; j < nrhs; j++) {
        for (int i = n - 1; i >= 0; i--) {
            for (int k = i + 1; k < n; k++) {
                B[i + j * ldb] -= A[i + k * lda] * B[k + j * ldb];
            }
            B[i + j * ldb] /= A[i + i * lda];
        }
    }
}

// Función principal mpsolvels que utiliza mpmlufact y mptsolve
void solvels(int *N, int *NRHS, double *A, int *LDA, int *IPIV, double *B, int *LDB, int *INFO) {
    // Factorización LU con pivoteo parcial
    mlufact(N, A, LDA, IPIV, INFO);

    // Si hubo un error en la factorización, salir
    if (*INFO != 0) {
        return;
    }

    // Resolver el sistema triangular usando la factorización LU
    tsolve(N, NRHS, A, LDA, B, LDB, IPIV);
}
//------------------------------------------------------------------------------------------------------
//-----------------------------------------------mpmmul--------------------------------------------------
void mmul(char *TRANSA, char *TRANSB, int *M, int *N, int *K,
           double *ALPHA, double *A, int *LDA, double *B, int *LDB,
           double *BETA, double *C, int *LDC) {
    
    int trans_a = (*TRANSA == 'T' || *TRANSA == 't');
    int trans_b = (*TRANSB == 'T' || *TRANSB == 't');

    // Escalar matriz C por BETA
    for (int j = 0; j < *N; j++) {
        for (int i = 0; i < *M; i++) {
            C[i + j * (*LDC)] *= *BETA;
        }
    }

    // Bloques para mejorar la localidad de memoria
    const int BLOCK_SIZE = 64; // Ajustar según la arquitectura
    #pragma omp parallel for collapse(3)
    for (int jj = 0; jj < *N; jj += BLOCK_SIZE) {
        for (int ii = 0; ii < *M; ii += BLOCK_SIZE) {
            for (int ll = 0; ll < *K; ll += BLOCK_SIZE) {
                for (int j = jj; j < jj + BLOCK_SIZE && j < *N; j++) {
                    for (int i = ii; i < ii + BLOCK_SIZE && i < *M; i++) {
                        double sum = 0.0;
                        for (int l = ll; l < ll + BLOCK_SIZE && l < *K; l++) {
                            double a_val = trans_a ? A[l + i * (*LDA)] : A[i + l * (*LDA)];
                            double b_val = trans_b ? B[j + l * (*LDB)] : B[l + j * (*LDB)];
                            sum += a_val * b_val;
                        }
                        C[i + j * (*LDC)] += *ALPHA * sum;
                    }
                }
            }
        }
    }
}

//------------------------------------------------------------------------------------------------------
//-----------------------------------------------mpmcpy-------------------------------------------------
void mcpy(char *UPLO, int *M, int *N, double *A, int *LDA, double *B, int *LDB) {
    int lda=*LDA;
    int ldb=*LDB;
    if (*UPLO == 'A' || *UPLO == 'a') {  // Copiar toda la matriz
        #pragma omp parallel for collapse(2)
        for (int j = 0; j < *N; j++) {
            for (int i = 0; i < *M; i++) {
                B[i + j * ldb] = A[i + j * lda];
            }
        }
    } else if (*UPLO == 'U' || *UPLO == 'u') {  // Copiar solo la parte triangular superior
        #pragma omp parallel for
        for (int j = 0; j < *N; j++) {
            for (int i = 0; i <= j && i < *M; i++) {  // Solo copiar si i <= j
                B[i + j * ldb] = A[i + j * lda];
            }
        }
    } else if (*UPLO == 'L' || *UPLO == 'l') {  // Copiar solo la parte triangular inferior
        #pragma omp parallel for
        for (int j = 0; j < *N; j++) {
            for (int i = j; i < *M; i++) {  // Solo copiar si i >= j
                B[i + j * ldb] = A[i + j * lda];
            }
        }
    }
}


//------------------------------------------------------------------------------------------------------
//-----------------------------------------------mpmset-------------------------------------------------
void mset(char *UPLO, int *M, int *N, double *ALPHA, double *BETA, double *A, int *LDA) {
    if (*UPLO == 'A' || *UPLO == 'a') {  // Inicializar toda la matriz
        #pragma omp parallel for collapse(2)
        for (int j = 0; j < *N; j++) {
            for (int i = 0; i < *M; i++) {
                if (i == j) {
                    A[i + j * (*LDA)] = *BETA;  // Diagonal
                } else {
                    A[i + j * (*LDA)] = *ALPHA;  // Fuera de la diagonal
                }
            }
        }
    } else if (*UPLO == 'U' || *UPLO == 'u') {  // Inicializar solo la parte triangular superior
        #pragma omp parallel for
        for (int j = 0; j < *N; j++) {
            for (int i = 0; i <= j && i < *M; i++) {
                if (i == j) {
                    A[i + j * (*LDA)] = *BETA;  // Diagonal
                } else {
                    A[i + j * (*LDA)] = *ALPHA;  // Fuera de la diagonal
                }
            }
        }
    } else if (*UPLO == 'L' || *UPLO == 'l') {  // Inicializar solo la parte triangular inferior
        #pragma omp parallel for
        for (int j = 0; j < *N; j++) {
            for (int i = j; i < *M; i++) {
                if (i == j) {
                    A[i + j * (*LDA)] = *BETA;  // Diagonal
                } else {
                    A[i + j * (*LDA)] = *ALPHA;  // Fuera de la diagonal
                }
            }
        }
    }
}


//------------------------------------------------------------------------------------------------------
//-----------------------------------------------mpmnorm-------------------------------------------------
double mnorm(char *NORM, int *M, int *N, double *A, int *LDA, double *WORK) {
    double norm = 0.0;

    // Norma Máximo Absoluto ('M')
    if (*NORM == 'M' || *NORM == 'm') {
        #pragma omp parallel for collapse(2)
	for (int j = 0; j < *N; j++) {
            for (int i = 0; i < *M; i++) {
                double abs_val = fabs(A[i + j * (*LDA)]);
                if (abs_val > norm) {
                    norm = abs_val;
                }
            }
        }
    }
    // Norma Uno ('1')
    else if (*NORM == '1' || *NORM == 'O' || *NORM == 'o') {
        #pragma omp parallel for 
	for (int j = 0; j < *N; j++) {
            double col_sum = 0.0;
            for (int i = 0; i < *M; i++) {
                col_sum += fabs(A[i + j * (*LDA)]);
            }
            if (col_sum > norm) {
                norm = col_sum;
            }
        }
    }
    // Norma Infinito ('I')
    else if (*NORM == 'I' || *NORM == 'i') {
        for (int i = 0; i < *M; i++) {
            WORK[i] = 0.0; // Inicializar el espacio de trabajo
        }
        #pragma omp parallel for collapse(2)
	for (int j = 0; j < *N; j++) {
            for (int i = 0; i < *M; i++) {
                WORK[i] += fabs(A[i + j * (*LDA)]);
            }
        }
        // Encontrar el valor máximo en WORK (la suma máxima de filas)
        for (int i = 0; i < *M; i++) {
            if (WORK[i] > norm) {
                norm = WORK[i];
            }
        }
    }
    // Norma Frobenius ('F' o 'E')
    else if (*NORM == 'F' || *NORM == 'f' || *NORM == 'E' || *NORM == 'e') {
        double sum = 0.0; // Para acumular la suma de cuadrados
        #pragma omp parallel for collapse(2) reduction(+:sum)
	for (int j = 0; j < *N; j++) {
            for (int i = 0; i < *M; i++) {
                double val = A[i + j * (*LDA)];
                sum += val * val; // Acumula el cuadrado de cada elemento
            }
        }
        norm = sqrt(sum); // Calcula la raíz cuadrada de la suma
    }
    
    return norm;
}
//------------------------------------------------------------------------------------------------------
//-----------------------------------------------MMULC--------------------------------------------------
void mmulc(double *D, int t, double *A, double c){
    #pragma omp parallel for collapse(2)
    for (int i=0; i<t; i++){
        for (int j=0; j<t; j++){
            D[(i*t)+j]=c*A[(i*t)+j];
        }
    }
}
//------------------------------------------------------------------------------------------------------
//-----------------------------------------------MSUB---------------------------------------------------
void msub(double *D, int t, double *A, double *B){
    #pragma omp parallel for
    for (int i=0; i<t; i++){
        for (int j=0; j<t; j++){
            D[(i*t)+j]=A[(i*t)+j]-B[(i*t)+j];
        }
    }
}
