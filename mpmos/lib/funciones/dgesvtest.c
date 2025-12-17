#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>  // Para memcpy
#include "ompblas.h"   // Asegúrate de incluir tu biblioteca que contiene dgesv

#define TOLERANCE 1e-10  // Tolerancia para comparar resultados

// Función auxiliar para imprimir matrices
void print_matrix(const char *name, double *matrix, int rows, int cols) {
    printf("%s:\n", name);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%10.4f ", matrix[i + j * rows]);
        }
        printf("\n");
    }
}

// Función auxiliar para verificar la solución de un sistema A * X = B
int check_solution(double *A_orig, double *X, double *B, int N, int NRHS) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < NRHS; j++) {
            double sum = 0.0;
            for (int k = 0; k < N; k++) {
                sum += A_orig[i + k * N] * X[k + j * N];
            }
            if (fabs(sum - B[i + j * N]) > TOLERANCE) {
                printf("Error: Solution does not satisfy A*X=B at row %d, column %d\n", i, j);
                return 0;
            }
        }
    }
    return 1;
}

// Prueba de un sistema simple 2x2
void test_simple_system() {
    int N = 2;
    int NRHS = 1;
    int LDA = N, LDB = N;
    int INFO;
    int IPIV[2];

    double A[4] = {4, 3, 6, 3};  // Matriz 2x2
    double A_orig[4];
    memcpy(A_orig, A, sizeof(A));  // Copia de la matriz original para la verificación

    double B[2] = {10, 12};      // Vector del lado derecho
    double B_orig[2];
    memcpy(B_orig, B, sizeof(B));  // Copia de B original para la verificación

    printf("Test Simple System 2x2:\n");
    print_matrix("A", A, N, N);
    print_matrix("B", B, N, 1);

    dgesv(&N, &NRHS, A, &LDA, IPIV, B, &LDB, &INFO);

    if (INFO != 0) {
        printf("dgesv failed with INFO = %d\n", INFO);
    } else {
        print_matrix("X (solution)", B, N, 1);
        if (check_solution(A_orig, B, B_orig, N, NRHS)) {
            printf("Test Simple System 2x2 passed.\n");
        } else {
            printf("Test Simple System 2x2 failed.\n");
        }
    }
    printf("\n");
}

// Prueba de un sistema 3x3
void test_larger_system() {
    int N = 3;
    int NRHS = 1;
    int LDA = N, LDB = N;
    int INFO;
    int IPIV[3];

    double A[9] = {3, 2, -1, 2, -2, 4, -1, 0.5, -1};  // Matriz 3x3
    double A_orig[9];
    memcpy(A_orig, A, sizeof(A));  // Copia de la matriz original para la verificación

    double B[3] = {1, -2, 0};       // Vector del lado derecho
    double B_orig[3];
    memcpy(B_orig, B, sizeof(B));   // Copia de B original para la verificación

    printf("Test Larger System 3x3:\n");
    print_matrix("A", A, N, N);
    print_matrix("B", B, N, 1);

    dgesv(&N, &NRHS, A, &LDA, IPIV, B, &LDB, &INFO);

    if (INFO != 0) {
        printf("dgesv failed with INFO = %d\n", INFO);
    } else {
        print_matrix("X (solution)", B, N, 1);
        if (check_solution(A_orig, B, B_orig, N, NRHS)) {
            printf("Test Larger System 3x3 passed.\n");
        } else {
            printf("Test Larger System 3x3 failed.\n");
        }
    }
    printf("\n");
}

// Prueba de un sistema singular (debe fallar)
void test_singular_system() {
    int N = 2;
    int NRHS = 1;
    int LDA = N, LDB = N;
    int INFO;
    int IPIV[2];

    double A[4] = {1, 2, 2, 4};  // Matriz singular 2x2
    double B[2] = {3, 6};        // Vector del lado derecho

    printf("Test Singular System 2x2:\n");
    print_matrix("A", A, N, N);
    print_matrix("B", B, N, 1);

    dgesv(&N, &NRHS, A, &LDA, IPIV, B, &LDB, &INFO);

    if (INFO != 0) {
        printf("dgesv correctly identified a singular matrix with INFO = %d\n", INFO);
    } else {
        printf("Test Singular System 2x2 failed: dgesv did not identify singularity.\n");
    }
    printf("\n");
}

int main() {
    test_simple_system();
    test_larger_system();
    test_singular_system();
    return 0;
}