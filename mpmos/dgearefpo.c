#include "memoryfun.h"
//#include "mkl.h"
//#include "mkl_lapack.h"
#include "mpmos.h"
#include <stdlib.h>
#include <stdio.h>

//#define DEBUG 1

int dgearefpo(double *A11,int lda11, double *A12,int lda12, double *A21,int lda21, double *A22,int lda22, double *X0,int ldax0, double tol,int maxiter)
{
/*
%
%[X0,k]=dgearefpo(A11,A12,A21,A22,X0,tol,maxiter)
%
% dgearenfpo solves the ARE A21+A22*X-X*A11-X*A12*X=0 by the Newton - Fix point method
%
% INPUTS:
%         Aij       = Coeficient matrices of the ARE
%         X0        = Initial solution
%         tol       = Tolerance used in Newton's method
%         maxiter   = Maximum number of iterations in Newton's method
%         p3        = Dummy parameter 
%
% OUTPUTS:
%         X0        = Solution in the final time t1 (t1>=tf)
%         k         = If k=-1 then error, else number of iterations

% Version 2
*/

int k,i,j;
int info;
int pn;
int *ipiv;
double mone=-1.0,done=1.0;
double norma;
double *C22,*C21;
double *WORK;

pn=lda11;
WORK=dvector(pn);
if (WORK==NULL)
   {
   print_error("dvector","Reserva de WORK");
   }

C22=dmatrix(pn,pn);
if (C22==NULL)
   {
   print_error("dmatrix","Reserva de C22");
   }

C21=dmatrix(pn,pn);
if (C21==NULL)
   {
   print_error("dmatrix","Reserva de C21");
   }

k=0;
ipiv=ivector(pn);
if (ipiv==NULL)
   {
   print_error("ivector","Reserva de ipiv");
   }

#if DEBUG
    fprintf(stdout,"A11=\n");
    for (i=0;i<pn;i++)
        {
        for (j=0;j<pn;j++)
            fprintf(stdout,"%lf ",M(A11,i,j,pn));
        fprintf(stdout,"\n");
        }
    fprintf(stdout,"\n");

    fprintf(stdout,"A12=\n");
    for (i=0;i<pn;i++)
        {
        for (j=0;j<pn;j++)
            fprintf(stdout,"%lf ",M(A12,i,j,pn));
        fprintf(stdout,"\n");
        }
    fprintf(stdout,"\n");

    fprintf(stdout,"A21=\n");
    for (i=0;i<pn;i++)
        {
        for (j=0;j<pn;j++)
            fprintf(stdout,"%lf ",M(A21,i,j,pn));
        fprintf(stdout,"\n");
        }
    fprintf(stdout,"\n");

    fprintf(stdout,"A22=\n");
    for (i=0;i<pn;i++)
        {
        for (j=0;j<pn;j++)
            fprintf(stdout,"%lf ",M(A22,i,j,pn));
        fprintf(stdout,"\n");
        }
    fprintf(stdout,"\n");
#endif


while (k<maxiter)
    {
    k=k+1;
    //C22=A22-X0*A12;
    mcpy("A",&pn,&pn,A22,&pn,C22,&pn);
    mmul("N","N",&pn,&pn,&pn,&mone,X0,&pn,A12,&pn,&done,C22,&pn);
    #if DEBUG
    fprintf(stdout,"k=%d\n",k);
    fprintf(stdout,"C22=\n");
    for (i=0;i<pn;i++)
        {
        for (j=0;j<pn;j++)
            fprintf(stdout,"%lf ",M(C22,i,j,pn));
        fprintf(stdout,"\n");
        }
    fprintf(stdout,"\n");
    #endif
    //C21=-A21+X0*A11;
    mcpy("A",&pn,&pn,A21,&pn,C21,&pn);
    mmul("N","N",&pn,&pn,&pn,&done,X0,&pn,A11,&pn,&mone,C21,&pn);
    #if DEBUG
    fprintf(stdout,"k=%d\n",k);
    fprintf(stdout,"C21=\n");
    for (i=0;i<pn;i++)
        {
        for (j=0;j<pn;j++)
            fprintf(stdout,"%lf ",M(C21,i,j,pn));
        fprintf(stdout,"\n");
        }
    fprintf(stdout,"\n");
    #endif

    //C21=C22\C21;
    solvels(&pn,&pn,C22,&pn,ipiv,C21,&pn,&info);
    #if DEBUG
    fprintf(stdout,"C21=\n");
    for (i=0;i<pn;i++)
        {
        for (j=0;j<pn;j++)
            fprintf(stdout,"%lf ",M(C21,i,j,pn));
        fprintf(stdout,"\n");
        }
    fprintf(stdout,"\n");
    #endif
    //norma=norm(C21-X0,inf);
    #pragma omp parallel for
    for (i=0;i<pn;i++)
        {
        for (j=0;j<pn;j++)
            {
            M(X0,i,j,pn)=M(C21,i,j,pn)-M(X0,i,j,pn);
            }
        }
    #if DEBUG
    fprintf(stdout,"DGEAREFPO X0=\n");
    for (i=0;i<pn;i++)
        {
        for (j=0;j<pn;j++)
            fprintf(stdout,"%lf ",M(X0,i,j,pn));
        fprintf(stdout,"\n");
        }
    fprintf(stdout,"\n");
    #endif
    norma=mnorm("F",&pn,&pn,X0,&pn,WORK);
    #if DEBUG
    fprintf(stdout,"norma=%lf\n",norma);
    #endif
    //X0=C21;
    mcpy("A",&pn,&pn,C21,&pn,X0,&pn);
    #if DEBUG
    fprintf(stdout,"DGEAREFPO X0=\n");
    for (i=0;i<pn;i++)
        {
        for (j=0;j<pn;j++)
            fprintf(stdout,"%lf ",M(X0,i,j,pn));
        fprintf(stdout,"\n");
        }
    fprintf(stdout,"\n");
    #endif
    if (norma<tol)
        break;
    }
if (k==maxiter)
    k=-1;

free(C22);
free(C21);
free(WORK);
free(ipiv);

return k;
}
