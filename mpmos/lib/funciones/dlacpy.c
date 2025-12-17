#include <stdio.h>

void dlacpy(char *UPLO, int *M, int *N, double *A, int *LDA, double *B, int *LDB) {
    if (*UPLO == 'A' || *UPLO == 'a') {  // Copiar toda la matriz
        for (int j = 0; j < *N; j++) {
            for (int i = 0; i < *M; i++) {
                B[i + j * (*LDB)] = A[i + j * (*LDA)];
            }
        }
    } else if (*UPLO == 'U' || *UPLO == 'u') {  // Copiar solo la parte triangular superior
        for (int j = 0; j < *N; j++) {
            for (int i = 0; i <= j && i < *M; i++) {  // Solo copiar si i <= j
                B[i + j * (*LDB)] = A[i + j * (*LDA)];
            }
        }
    } else if (*UPLO == 'L' || *UPLO == 'l') {  // Copiar solo la parte triangular inferior
        for (int j = 0; j < *N; j++) {
            for (int i = j; i < *M; i++) {  // Solo copiar si i >= j
                B[i + j * (*LDB)] = A[i + j * (*LDA)];
            }
        }
    }
}