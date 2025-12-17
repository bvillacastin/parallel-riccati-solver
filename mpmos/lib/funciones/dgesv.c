#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void dgetf2(int *N, double *A, int *LDA, int *IPIV, int *INFO);
void dtrsm(int *N, int *NRHS, double *A, int *LDA, double *B, int *LDB, int *IPIV);


void dgetf2(int *N, double *A, int *LDA, int *IPIV, int *INFO) {
    *INFO = 0;
    for (int i = 0; i < *N; i++) {
        // Buscar el pivote máximo en la columna actual
        int max_index = i;
        double max_value = fabs(A[i + i * (*LDA)]);
        for (int k = i + 1; k < *N; k++) {
            double abs_val = fabs(A[k + i * (*LDA)]);
            if (abs_val > max_value) {
                max_value = abs_val;
                max_index = k;
            }
        }

        // Verificar si el pivote es cero (matriz singular)
        if (max_value == 0.0) {
            *INFO = i + 1;
            return;
        }

        // Guardar el índice de pivote
        IPIV[i] = max_index;

        // Intercambiar filas si es necesario
        if (max_index != i) {
            for (int j = 0; j < *N; j++) {
                double temp = A[i + j * (*LDA)];
                A[i + j * (*LDA)] = A[max_index + j * (*LDA)];
                A[max_index + j * (*LDA)] = temp;
            }
        }

        // Factorización LU
        for (int j = i + 1; j < *N; j++) {
            A[j + i * (*LDA)] /= A[i + i * (*LDA)];
            for (int k = i + 1; k < *N; k++) {
                A[j + k * (*LDA)] -= A[j + i * (*LDA)] * A[i + k * (*LDA)];
            }
        }
    }
}

// Resolución del sistema triangular
void dtrsm(int *N, int *NRHS, double *A, int *LDA, double *B, int *LDB, int *IPIV) {
    // Aplicar las permutaciones a B
    for (int i = 0; i < *N; i++) {
        if (IPIV[i] != i) {
            for (int j = 0; j < *NRHS; j++) {
                double temp = B[i + j * (*LDB)];
                B[i + j * (*LDB)] = B[IPIV[i] + j * (*LDB)];
                B[IPIV[i] + j * (*LDB)] = temp;
            }
        }
    }

    // Sustitución hacia adelante para resolver L * Y = B
    for (int j = 0; j < *NRHS; j++) {
        for (int i = 0; i < *N; i++) {
            for (int k = 0; k < i; k++) {
                B[i + j * (*LDB)] -= A[i + k * (*LDA)] * B[k + j * (*LDB)];
            }
        }
    }

    // Sustitución hacia atrás para resolver U * X = Y
    for (int j = 0; j < *NRHS; j++) {
        for (int i = *N - 1; i >= 0; i--) {
            for (int k = i + 1; k < *N; k++) {
                B[i + j * (*LDB)] -= A[i + k * (*LDA)] * B[k + j * (*LDB)];
            }
            B[i + j * (*LDB)] /= A[i + i * (*LDA)];
        }
    }
}

// Función principal dgesv que utiliza dgetf2 y dtrsm
void dgesv(int *N, int *NRHS, double *A, int *LDA, int *IPIV, double *B, int *LDB, int *INFO) {
    // Factorización LU con pivoteo parcial
    dgetf2(N, A, LDA, IPIV, INFO);

    // Si hubo un error en la factorización, salir
    if (*INFO != 0) {
        return;
    }

    // Resolver el sistema triangular usando la factorización LU
    dtrsm(N, NRHS, A, LDA, B, LDB, IPIV);
}