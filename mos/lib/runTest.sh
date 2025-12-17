#!/bin/bash
# Archivo de resultados
OUTPUT_FILE="res_mos.txt"

# Crear (o truncar) el archivo de resultados
: > "$OUTPUT_FILE"

# Escribir cabecera
echo "Resultados de las pruebas de test.o" >> "$OUTPUT_FILE"
echo "===================================" >> "$OUTPUT_FILE"

# Función para obtener los tamaños específicos según la función
get_sizes() {
    case $1 in
        "solvels") echo "100 1000 2000 3000 4000 5000 6000 7000 8000 9000";;
        "mmul") echo "100 1000 2000 3000 4000 5000 6000 7000 8000 9000";;
        "mcpy") echo "250 500 1000 2000 4000 8000 16000 32000";;
        "mset") echo "250 500 1000 2000 4000 8000 16000 32000";;
        "mnorm") echo "250 500 1000 2000 4000 8000 16000 32000";;
        "mmulc") echo "250 500 1000 2000 4000 8000 16000 32000";;
        "msub") echo "250 500 1000 2000 4000 8000 16000 32000";;
        *) echo ""; return 1;;
    esac
    return 0
}

# Lista de funciones a probar
FUNCTIONS=("solvels" "mmul" "mcpy" "mset" "mnorm" "mmulc" "msub")

# Iterar sobre las funciones
for FUNC in "${FUNCTIONS[@]}"; do
    echo -e "\nFunción: $FUNC" >> "$OUTPUT_FILE"
    echo "N       Tiempo (segundos)" >> "$OUTPUT_FILE"
    echo "--------------------------" >> "$OUTPUT_FILE"

    # Obtener tamaños específicos para la función
    SIZES=$(get_sizes "$FUNC")
    if [ $? -ne 0 ]; then
        echo "Error al obtener tamaños para la función $FUNC."
        continue
    fi

    # Iterar sobre los tamaños
    for N in $SIZES; do
        # Ejecutar el programa y capturar el tiempo
        TIME=$(./test.o "$FUNC" "$N")
        
        # Verificar si la ejecución fue exitosa
        if [ $? -eq 0 ]; then
            echo "$N     $TIME" >> "$OUTPUT_FILE"
        else
            echo "$N     ERROR" >> "$OUTPUT_FILE"
        fi
    done
done

# Mensaje final
echo "Pruebas completadas. Resultados guardados en $OUTPUT_FILE."
