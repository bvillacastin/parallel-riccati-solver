#include <stdio.h>
#include <string.h>
#include "ompblas.h"   // Asegúrate de incluir tu biblioteca que contiene dgemm
#include <math.h>

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

// Función auxiliar para verificar si dos matrices son iguales
int matrices_are_equal(double *A, double *B, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (A[i + j * rows] != B[i + j * rows]) {
                return 0;
            }
        }
    }
    return 1;
}

// Test: C = A * B sin transposición
void test_simple_multiplication() {
    int M = 2, N = 2, K = 2;
    int LDA = M, LDB = K, LDC = M;
    double ALPHA = 1.0, BETA = 0.0;

    double A[4] = {1, 2, 3, 4};  // 2x2 matriz A
    double B[4] = {5, 6, 7, 8};  // 2x2 matriz B
    double C[4] = {0};           // 2x2 matriz resultado

    char TRANSA = 'N', TRANSB = 'N';

    printf("Test Simple Multiplication:\n");
    print_matrix("A", A, M, K);
    print_matrix("B", B, K, N);

    dgemm(&TRANSA, &TRANSB, &M, &N, &K, &ALPHA, A, &LDA, B, &LDB, &BETA, C, &LDC);

    print_matrix("C (result)", C, M, N);
    

    // Matriz esperada: A * B
    double expected_C[4] = {23, 34, 31, 46};
    print_matrix("Expected C", expected_C, M, N);

    if (matrices_are_equal(C, expected_C, M, N)) {
        printf("Test Simple Multiplication passed.\n");
    } else {
        printf("Test Simple Multiplication failed.\n");
    }
    printf("\n");
}

// Test: C = A^T * B
void test_transpose_A() {
    int M = 2, N = 2, K = 2;
    int LDA = K, LDB = K, LDC = M;
    double ALPHA = 1.0, BETA = 0.0;

    double A[4] = {1, 3, 2, 4};  // 2x2 matriz A en formato columna mayor
    double B[4] = {5, 6, 7, 8};  // 2x2 matriz B
    double C[4] = {0};           // 2x2 matriz resultado

    char TRANSA = 'T', TRANSB = 'N';

    printf("Test Transpose A:\n");
    print_matrix("A", A, K, M);
    print_matrix("B", B, K, N);

    dgemm(&TRANSA, &TRANSB, &M, &N, &K, &ALPHA, A, &LDA, B, &LDB, &BETA, C, &LDC);

    print_matrix("C (result)", C, M, N);

    // Matriz esperada: A^T * B
    double expected_C[4] = {23, 34, 31, 46};

    if (matrices_are_equal(C, expected_C, M, N)) {
        printf("Test Transpose A passed.\n");
    } else {
        printf("Test Transpose A failed.\n");
    }
    printf("\n");
}

// Test: C = A * B^T
void test_transpose_B() {
    int M = 2, N = 2, K = 2;
    int LDA = M, LDB = N, LDC = M;
    double ALPHA = 1.0, BETA = 0.0;

    double A[4] = {1, 2, 3, 4};  // 2x2 matriz A
    double B[4] = {5, 7, 6, 8};  // 2x2 matriz B transpuesta
    double C[4] = {0};           // 2x2 matriz resultado

    char TRANSA = 'N', TRANSB = 'T';

    printf("Test Transpose B:\n");
    print_matrix("A", A, M, K);
    print_matrix("B", B, N, K);

    dgemm(&TRANSA, &TRANSB, &M, &N, &K, &ALPHA, A, &LDA, B, &LDB, &BETA, C, &LDC);

    print_matrix("C (result)", C, M, N);

    // Matriz esperada: A * B^T
    double expected_C[4] = {23, 34, 31, 46};
    print_matrix("Expected C", expected_C, M, N);

    if (matrices_are_equal(C, expected_C, M, N)) {
        printf("Test Transpose B passed.\n");
    } else {
        printf("Test Transpose B failed.\n");
    }
    printf("\n");
}

// Test: C = 0 * A * B + 1 * C
void test_beta_C() {
    int M = 2, N = 2, K = 2;
    int LDA = M, LDB = K, LDC = M;
    double ALPHA = 0.0, BETA = 1.0;

    double A[4] = {1, 2, 3, 4};  // 2x2 matriz A
    double B[4] = {5, 6, 7, 8};  // 2x2 matriz B
    double C[4] = {1, 2, 3, 4};  // 2x2 matriz resultado inicial

    char TRANSA = 'N', TRANSB = 'N';

    printf("Test Beta * C:\n");
    print_matrix("A", A, M, K);
    print_matrix("B", B, K, N);
    print_matrix("C (initial)", C, M, N);

    dgemm(&TRANSA, &TRANSB, &M, &N, &K, &ALPHA, A, &LDA, B, &LDB, &BETA, C, &LDC);

    print_matrix("C (result)", C, M, N);

    // Matriz esperada: C = C * 1
    double expected_C[4] = {1, 2, 3, 4};

    if (matrices_are_equal(C, expected_C, M, N)) {
        printf("Test Beta * C passed.\n");
    } else {
        printf("Test Beta * C failed.\n");
    }
    printf("\n");
}

int main() {
    test_simple_multiplication();
    test_transpose_A();
    test_transpose_B();
    test_beta_C();
    return 0;
}
