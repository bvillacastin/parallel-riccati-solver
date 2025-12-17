/*
function pridr_bdf(k,pn,pr,H,Tf,r,tol,maxiter)
%pridr_bdf(2,16,100,0.01,10,3,1e-6,100)
%pridr_bdf(3,2,100,0.01,1,3,1e-6,100)
%pridr_bdf(3,2,100,[0.1 0.01],1,3,1e-6,100)
*/

#include <stdio.h>
#include <stdlib.h>
#include "memoryfun.h"
//#include <mkl.h>
#include "timer.h"
#include "dgeidrbdf.h"

//#define DEBUG 1

int main(int argc,char **argv)
{


// Parámetros de entrada
//
int i,j; // Para los bucles
int k; // Problema a considerar
int pn; // Tamaño del problema
double pr; // Rigidez de la Ecuación Diferencial de Riccati
double h; // Paso de integración
double Tf; // Tiempo final
int r; 
double tol; // Tolerancia
int maxiter; // número máximo de iteraciones en el método de Newton

// Variables

FILE *fsal;
double treal,tcpu; /* Tiempo para evaluación de prestaciones */
double *X0; /* Almacena el resultado */
double time; /* Tiempo de simulación */
int n_iter; /* Numero de iteraciones */


// Comprobación de número correcto de parámetros
//

//fsal=fopen("resultado.txt","w+");
fsal=stdout;
if (fsal==NULL)
   {
   fprintf(stdout,"Error en la apertura del fichero de salida\n");
   return 1;
   }

if (argc!=9)
   {
   fprintf(fsal,"Error de Sintaxis: ./pravdr problema tamaño rigidez paso tiempo_final r tolerancia iteraciones\n");
   return 2;
   }

k=atoi(argv[1]);
//fprintf(stdout, "argv[1] = %s	(k, Problema a considerar)\n", argv[1]);
pn=atoi(argv[2]);
//fprintf(stdout, "argv[2] = %s	(pn, Tamaño Problema)\n", argv[2]);
pr=atof(argv[3]);
//fprintf(stdout, "argv[3] = %s	(pr, Rigidez Problema\n", argv[3]);
h=atof(argv[4]);
//fprintf(stdout, "argv[4] = %s	(h, Paso de Integracion)\n", argv[4]);
Tf=atof(argv[5]);
//fprintf(stdout, "argv[5] = %s	(Tf, Tiempo Final)\n", argv[5]);
r=atoi(argv[6]);
//fprintf(stdout, "argv[6] = %s	(r, ???)\n", argv[6]);
tol=atof(argv[7]);
//fprintf(stdout, "argv[7] = %s	(tol, Tolerancia)\n", argv[7]);
maxiter=atoi(argv[8]);
//fprintf(stdout, "argv[8] = %s	(maxiter, Maximo iteraciones Metodo de Newton)\n", argv[8]);

#if DEBUG
fprintf(fsal,"Los parámetros de entrada son:\n");
fprintf(fsal,"\tk=%d pn=%d pr=%e h=%e Tf=%e r=%d tol=%e maxiter=%d\n\n",k,pn,pr,h,Tf,r,tol,maxiter);
#endif


/*
[X0,t,k]=dgeidrbdf(pn,pr,f,f0,tf,h,r,'dgearefpo',tol,maxiter);
*/

X0=dmatrix(pn,pn);
if (X0==NULL)
   {
   print_error("dmatrix","Reserva de x0");
   }

time=0;
n_iter=0;

//omp_set_num_threads(10);
TimerStart();

dgeidrbdf(pn,pr,Tf,h,r,tol,maxiter,fsal,X0,&time,&n_iter);

TimerStop(&treal,&tcpu);
//fprintf(fsal,"\n Tiempo de simulacion =%lf segundos y  Numero de iteraciones=%d \n",time,n_iter);
//fprintf(fsal,"\n Tiempo real=%lf segundos \n",treal/1000.0);
fprintf(fsal,"%lf\n",treal/1000.0);

/*fprintf(fsal, "PRIDR X0=\n");
for (i=0;i<pn;i++)
    {
    for (j=0;j<pn;j++)
        fprintf(fsal,"%lf ",M(X0,i,j,pn));
    fprintf(fsal,"\n");
    }
fprintf(fsal,"\n");*/

return 0;
}
