#include <stdio.h>
#include "ompblas.h"   // Asegúrate de incluir tu biblioteca que contiene dlange
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

// Función para probar dlange con diferentes normas
void test_dlange(char *NORM, int M, int N, double *A, int LDA, double *WORK, double expected) {
    double result = dlange(NORM, &M, &N, A, &LDA, WORK);

    printf("Test dlange with NORM='%c':\n", *NORM);
    print_matrix("A (matrix)", A, M, N);
    printf("Expected: %10.4f\n", expected);
    printf("Result:   %10.4f\n", result);

    if (result == expected) {
        printf("Test passed.\n");
    } else {
        printf("Test failed. Expected %10.4f but got %10.4f\n", expected, result);
    }
    printf("\n");
}

int main() {
    int M = 3, N = 3, LDA = 3;
    double A[9] = {
        1, -2, 3, 
        -4, 5, -6, 
        7, -8, 9
    };
    double WORK[3] = {0};  // Espacio de trabajo necesario para la norma infinito

    // Test 1: Norma máximo absoluto ('M')
    test_dlange("M", M, N, A, LDA, WORK, 9.0);

    // Test 2: Norma uno ('1')
    test_dlange("1", M, N, A, LDA, WORK, 24.0);  // Suma máxima de las columnas

    // Test 3: Norma infinito ('I')
    test_dlange("I", M, N, A, LDA, WORK, 18.0);  // Suma máxima de las filas

    // Test 4: Norma Frobenius ('F')
    test_dlange("F", M, N, A, LDA, WORK, 16.8819);  // Raíz cuadrada de la suma de cuadrados

    return 0;
}
