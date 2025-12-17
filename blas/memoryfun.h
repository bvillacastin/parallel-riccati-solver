void print_error(char *rutina, char *texto_err);
double *dvector(int nh);
int *ivector(int nh);
double *dmatrix(int nfh, int nch);
int *imatrix(int nfh, int nch);

void liberar_memoria1b(int *z,int *ns,int*nh,int *os,int *oh,int *cd,int *ca,int *nps,int *nph,
                       int *cooik11,int *acooik11,int *coojk11,int *cooic,int *coojc,
                       int *csrik11,int *csrjk11,
					   double *xp,double *lp,double *fp,double *pp,double *r,double *x,
					   double *yoe,double *m,double *fap,double *kp,double *cp,
                       double *mp,double *xi,double *xpa,double *yo,double *dyo,double *y1,
					   double *dy1, double *ls,double *lh,double *vr1,double *vr2,
                       double *zep,double *lo,double *frp,double *cae,double *hoc,double *hot,
					   double *lx,double *fc, double *cook11,double *cooc,double *csrk11,
					   double *dwork);


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
					   double *cook_1, double *cook_2,double *dwork);

    
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
					   double *vcalpos1, double *vcalpos2, double *dwork);

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
					   double *vcalpos1, double *vcalpos2, double *dwork);

/* definicion de la macro para realizar m�s facilmente el acceso a las matrices del problema */

#define M(a,i,j,lda) (a)[(i)+(j)*(lda)]

