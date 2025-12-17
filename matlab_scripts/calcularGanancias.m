% Script para analizar resultados y calcular speedups y eficiencias

% Archivos de resultados
files = {
    'res_mos.txt',
    'res_mpmos_qt.txt',
    'res_mpmos_ht.txt',
    'res_mpmos.txt',
    'res_mpmos_dt.txt'
};

% Número de hilos utilizados en cada configuración
%threads = [1, 4, 8, 16, 32];
threads = [1, 6, 12, 24, 48];
%threads = [1, 3, 6, 12, 24];

% Archivo de salida
output_file = 'ganancias.txt';
fid_out = fopen(output_file, 'w');
if fid_out == -1
    error('No se pudo abrir el archivo de salida: %s', output_file);
end

% Leer los datos de los archivos
data = cell(size(files));
for i = 1:length(files)
    data{i} = leer_resultados(files{i});
end

% Obtener las funciones a analizar
funciones = fieldnames(data{1}); % Basado en el archivo secuencial

% Crear tablas de resultados para cada función
for f = 1:length(funciones)
    funcion = funciones{f};
    
    % Escribir encabezado de la función en el archivo de salida
    fprintf(fid_out, 'Resultados para la función: %s\n', funcion);
    fprintf(fid_out, '%-10s %-15s %-15s\n', 'Hilos', 'Speedup', 'Eficiencia');
    
    % Obtener el tiempo base (último tamaño del archivo secuencial)
    tiempo_base = data{1}.(funcion).tiempos(end);
    
    % Comparar con cada archivo paralelo
    for i = 2:length(files)
        tiempos_paralelos = data{i}.(funcion).tiempos;
        tiempo_paralelo = tiempos_paralelos(end); % Último tamaño
        
        % Calcular speedup y eficiencia
        speedup = tiempo_base / tiempo_paralelo;
        eficiencia = speedup / threads(i);
        
        % Escribir resultados en el archivo
        fprintf(fid_out, '%-10d %-15.2f %-15.2f\n', threads(i), speedup, eficiencia);
    end
    
    % Espaciado entre funciones
    fprintf(fid_out, '\n');
end

% Cerrar el archivo de salida
fclose(fid_out);

fprintf('Resultados almacenados en %s\n', output_file);

% Función para leer resultados desde un archivo
function resultados = leer_resultados(filename)
    fid = fopen(filename, 'r');
    if fid == -1
        error('No se pudo abrir el archivo: %s', filename);
    end
    
    % Leer líneas del archivo
    lines = textscan(fid, '%s', 'Delimiter', '\n');
    lines = lines{1};
    fclose(fid);
    
    % Inicializar estructura de resultados
    resultados = struct();
    
    % Analizar líneas
    current_function = '';
    for i = 1:length(lines)
        line = strtrim(lines{i});
        
        % Detectar nueva función
        if startsWith(line, 'Función:')
            current_function = strtrim(strrep(line, 'Función:', ''));
            resultados.(current_function) = struct('N', [], 'tiempos', []);
        elseif ~isempty(current_function) && ~isempty(line) && ~startsWith(line, '-')
            % Extraer datos numéricos
            data = sscanf(line, '%d %f');
            if ~isempty(data)
                resultados.(current_function).N(end+1) = data(1);
                resultados.(current_function).tiempos(end+1) = data(2);
            end
        end
    end
end