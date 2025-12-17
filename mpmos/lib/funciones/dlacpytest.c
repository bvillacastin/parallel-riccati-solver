#include <stdio.h>
#include <string.h>
#include "ompblas.h"   // AsegÃºrate de incluir tu biblioteca que contiene dlacpy
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

// Función auxiliar para verificar si dos matrices son iguales dentro de una tolerancia
int matrices_are_equal(double *A, double *B, int rows, int cols, double tolerance) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (fabs(A[i + j * rows] - B[i + j * rows]) > tolerance) {
                return 0;  // Las matrices no son iguales
            }
        }
    }
    return 1;  // Las matrices son iguales
}

// Prueba de copia completa
void test_copy_full() {
    int M = 3, N = 3;
    int LDA = M, LDB = M;
    double A[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};  // Matriz 3x3 en formato columna mayor
    double B[9] = {0};                          // Matriz destino inicializada a 0

    printf("Test Copy Full Matrix:\n");
    print_matrix("A (original)", A, M, N);

    char uplo = 'A';
    dlacpy(&uplo, &M, &N, A, &LDA, B, &LDB);

    print_matrix("B (after copy)", B, M, N);

    if (matrices_are_equal(A, B, M, N, 1e-10)) {
        printf("Test Copy Full Matrix passed.\n");
    } else {
        printf("Test Copy Full Matrix failed.\n");
    }
    printf("\n");
}

// Función auxiliar para verificar si la parte triangular superior es correcta (sin tolerancia)
int check_upper_triangular(double *B, double *expected, int rows, int cols) {
    for (int j = 0; j < cols; j++) {
        for (int i = 0; i < rows; i++) {
            if (i <= j) {  // Verificar solo la parte triangular superior (incluyendo diagonal)
                if (B[i + j * rows] != expected[i + j * rows]) {
                    return 0;
                }
            } else {  // Fuera de la triangular superior debe ser cero
                if (B[i + j * rows] != expected[i + j * rows]) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

// Función auxiliar para verificar si la parte triangular inferior es correcta (sin tolerancia)
int check_lower_triangular(double *B, double *expected, int rows, int cols) {
    for (int j = 0; j < cols; j++) {
        for (int i = 0; i < rows; i++) {
            if (i >= j) {  // Verificar solo la parte triangular inferior (incluyendo diagonal)
                if (B[i + j * rows] != expected[i + j * rows]) {
                    return 0;
                }
            } else {  // Fuera de la triangular inferior debe ser cero
                if (B[i + j * rows] != expected[i + j * rows]) {
                    return 0;
                }
            }
        }
    }
    return 1;
}
// Prueba de copia triangular superior
void test_copy_upper() {
    int M = 3, N = 3;
    int LDA = M, LDB = M;
    double A[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};  // Matriz 3x3 en formato columna mayor
    double B[9] = {0};                          // Matriz destino inicializada a 0

    printf("Test Copy Upper Triangular Matrix:\n");
    print_matrix("A (original)", A, M, N);

    char uplo = 'U';
    dlacpy(&uplo, &M, &N, A, &LDA, B, &LDB);

    print_matrix("B (after copy)", B, M, N);

    // Matriz esperada en B después de la copia triangular superior
    double expected_B[9] = {
        1.0, 0.0, 0.0,
        4.0, 5.0, 0.0,
        7.0, 8.0, 9.0
    };

    int i=0;
    for(i=0; i<9; i++){
	if(B[i]!=expected_B[i]) break;
    }
    if (i==9) {
        printf("Test Copy Upper Triangular Matrix passed.\n");
    } else {
        printf("Test Copy Upper Triangular Matrix failed.\n");
    }
    printf("\n");
}

// Prueba de copia triangular inferior
void test_copy_lower() {
    int M = 3, N = 3;
    int LDA = M, LDB = M;
    double A[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};  // Matriz 3x3 en formato columna mayor
    double B[9] = {0};                          // Matriz destino inicializada a 0

    printf("Test Copy Lower Triangular Matrix:\n");
    print_matrix("A (original)", A, M, N);

    char uplo = 'L';
    dlacpy(&uplo, &M, &N, A, &LDA, B, &LDB);

    print_matrix("B (after copy)", B, M, N);

    // Matriz esperada en B después de la copia triangular inferior
    double expected_B[9] = {
        1, 2, 3,
        0, 5, 6,
        0, 0, 9
    };

    if (check_lower_triangular(B, expected_B, M, N)) {
        printf("Test Copy Lower Triangular Matrix passed.\n");
    } else {
        printf("Test Copy Lower Triangular Matrix failed.\n");
    }
    printf("\n");
}

int main() {
    test_copy_full();
    test_copy_upper();
    test_copy_lower();
    return 0;
}