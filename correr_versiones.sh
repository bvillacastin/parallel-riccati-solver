#Corre el algortitmo utilizando diferentes versiones de las librerias
#!/bin/bash

# Directorios de las versiones del proyecto
declare -A versiones=(
  ["MS"]="/home/borja/tfg/mos"
  ["MPMS12t"]="/home/borja/tfg/mpmos"
  ["MPMS24t"]="/home/borja/tfg/mpmos"
  ["MPMS6t"]="/home/borja/tfg/mpmos"
  ["MPMS3t"]="/home/borja/tfg/mpmos"
)

# Valores de N
valores_N=(10 50 100 200 400 800 1000)

# Archivo de salida
archivo_salida="tiempos.txt"

# Limpiar archivo de salida
> "$archivo_salida"

# Escribir cabecera
echo -e "Versión\tN\tTiempo (s)" >> "$archivo_salida"

# Recorrer las versiones
for version in "${!versiones[@]}"; do
  directorio="${versiones[$version]}"
  ejecuta_script="$directorio/ejecuta"

  # Configurar el número de threads según la versión
  case "$version" in
    "MS")
      export OMP_NUM_THREADS=1
      ;;
    "MPMS12t")
      export OMP_NUM_THREADS=12
      ;;
    "MPMS24t")
      export OMP_NUM_THREADS=24
      ;;
    "MPMS6t")
      export OMP_NUM_THREADS=6
      ;;
    "MPMS3t")
      export OMP_NUM_THREADS=3
      ;;
    *)
      export OMP_NUM_THREADS=12
      ;;
  esac

  # Verificar que el script ejecuta existe y es ejecutable
  if [[ -x "$ejecuta_script" ]]; then
    for N in "${valores_N[@]}"; do
      # Cambiar al directorio del script
      pushd "$directorio" > /dev/null

      # Ejecutar el script con el valor de N
      tiempo=$("$ejecuta_script" "$N" 2>/dev/null)

      # Volver al directorio original
      popd > /dev/null

      # Verificar si se obtuvo un tiempo válido
      if [[ $? -eq 0 && ! -z "$tiempo" ]]; then
        echo -e "${version}\t${N}\t${tiempo}" >> "$archivo_salida"
      else
        echo -e "${version}\t${N}\tError" >> "$archivo_salida"
      fi
    done
  else
    echo "El script 'ejecuta' de la versión '$version' no se encuentra o no es ejecutable." >> "$archivo_salida"
  fi

done

echo "Resultados guardados en $archivo_salida."
