# Se encarga de ejecutar las pruebas a las funciones de las librerias
# y las del algoritmo con las distintas librerias
#!/bin/bash
./compilaTodo.sh
cd blas/lib/
./runTest.sh
cd -
cd mpmos/lib/
./runTest.sh ft
./runTest.sh ht
./runTest.sh dt
./runTest.sh qt
cd -
cd mos/lib/
./runTest.sh 
cd -
./correr_versiones.sh
./guardarResultados.sh