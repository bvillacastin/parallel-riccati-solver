#ifndef MPMOS_H
#define MPMOS_H

void solvels(int *N, int *NRHS, double *A, int *LDA, int *IPIV,double *B, int *LDB, int *INFO );
double mnorm(char *NORM, int *M, int *N, double *A, int *LDA, double *WORK);
void mset(char *UPLO, int *M, int *N, double *ALPHA, double *BETA, double *A, int *LDA);
void mmul(char *TRANSA, char *TRANSB, int *M, int *N, int *K, double *ALPHA, double *A, int *LDA, double *B, int *LDB, double *BETA, double *C, int *LDC);
void mcpy(char *UPLO, int *M, int *N, double *A, int *LDA, double *B, int *LDB);
void mmulc(double *D, int t, double *A, double c);
void msub(double *D, int t, double *A, double *B);

#endif