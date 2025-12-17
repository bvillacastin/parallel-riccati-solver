#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include "memoryfun.h"

void print_error(rutina,texto_err)
char rutina[],texto_err[];

/* Manejo standard de errores */
{
        void exit();

        printf("ERROR DE EJECUCION EN RUTINA : %s\n",rutina);
        printf("%s\n",texto_err);
        printf("...EJECUCION DE PROGRAMA SUSPENDIDA...\n");
        exit(1);
}


double *dvector(int nh)

/* Dimensiona y reserve espacio para un vector double de rango [n1..nh] */
{
        double *v;

        v=(double *) malloc((nh)*sizeof(double));
        return v;
}

int *ivector(int nh)

/* Dimensiona y reserve espacio para un vector double de rango [n1..nh] */
{
        int *v;

        v=(int *) malloc((nh)*sizeof(int));
        return v;
}

double *dmatrix (int nfh, int nch) 

/* Dimensiona y reserva espacio para una matriz double [nf1..nfh][nc1..nch] */
{
	double *m;
	
	m=(double *)malloc((nfh)*(nch)*sizeof(double));
	return m;
}

int *imatrix (int nfh, int nch) 

/* Dimensiona y reserva espacio para una matriz int [nf1..nfh][nc1..nch] */
{
	int *m;
	
	m=(int *)malloc((nfh)*(nch)*sizeof(int));
	return m;
}


void liberar_memoria1b(int *z,int *ns,int*nh,int *os,int *oh,int *cd,int *ca,int *nps,int *nph,
                       int *cooik11,int *acooik11,int *coojk11,int *cooic,int *coojc,
                       int *csrik11,int *csrjk11,
					   double *xp,double *lp,double *fp,double *pp,double *r,double *x,
					   double *yoe,double *m,double *fap,double *kp,double *cp,
                       double *mp,double *xi,double *xpa,double *yo,double *dyo,double *y1,
					   double *dy1, double *ls,double *lh,double *vr1,double *vr2,
                       double *zep,double *lo,double *frp,double *cae,double *hoc,double *hot,
					   double *lx,double *fc, double *cook11,double *cooc,double *csrk11,
					   double *dwork)
{
free(z);
free(ns);
free(nh);
free(os);
free(oh);
free(cd);
free(ca);
free(nps);
free(nph);
free(cooik11);
free(acooik11);
free(coojk11);
free(cooic);
free(coojc);
free(csrik11);
free(csrjk11);
free(xp);
free(lp);
free(fp);
free(pp);
free(r);
free(x);
free(yoe);
free(m);
free(fap);
free(kp);
free(cp);
free(mp);
free(xi);
free(xpa);
free(yo);
free(dyo);
free(y1);
free(dy1);
free(ls);
free(lh);
free(vr1);
free(vr2);
free(zep);
free(lo);
free(frp);
free(cae);
free(hoc);
free(hot);
free(lx);
free(fc); 
free(cook11);
free(cooc);
free(csrk11);
free(dwork);

}

