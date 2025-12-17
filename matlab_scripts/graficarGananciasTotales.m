% Script para generar gráfica de tiempos y speedup desde "tiempos.txt"
%maquina = "Intel Core i5-10600KF 4.1GHz, 6 Nucleos";
maquina="Intel Xeon E5-2620 v3 2.4GHz, 12 Núcleos (DATSI)";
%maquina="AMD Ryzen 7 PRO 8840HS 3.3GHz, 8 Núcleos";
%maquina="AMD Ryzen 7 5800H 3.2GHz, 8 Núcleos";

% Leer el archivo "tiempos.txt"
fid = fopen('tiempos.txt', 'r');
if fid == -1
    error('No se pudo abrir el archivo tiempos.txt.');
end

% Leer encabezado y ajustar nombres de columnas
header = fgetl(fid); % Leer la primera línea como encabezado
header = strrep(header, 'Versión', 'Version'); % Reemplazar "Versión" por "Version"
header = strrep(header, 'Tiempo (s)', 'Tiempo'); % Reemplazar "Tiempo (s)" por "Tiempo"

% Leer los datos del archivo
data = textscan(fid, '%s %d %f', 'Delimiter', '\t'); % Leer columnas: Version, N, Tiempo
fclose(fid);

% Crear tabla para procesar los datos
tabla = table(data{1}, data{2}, data{3}, 'VariableNames', {'Version', 'N', 'Tiempo'});

% Extraer el número de hilos de cada versión
hilos = cellfun(@(x) sscanf(x, '%*[^0-9]%dt'), tabla.Version, 'UniformOutput', false);
hilos(cellfun(@isempty, hilos)) = {1}; % Si no hay número de hilos, asumir 1 hilo
hilos = cell2mat(hilos);

% Identificar las versiones únicas incluyendo los hilos
versiones_unicas = unique(tabla.Version);

% Seleccionar el tamaño máximo (N máximo) para las comparaciones
N_max = max(tabla.N);

% Filtrar datos para N máximo y calcular tiempos y speedup
tiempos = zeros(length(versiones_unicas), 1);
hilos_finales = zeros(length(versiones_unicas), 1);

for i = 1:length(versiones_unicas)
    filtro = strcmp(tabla.Version, versiones_unicas{i}) & tabla.N == N_max;
    tiempos_filtrados = tabla.Tiempo(filtro);
    hilos_filtrados = hilos(filtro);
    
    if isempty(tiempos_filtrados)
        warning('No se encontraron datos para la versión %s y N máximo.', versiones_unicas{i});
        tiempos(i) = NaN; % Manejar caso sin datos
        hilos_finales(i) = NaN;
        continue;
    end
    
    % Tomar el primer valor encontrado (o promedio si hay múltiples valores)
    tiempos(i) = mean(tiempos_filtrados);
    hilos_finales(i) = hilos_filtrados(1);
end

% Remover datos NaN (casos sin datos válidos)
valid_idx = ~isnan(tiempos) & ~isnan(hilos_finales);
tiempo_secuencial = tiempos(1); % No necesita filtrado
hilos_finales = hilos_finales(valid_idx);
tiempos = tiempos(valid_idx);

% Calcular speedup relativo al tiempo secuencial
speedup = tiempo_secuencial ./ tiempos;

% Ordenar datos por número de hilos
[~, sort_idx] = sort(hilos_finales);
hilos_finales = hilos_finales(sort_idx);
tiempos = tiempos(sort_idx);
speedup = speedup(sort_idx);

% Crear la gráfica
figure;
yyaxis left;
bar(hilos_finales, tiempos, 'FaceColor', [0.4 0.6 0.8], 'BarWidth', 0.5);
ylabel('Tiempo (segundos)');
xlabel('Numero de hilos');
title([maquina, ' - Rendimiento total']);

yyaxis right;
plot(hilos_finales, speedup, '-o', 'LineWidth', 2, 'Color', [1 0.4 0.4]);
ylabel('Speedup');

% Configuración adicional
grid on;

% Guardar la gráfica como archivo PNG
%saveas(gcf, 'rendimiento_speedup.png');