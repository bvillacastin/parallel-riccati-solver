F77         = f77
CC          = gcc
OPTIONS     = -O3 -fopenmp -DEBUG #-DDEBUF
CFLAGS      = $(OPTIONS) -I.
MATHLIB     = -lm libmos.a #-llapack -lblas

# Caso de estudio 2
OBJSC = pridr.o timer.o memoryfun.o dgeidrbdf.o caso_2.o dgearefpo.o
OBJSF = 
OBJS  = $(OBJSC) $(OBJSF)

default: caso_v3

clean:	
	rm -f *.o  
	rm -f *.exe
	rm -f *.sal
	rm -f salida

caso_v3: $(OBJS)
	$(CC) $(CFLAGS) -o pridr.exe $(OBJS) $(MATHLIB) -lc

.f.o:
	$(F77) $(F77FLAGS) -c $<

.c.o:
	$(CC) -c $(CFLAGS) $(CDEFS) $<
