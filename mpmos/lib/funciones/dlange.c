#include <stdio.h>
#include <stdlib.h>
#include <math.h>   // Para fabs y sqrt
#include <float.h>  // Para DBL_MAX

double dlange(char *NORM, int *M, int *N, double *A, int *LDA, double *WORK) {
    double norm = 0.0;

    // Norma Máximo Absoluto ('M')
    if (*NORM == 'M' || *NORM == 'm') {
        for (int j = 0; j < *N; j++) {
            for (int i = 0; i < *M; i++) {
                double abs_val = fabs(A[i + j * (*LDA)]);
                if (abs_val > norm) {
                    norm = abs_val;
                }
            }
        }
    }
    // Norma Uno ('1')
    else if (*NORM == '1' || *NORM == 'O' || *NORM == 'o') {
        for (int j = 0; j < *N; j++) {
            double col_sum = 0.0;
            for (int i = 0; i < *M; i++) {
                col_sum += fabs(A[i + j * (*LDA)]);
            }
            if (col_sum > norm) {
                norm = col_sum;
            }
        }
    }
    // Norma Infinito ('I')
    else if (*NORM == 'I' || *NORM == 'i') {
        for (int i = 0; i < *M; i++) {
            WORK[i] = 0.0; // Inicializar el espacio de trabajo
        }
        for (int j = 0; j < *N; j++) {
            for (int i = 0; i < *M; i++) {
                WORK[i] += fabs(A[i + j * (*LDA)]);
            }
        }
        // Encontrar el valor máximo en WORK (la suma máxima de filas)
        for (int i = 0; i < *M; i++) {
            if (WORK[i] > norm) {
                norm = WORK[i];
            }
        }
    }
    // Norma Frobenius ('F' o 'E')
    else if (*NORM == 'F' || *NORM == 'f' || *NORM == 'E' || *NORM == 'e') {
        double sum = 0.0; // Para acumular la suma de cuadrados
        for (int j = 0; j < *N; j++) {
            for (int i = 0; i < *M; i++) {
                double val = A[i + j * (*LDA)];
                sum += val * val; // Acumula el cuadrado de cada elemento
            }
        }
        norm = sqrt(sum); // Calcula la raíz cuadrada de la suma
    }
    
    return norm;
}