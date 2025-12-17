#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "blas.h"  
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
    if (strcmp(argv[1], "dgesv") == 0) op = 0;
    if (strcmp(argv[1], "dlange") == 0) op = 1;
    if (strcmp(argv[1], "dlaset") == 0) op = 2;
    if (strcmp(argv[1], "dgemm") == 0) op = 3;
    if (strcmp(argv[1], "dlacpy") == 0) op = 4;

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
    double *A = NULL, *B = NULL, *C = NULL, *WORK = NULL;
    char norm, uplo, transa, transb;
    double alpha, beta, result;

    // Inicializar matrices y parámetros específicos
    switch (op) {
        case 0: // dgesv
            ipiv = malloc(n * sizeof(int));
            A = malloc(n * n * sizeof(double));
            B = malloc(n * sizeof(double));
            if (!A || !B || !ipiv) {
                fprintf(stderr, "Error al asignar memoria para dgesv.\n");
                free(A); free(B); free(ipiv);
                return 1;
            }
            // Inicializar A y B
            for (int i = 0; i < n * n; i++) A[i] = rand() % 10 + 1;
            for (int i = 0; i < n; i++) B[i] = rand() % 10 + 1;
            break;

        case 1: // dlange
            A = malloc(n * n * sizeof(double));
            WORK = malloc(n * sizeof(double));
            if (!A || !WORK) {
                fprintf(stderr, "Error al asignar memoria para dlange.\n");
                free(A); free(WORK);
                return 1;
            }
            // Inicializar A
            for (int i = 0; i < n * n; i++) A[i] = rand() % 10 + 1;
            norm = 'F'; // Norma Frobenius
            break;

        case 2: // dlaset
            C = malloc(n * n * sizeof(double));
            if (!C) {
                fprintf(stderr, "Error al asignar memoria para dlaset.\n");
                free(C);
                return 1;
            }
            // Inicializar parámetros
            uplo = 'A'; // Inicializar toda la matriz
            alpha = 2.0;
            beta = 5.0;
            break;

        case 3: // dgemm
            A = malloc(n * n * sizeof(double));
            B = malloc(n * n * sizeof(double));
            C = malloc(n * n * sizeof(double));
            if (!A || !B || !C) {
                fprintf(stderr, "Error al asignar memoria para dgemm.\n");
                free(A); free(B); free(C);
                return 1;
            }
            // Inicializar A, B, y C
            for (int i = 0; i < n * n; i++) {
                A[i] = rand() % 10 + 1;
                B[i] = rand() % 10 + 1;
                C[i] = 0.0;
            }
            // Parámetros para dgemm
            transa = 'N';
            transb = 'N';
            alpha = 1.0;
            beta = 0.0;
            break;

        case 4: // dlacpy
            A = malloc(n * n * sizeof(double));
            C = malloc(n * n * sizeof(double));
            if (!A || !C) {
                fprintf(stderr, "Error al asignar memoria para dlacpy.\n");
                free(A); free(C);
                return 1;
            }
            // Inicializar A
            for (int i = 0; i < n * n; i++) {
                A[i] = rand() % 10 + 1;
            }
            uplo = 'A'; // Copiar toda la matriz
            break;
    }

    // Medir el tiempo de ejecución de la operación seleccionada
    double start = omp_get_wtime();

    switch (op) {
        case 0: // dgesv
            dgesv_(&n, &nrhs, A, &lda, ipiv, B, &ldb, &info);
            //if (info == 0) printf("dgesv ejecutado correctamente.\n");
            //else if (info < 0) fprintf(stderr, "Error: El argumento %d es inválido.\n", -info);
            //else fprintf(stderr, "Error: La matriz es singular en la fila %d.\n", info);
            break;

        case 1: // dlange
            result = dlange_(&norm, &n, &n, A, &lda, WORK);
            //printf("Resultado de dlange: %.6f\n", result);
            break;

        case 2: // dlaset
            dlaset_(&uplo, &n, &n, &alpha, &beta, C, &lda);
            break;

        case 3: // dgemm
            dgemm_(&transa, &transb, &n, &n, &n, &alpha, A, &lda, B, &ldb, &beta, C, &ldc);
            //print_matrix("C (result)", C, n, n);
            break;

        case 4: // dlacpy
            dlacpy_(&uplo, &n, &n, A, &lda, C, &lda);
            break;
    }

    double end = omp_get_wtime();
    double elapsed_time = end - start;

    // Imprimir el tiempo de ejecución
    //printf("Tiempo de ejecución de %s: %.6f segundos.\n", argv[1], elapsed_time);
    fprintf(stdout, "%.6f\n",elapsed_time);


    // Liberar memoria
    free(A); free(B); free(C); free(WORK); free(ipiv);

    return 0;
}
