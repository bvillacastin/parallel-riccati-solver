#include "math.h"
#include "memoryfun.h"
//#include <mkl.h>
#include "mpmos.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "dgearefpo.h"
#include "caso_2.h"

#define MAX(x,y)      (x<y)?y:x;
//#define DEBUG 1

int dgeidrbdf(int pn,double pr, double tf, double h, int r,double tol, int maxiter,FILE *fsal,double *X0,double *time, int *n_iter)
{

double beta[7]={0,1.00000, 0.66667, 0.54545, 0.48000, 0.43796, 0.40816};
double alfa[7][7]={0,0,0,0,0,0,0,
      0 ,1.00000 ,  0.00000 , 0.00000 , 0.00000 , 0.00000 , 0.00000,
      0 ,1.33333 , -0.33333 , 0.00000 , 0.00000 , 0.00000 , 0.00000,
      0 ,1.63636 , -0.81818 , 0.18182 , 0.00000 , 0.00000 , 0.00000,
      0 ,1.92000 , -1.44000 , 0.64000 ,-0.12000 , 0.00000 , 0.00000,
      0 ,2.18978 , -2.18978 , 1.45985 ,-0.54745 , 0.08759 , 0.00000,
      0 ,2.44898 , -3.06122 , 2.72109 ,-1.53061 , 0.48980 ,-0.06803};

int i,j,k; /* Indices de bucles */
int error=0; /* C�digo de error */
int niter=0; /* Numero de iteraciones acumuladas */
double t0; /* Tiempo inicio de la simulaci�n */
double t; /* Tiempo actual */
double *A11,*A12,*A21,*A22;
double *A11m,*A12m,*A21m,*A22m;

int one=1;
int iter; /* Indica si la llamada a dgearefpo ha finalizado correctamente. En
             caso de finalizar correctamente contiene el numero de iteraciones
             instantaneas */
double t1;
double hb; /* Paso */
double *X1; /* Copia de las X0 anteriores */
double alfae; /* Valor estraido de alfa[][] */

/* Comprobaci�n de los par�metros de entrada */ 

#if DEBUG
fprintf(fsal,"Los par�metros de entrada son:\n");
fprintf(fsal,"\tpn=%d pr=%e h=%e Tf=%e r=%d tol=%e maxiter=%d\n\n",pn,pr,h,tf,r,tol,maxiter);
#endif


/*
% Obtaining the initial conditions
[t0,X0]=feval(data0,pn,pr);
*/
/*
X0=dmatrix(pn,pn);
if (X0==NULL)
   {
   print_error("dmatrix","Reserva de x0");
   }
*/

error=init(pn,pr,&t0,X0);
if (error!=0)
   {
   print_error("init","");
   }

#if DEBUG
fprintf(fsal,"t0=%lf\n",t0);
for (i=0;i<pn;i++)
    {
    for (j=0;j<pn;j++)
        fprintf(fsal,"%lf ",M(X0,i,j,pn));
    fprintf(fsal,"\n");
    }
#endif

// The following loop computes the solutions in the first r-1 steps (X1, X2,...,Xr-1). We consider r=1, the simplest form.

niter=0;
t=t0;

// [A11,A12,A21,A22]=feval(datat,pn,pr,t(1));
A11=dmatrix(pn,pn);
if (A11==NULL)
   {
   print_error("dmatrix","Reserva de A11");
   }

A12=dmatrix(pn,pn);
if (A12==NULL)
   {
   print_error("dmatrix","Reserva de A12");
   }

A21=dmatrix(pn,pn);
if (A21==NULL)
   {
   print_error("dmatrix","Reserva de A21");
   }

A22=dmatrix(pn,pn);
if (A22==NULL)
   {
   print_error("dmatrix","Reserva de A22");
   }

error=feval(pn,pr,t,A11,A12,A21,A22);

if (error!=0)
   {
   print_error("feval","");
   }

#if DEBUG
for (i=0;i<pn;i++)
    {
    for (j=0;j<pn;j++)
        fprintf(fsal,"%lf ",M(A11,i,j,pn));
    fprintf(fsal,"\n");
    }
    fprintf(fsal,"\n");
for (i=0;i<pn;i++)
    {
    for (j=0;j<pn;j++)
        fprintf(fsal,"%lf ",M(A12,i,j,pn));
    fprintf(fsal,"\n");
    }
    fprintf(fsal,"\n");
for (i=0;i<pn;i++)
    {
    for (j=0;j<pn;j++)
        fprintf(fsal,"%lf ",M(A21,i,j,pn));
    fprintf(fsal,"\n");
    }
    fprintf(fsal,"\n");
for (i=0;i<pn;i++)
    {
    for (j=0;j<pn;j++)
        fprintf(fsal,"%lf ",M(A22,i,j,pn));
    fprintf(fsal,"\n");
    }
    fprintf(fsal,"\n");
#endif

/* Vamos a considerar el caso m�s sencillo de r=1. En este caso el primer
   bucle de c�lculo de los 1 hasta r-1 no se lleva a cabo. De igual manera
   en el segundo bucle no se realizan copias. */

hb=h*beta[r];

#if DEBUG
fprintf(fsal,"hb=%lf\n",hb);
#endif

X1=dmatrix(pn,pn);
if (X1==NULL)
   {
   print_error("dmatrix","Reserva de x1");
   }

A11m=dmatrix(pn,pn);
if (A11m==NULL)
   {
   print_error("dmatrix","Reserva de A11m");
   }

A12m=dmatrix(pn,pn);
if (A12m==NULL)
   {
   print_error("dmatrix","Reserva de A12m");
   }

A21m=dmatrix(pn,pn);
if (A21m==NULL)
   {
   print_error("dmatrix","Reserva de A21m");
   }

A22m=dmatrix(pn,pn);
if (A22m==NULL)
   {
   print_error("dmatrix","Reserva de A22m");
   }


while (t<tf+h)
    {
    /* Copia innecesaria al considerar r=1 y en el bucle k=1 */
    t=t+h;
    
    // BDF step

    // C�lculo de A21m. 
    k=1;
    alfae=alfa[r][k];
    #pragma omp parallel for
    for (i=0;i<pn;i++)
        {
        for (j=0;j<pn;j++)
            {
            M(A21m,i,j,pn)=hb*M(A21,i,j,pn)+alfae*M(X0,i,j,pn);
            }
        }

    // C�lculo de A22m
    mcpy("A",&pn,&pn,A22,&pn,A22m,&pn);    
    #pragma omp parallel for
    for (i=0;i<pn;i++)
        {
        for (j=0;j<pn;j++)
            {
            M(A22m,i,j,pn)=hb*M(A22m,i,j,pn);
            if (i==j)
               {
               M(A22m,i,j,pn)=M(A22m,i,j,pn)-1;
               }
            }
        }

    // C�lculo de A11m
    mcpy("A",&pn,&pn,A11,&pn,A11m,&pn);
    #pragma omp parallel for
    for (i=0;i<pn;i++)
        {
        for (j=0;j<pn;j++)
            {
            M(A11m,i,j,pn)=hb*M(A11m,i,j,pn);
            }
        }

    // C�lculo de A12m
    mcpy("A",&pn,&pn,A12,&pn,A12m,&pn);
    #pragma omp parallel for
    for (i=0;i<pn;i++)
        {
        for (j=0;j<pn;j++)
            {
            M(A12m,i,j,pn)=hb*M(A12m,i,j,pn);
            }
        }
    #if DEBUG
    fprintf(stdout,"A11m\n");
    for (i=0;i<pn;i++)
        {
        for (j=0;j<pn;j++)
            fprintf(fsal,"%lf ",M(A11m,i,j,pn));
        fprintf(fsal,"\n");
        }
        fprintf(fsal,"\n");
    fprintf(stdout,"A12m\n");
    for (i=0;i<pn;i++)
        {
        for (j=0;j<pn;j++)
            fprintf(fsal,"%lf ",M(A12m,i,j,pn));
        fprintf(fsal,"\n");
        }
        fprintf(fsal,"\n");
    fprintf(stdout,"A21m\n");
    for (i=0;i<pn;i++)
        {
        for (j=0;j<pn;j++)
            fprintf(fsal,"%lf ",M(A21m,i,j,pn));
        fprintf(fsal,"\n");
        }
        fprintf(fsal,"\n");
    fprintf(stdout,"A22m\n");
    for (i=0;i<pn;i++)
        {
        for (j=0;j<pn;j++)
            fprintf(fsal,"%lf ",M(A22m,i,j,pn));
        fprintf(fsal,"\n");
        }
        fprintf(fsal,"\n");
    #endif

    // Llamada a dgarefpo
    iter=dgearefpo(A11m,pn,A12m,pn,A21m,pn,A22m,pn,X0,pn,tol,maxiter);
    niter=niter+iter;
    #if DEBUG
    fprintf(fsal,"t=%lf with k=%d of a total=%d\n",t,iter,niter);
    fprintf(fsal,"DGEIDRBDF X0\n");
    for (i=0;i<pn;i++)
        {
        for (j=0;j<pn;j++)
            fprintf(fsal,"%lf ",M(X0,i,j,pn));
        fprintf(fsal,"\n");
        }
        fprintf(fsal,"\n");
    #endif
    t1=t;
    if (iter==-1)
       {
       *n_iter=-1;
       return 0;
       }
    }

*time=t1;
*n_iter=niter;

return 0;
}

