#include <sys/time.h>
struct timeval tv1,tv2;
#include <sys/times.h>
struct tms tms1,tms2;
double treal;
double tcpu;
void TimerStart()
{
  times(&tms1);
  gettimeofday(&tv1,(struct timezone*)0);
}

void TimerStop(double *treal,double *tcpu)
{
  times(&tms2);
  gettimeofday(&tv2,(struct timezone*)0);
  *treal = (tv2.tv_sec - tv1.tv_sec) * 1000 + (tv2.tv_usec - tv1.tv_usec) / (double)1000;
//  *tcpu = ((tms2.tms_utime - tms1.tms_utime) / (double)CLK_TCK) * 1000;
}

