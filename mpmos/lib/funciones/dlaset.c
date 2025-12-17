#include <stdio.h>

void dlaset(char *UPLO, int *M, int *N, double *ALPHA, double *BETA, double *A, int *LDA) {
    if (*UPLO == 'A' || *UPLO == 'a') {  // Inicializar toda la matriz
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
