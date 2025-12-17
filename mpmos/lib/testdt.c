#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mpmos.h"  
#include <omp.h>

// Función para imprimir matrices
void print_matrix(const char *name, double *matrix, int rows, int cols) {
    printf("%s:\n", name);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%10.4f ", matrix[i + j * rows]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Número de argumentos erróneo.\n");
        return 1;
    }

    // Asignar operación
    int op = -1;
    if (strcmp(argv[1], "solvels") == 0) op = 0;
    if (strcmp(argv[1], "mnorm") == 0) op = 1;
    if (strcmp(argv[1], "mset") == 0) op = 2;
    if (strcmp(argv[1], "mmul") == 0) op = 3;
    if (strcmp(argv[1], "mcpy") == 0) op = 4;
    if (strcmp(argv[1], "mmulc") == 0) op = 5; // Nueva operación: mmulc
    if (strcmp(argv[1], "msub") == 0) op = 6;  // Nueva operación: msub

    if (op == -1) {
        fprintf(stderr, "Operación inválida.\n");
        return 2;
    }

    // Leer el tamaño de la matriz desde los argumentos
    int n = atoi(argv[2]);
    if (n <= 0) {
        fprintf(stderr, "El tamaño de la matriz debe ser un entero positivo.\n");
        return 1;
    }

    // Variables comunes
    int lda = n, ldb = n, ldc = n, nrhs = 1, info;
    int *ipiv = NULL;
    double *A = NULL, *B = NULL, *C = NULL, *WORK = NULL, *D = NULL;
    char norm, uplo, transa, transb;
    double alpha, beta, result, constant;

    // Inicializar matrices y parámetros específicos
    switch (op) {
        case 0: // solvels
            ipiv = malloc(n * sizeof(int));
            A = malloc(n * n * sizeof(double));
            B = malloc(n * sizeof(double));
            if (!A || !B || !ipiv) {
                fprintf(stderr, "Error al asignar memoria para solvels.\n");
                free(A); free(B); free(ipiv);
                return 1;
            }
            for (int i = 0; i < n * n; i++) A[i] = rand() % 10 + 1;
            for (int i = 0; i < n; i++) B[i] = rand() % 10 + 1;
            break;

        case 1: // mnorm
            A = malloc(n * n * sizeof(double));
            WORK = malloc(n * sizeof(double));
            if (!A || !WORK) {
                fprintf(stderr, "Error al asignar memoria para mnorm.\n");
                free(A); free(WORK);
                return 1;
            }
            for (int i = 0; i < n * n; i++) A[i] = rand() % 10 + 1;
            norm = 'F';
            break;

        case 2: // mset
            C = malloc(n * n * sizeof(double));
            if (!C) {
                fprintf(stderr, "Error al asignar memoria para mset.\n");
                free(C);
                return 1;
            }
            uplo = 'A';
            alpha = 2.0;
            beta = 5.0;
            break;

        case 3: // mmul
            A = malloc(n * n * sizeof(double));
            B = malloc(n * n * sizeof(double));
            C = malloc(n * n * sizeof(double));
            if (!A || !B || !C) {
                fprintf(stderr, "Error al asignar memoria para mmul.\n");
                free(A); free(B); free(C);
                return 1;
            }
            for (int i = 0; i < n * n; i++) {
                A[i] = rand() % 10 + 1;
                B[i] = rand() % 10 + 1;
                C[i] = 0.0;
            }
            transa = 'N';
            transb = 'N';
            alpha = 1.0;
            beta = 0.0;
            break;

        case 4: // mcpy
            A = malloc(n * n * sizeof(double));
            C = malloc(n * n * sizeof(double));
            if (!A || !C) {
                fprintf(stderr, "Error al asignar memoria para mcpy.\n");
                free(A); free(C);
                return 1;
            }
            for (int i = 0; i < n * n; i++) A[i] = rand() % 10 + 1;
            uplo = 'A';
            break;

        case 5: // mmulc
            A = malloc(n * n * sizeof(double));
            D = malloc(n * n * sizeof(double));
            if (!A || !D) {
                fprintf(stderr, "Error al asignar memoria para mmulc.\n");
                free(A); free(D);
                return 1;
            }
            for (int i = 0; i < n * n; i++) A[i] = rand() % 10 + 1;
            constant = 2.0; // Ejemplo: multiplicar por 2
            break;

        case 6: // msub
            A = malloc(n * n * sizeof(double));
            B = malloc(n * n * sizeof(double));
            D = malloc(n * n * sizeof(double));
            if (!A || !B || !D) {
                fprintf(stderr, "Error al asignar memoria para msub.\n");
                free(A); free(B); free(D);
                return 1;
            }
            for (int i = 0; i < n * n; i++) {
                A[i] = rand() % 10 + 1;
                B[i] = rand() % 10 + 1;
            }
            break;
    }

    // Medir el tiempo de ejecución de la operación seleccionada
    int nthreads=omp_get_max_threads()*2;
    omp_set_num_threads(nthreads);
    double start = omp_get_wtime();

    switch (op) {
        case 0: solvels(&n, &nrhs, A, &lda, ipiv, B, &ldb, &info); break;
        case 1: result = mnorm(&norm, &n, &n, A, &lda, WORK); break;
        case 2: mset(&uplo, &n, &n, &alpha, &beta, C, &lda); break;
        case 3: mmul(&transa, &transb, &n, &n, &n, &alpha, A, &lda, B, &ldb, &beta, C, &ldc); break;
        case 4: mcpy(&uplo, &n, &n, A, &lda, C, &lda); break;
        case 5: mmulc(D, n, A, constant); break;
        case 6: msub(D, n, A, B); break;
    }

    double end = omp_get_wtime();
    double elapsed_time = end - start;

    // Imprimir el tiempo de ejecución
    fprintf(stdout, "%.6f\n", elapsed_time);

    // Liberar memoria
    free(A); free(B); free(C); free(WORK); free(ipiv); free(D);

    return 0;
}
