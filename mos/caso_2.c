#include <math.h>
#include "memoryfun.h"
#include "mos.h"
#include "caso_2.h"

int init(int pn,double pr,double *t0, double *x0)
{
int n;
double alpha=0.0,beta=0.0;

*t0=0.0;
mset("A",&pn,&pn,&alpha,&beta,x0,&pn);
return 0;

}

int feval(int pn,double pr,double t,double *A11,double *A12,double *A21,double *A22)
{
int i,j;
int n,ld;
double a,b,alpha,beta;


alpha=0.0;
beta=0.0;

/* A11 */
mset("A",&pn,&pn,&alpha,&beta,A11,&pn);

/* A22 */

mset("A",&pn,&pn,&alpha,&beta,A22,&pn);

/* A12 */

mset("A",&pn,&pn,&alpha,&pr,A12,&pn);

/* A21 */

mset("A",&pn,&pn,&alpha,&pr,A21,&pn);

return 0;
}


/*function [X]=datasv1(pn,pr,t)
%[X]=datasv1(pn,pr,t)
if t<0
	X=t;
else
	X=0;
end
*/

int solut(int pn,double pr,double t,double *X)
{
int n;
double t0;
double alpha,beta;

n=(int)pow(2,pn);
t0=0;

alpha=0;
beta=(1+tan(cos(t)-cos(t0)))/(1-tan(cos(t)-cos(t0)));
mset("A",&n,&n,&alpha,&beta,&X,&n);

return 0;
}
