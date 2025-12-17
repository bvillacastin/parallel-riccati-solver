# Compila las librerias y las versiones del algoritmo
#!/bin/bash
cd mpmos/lib/
./montarlib
./compt
cd ..
make clean
cp -f lib/libmpmos.a libmpmos.a
make
cd ..
cd mos/lib/
./montarlib
./compt
cd ..
make clean
cp -f lib/libmos.a libmos.a
make
cd ..
cd blas/lib/
./compt
cd ..
make clean
make
cd ..
