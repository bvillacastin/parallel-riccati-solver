# Copia todos los resultados obtenidos en las pruebas en la carpeta Resultados
#!/bin/bash
cp -f blas/lib/res_blas.txt Resultados/res_blas.txt
cp -f mpmos/lib/res_mpmos.txt Resultados/res_mpmos.txt
cp -f mpmos/lib/res_mpmos_ht.txt Resultados/res_mpmos_ht.txt
cp -f mpmos/lib/res_mpmos_dt.txt Resultados/res_mpmos_dt.txt
cp -f mpmos/lib/res_mpmos_qt.txt Resultados/res_mpmos_qt.txt
cp -f mos/lib/res_mos.txt Resultados/res_mos.txt
cp -f tiempos.txt Resultados/tiempos.txt