void liberar_memoria2b(int *z,int *ns,int *nh1,int *nh2,int *os,int *oh,int *cd,int *ca,
					   int *nps,int *nph,
                       int *cooik11,int *acooik11,int *coojk11,int *cooic,int *coojc,
                       int *csrik11,int *csrjk11,
					   int *cooik_1, int *coojk_1,int *cooik_2, int *coojk_2,
					   double *xp,double *lp,double *fp,double *pp,double *r,double *x,
					   double *yoe,double *m,double *fap,double *kp,double *cp,
                       double *mp,double *xi,double *xpa,double *yo,double *dyo,double *y1,
					   double *dy1, double *ls,double *lh,double *vr1,double *vr2, double *vr3,double *vr4,
                       double *zep,double *lo,double *frp,double *cae,double *hoc,double *hot,
					   double *lx,double *fc, double *cook11,double *cooc,double *csrk11,
					   double *cook_1, double *cook_2,double *dwork)
{
free(z);
free(ns);
free(nh1);
free(nh2);
free(os);
free(oh);
free(cd);
free(ca);
free(nps);
free(nph);
free(cooik11);
free(acooik11);
free(coojk11);
free(cooic);
free(coojc);
free(csrik11);
free(csrjk11);
free(cooik_1);
free(coojk_1);
free(cooik_2);
free(coojk_2);
free(xp);
free(lp);
free(fp);
free(pp);
free(r);
free(x);
free(yoe);
free(m);
free(fap);
free(kp);
free(cp);
free(mp);
free(xi);
free(xpa);
free(yo);
free(dyo);
free(y1);
free(dy1);
free(ls);
free(lh);
free(vr1);
free(vr2);
free(vr3);
free(vr4);
free(zep);
free(lo);
free(frp);
free(cae);
free(hoc);
free(hot);
free(lx);
free(fc); 
free(cook11);
free(cooc);
free(csrk11);
free(cook_1);
free(cook_2);
free(dwork);

}

    
void liberar_memoriay1b(int *z,int *ns,int *ns1,int *ns2,int *nh,int *os,int *oh,
						int *cd,int *ca,int *nes,int *ney1,int *ney2,int *neh,
				       int *cooik11,int *acooik11,int *coojk11,int *cooic,int *coojc,
                       int *csrik11,int *csrjk11,
					   double *xp,double *lp,double *fp,double *pp,double *r,double *x,
					   double *yoe,double *m,double *fap,double *kp,double *cp,
                       double *mp,double *xi,double *xpa,double *yo,double *dyo,double *y1,
					   double *dy1, double *ls,double *lh,double *vr1,double *vr2,
                       double *zep,double *lo,double *frp,double *cae,double *hoc,double *hot,
					   double *lx,double *fc, double *cook11,double *cooc,double *csrk11,
					   double *xps, double *xpy1, double *xpy2, double *l1, double *l2,
					   double *vcalpos1, double *vcalpos2, double *dwork)
{
free(z);
free(ns);
free(ns1);
free(ns2);
free(nh);
free(os);
free(oh);
free(cd);
free(ca);
free(nes);
free(ney1);
free(ney2);
free(neh);
free(cooik11);
free(acooik11);
free(coojk11);
free(cooic);
free(coojc);
free(csrik11);
free(csrjk11);
free(xp);
free(lp);
free(fp);
free(pp);
free(r);
free(x);
free(yoe);
free(m);
free(fap);
free(kp);
free(cp);
free(mp);
free(xi);
free(xpa);
free(yo);
free(dyo);
free(y1);
free(dy1);
free(ls);
free(lh);
free(vr1);
free(vr2);
free(zep);
free(lo);
free(frp);
free(cae);
free(hoc);
free(hot);
free(lx);
free(fc); 
free(cook11);
free(cooc);
free(csrk11);
free(xps);
free(xpy1);
free(xpy2);
free(l1);
free(l2);
free(vcalpos1);
free(vcalpos2);
free(dwork);

}


void liberar_memoriay2b(int *z,int *ns,int *ns1,int *ns2,int *nh1, int *nh2,int *os,int *oh,
						int *cd,int *ca,int *nes,int *ney1,int *ney2,int *neh,
				       int *cooik11,int *acooik11,int *coojk11,int *cooic,int *coojc,
                       int *csrik11,int *csrjk11,int *cooik_1, int *coojk_1, int *cooik_2, int *coojk_2,
					   double *xp,double *lp,double *fp,double *pp,double *r,double *x,
					   double *yoe,double *m,double *fap,double *kp,double *cp,
                       double *mp,double *xi,double *xpa,double *yo,double *dyo,double *y1,
					   double *dy1, double *ls,double *lh,double *vr1,double *vr2,double *vr3, double *vr4,
                       double *zep,double *lo,double *frp,double *cae,double *hoc,double *hot,
					   double *lx,double *fc, double *cook11,double *cooc,double *csrk11,double *cook_1, double *cook_2,
					   double *xps, double *xpy1, double *xpy2, double *l1, double *l2,
					   double *vcalpos1, double *vcalpos2, double *dwork)
{
free(z);
free(ns);
free(ns1);
free(ns2);
free(nh1);
free(nh2);
free(os);
free(oh);
free(cd);
free(ca);
free(nes);
free(ney1);
free(ney2);
free(neh);
free(cooik11);
free(acooik11);
free(coojk11);
free(cooic);
free(coojc);
free(csrik11);
free(csrjk11);
free(cooik_1);
free(coojk_1);
free(cooik_2);
free(coojk_2);
free(xp);
free(lp);
free(fp);
free(pp);
free(r);
free(x);
free(yoe);
free(m);
free(fap);
free(kp);
free(cp);
free(mp);
free(xi);
free(xpa);
free(yo);
free(dyo);
free(y1);
free(dy1);
free(ls);
free(lh);
free(vr1);
free(vr2);
free(vr3);
free(vr4);
free(zep);
free(lo);
free(frp);
free(cae);
free(hoc);
free(hot);
free(lx);
free(fc); 
free(cook11);
free(cooc);
free(csrk11);
free(cook_1);
free(cook_2);
free(xps);
free(xpy1);
free(xpy2);
free(l1);
free(l2);
free(vcalpos1);
free(vcalpos2);
free(dwork);

}