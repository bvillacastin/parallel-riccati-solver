#include <stdio.h>

void dgemm(char *TRANSA, char *TRANSB, int *M, int *N, int *K,
           double *ALPHA, double *A, int *LDA, double *B, int *LDB,
           double *BETA, double *C, int *LDC) {
    
    // Determinamos si A y/o B deben transponerse
    int trans_a = (*TRANSA == 'T' || *TRANSA == 't');
    int trans_b = (*TRANSB == 'T' || *TRANSB == 't');

    // Escalamos la matriz C por BETA
    for (int j = 0; j < *N; j++) {
        for (int i = 0; i < *M; i++) {
            C[i + j * (*LDC)] *= *BETA;
        }
    }

    // Calculamos el producto de matrices y lo sumamos a C
    for (int j = 0; j < *N; j++) {
        for (int i = 0; i < *M; i++) {
            double sum = 0.0;
            for (int l = 0; l < *K; l++) {
                // Selecciona los elementos correctos dependiendo de si A o B están transpuestas
                double a_val = trans_a ? A[l + i * (*LDA)] : A[i + l * (*LDA)];
                double b_val = trans_b ? B[j + l * (*LDB)] : B[l + j * (*LDB)];
                sum += a_val * b_val;
            }
            C[i + j * (*LDC)] += *ALPHA * sum;
        }
    }
}
