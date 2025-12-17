#include <stdio.h>
#include <string.h>
#include "ompblas.h"   // Asegúrate de incluir tu biblioteca que contiene dlaset

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

// Función auxiliar para verificar que una matriz es igual a la esperada
int matrices_are_equal(double *A, double *expected, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (A[i + j * rows] != expected[i + j * rows]) {
                return 0;
            }
        }
    }
    return 1;
}

// Test 1: Inicialización completa de la matriz
void test_full_initialization() {
    int M = 3, N = 3, LDA = M;
    double ALPHA = 2.0, BETA = 5.0;
    double A[9] = {0};  // Matriz a inicializar

    printf("Test Full Initialization (UPLO='A'):\n");

    char UPLO = 'A';
    dlaset(&UPLO, &M, &N, &ALPHA, &BETA, A, &LDA);

    print_matrix("A (result)", A, M, N);

    // Matriz esperada después de la inicialización
    double expected_A[9] = {
        5.0, 2.0, 2.0,
        2.0, 5.0, 2.0,
        2.0, 2.0, 5.0
    };

    if (matrices_are_equal(A, expected_A, M, N)) {
        printf("Test Full Initialization passed.\n");
    } else {
        printf("Test Full Initialization failed.\n");
    }
    printf("\n");
}

// Test 2: Inicialización de la parte triangular superior
void test_upper_triangular_initialization() {
    int M = 3, N = 3, LDA = M;
    double ALPHA = 3.0, BETA = 7.0;
    double A[9] = {0};  // Matriz a inicializar

    printf("Test Upper Triangular Initialization (UPLO='U'):\n");

    char UPLO = 'U';
    dlaset(&UPLO, &M, &N, &ALPHA, &BETA, A, &LDA);

    print_matrix("A (result)", A, M, N);

    // Matriz esperada después de la inicialización
    double expected_A[9] = {
        7.0, 0.0, 0.0,
        3.0, 7.0, 0.0,
        3.0, 3.0, 7.0
    };

    if (matrices_are_equal(A, expected_A, M, N)) {
        printf("Test Upper Triangular Initialization passed.\n");
    } else {
        printf("Test Upper Triangular Initialization failed.\n");
    }
    printf("\n");
}

// Test 3: Inicialización de la parte triangular inferior
void test_lower_triangular_initialization() {
    int M = 3, N = 3, LDA = M;
    double ALPHA = 4.0, BETA = 6.0;
    double A[9] = {0};  // Matriz a inicializar

    printf("Test Lower Triangular Initialization (UPLO='L'):\n");

    char UPLO = 'L';
    dlaset(&UPLO, &M, &N, &ALPHA, &BETA, A, &LDA);

    print_matrix("A (result)", A, M, N);

    // Matriz esperada después de la inicialización
    double expected_A[9] = {
        6.0, 4.0, 4.0,
        0.0, 6.0, 4.0,
        0.0, 0.0, 6.0
    };

    if (matrices_are_equal(A, expected_A, M, N)) {
        printf("Test Lower Triangular Initialization passed.\n");
    } else {
        printf("Test Lower Triangular Initialization failed.\n");
    }
    printf("\n");
}

int main() {
    test_full_initialization();
    test_upper_triangular_initialization();
    test_lower_triangular_initialization();
    return 0;
}
