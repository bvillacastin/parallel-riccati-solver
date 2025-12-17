% Script para generar gráficas de tiempo y speedup para cada función
maquina="Intel Xeon E5-2620 v3 2.4GHz, 12 Núcleos (DATSI)"; threads = [1, 24, 48, 12, 6];
%maquina="AMD Ryzen 7 PRO 8840HS 3.3GHz, 8 Núcleos"; threads = [1, 16, 32, 8, 4];
%maquina="AMD Ryzen 7 5800H 3.2GHz, 8 Núcleos"; threads = [1, 16, 32, 8, 4];
%maquina="Intel Core i5-10600KF 4.1GHz, 6 Núcleos"; threads = [1, 12, 24, 6, 3];

% Archivos de resultados
files = {
    'res_mos.txt', ...
    'res_mpmos.txt', ...
    'res_mpmos_dt.txt', ...
    'res_mpmos_ht.txt', ...
    'res_mpmos_qt.txt'
};


% Leer los datos de los archivos
data = cell(size(files));
for i = 1:length(files)
    data{i} = leer_resultados(files{i});
end

% Obtener las funciones a analizar
funciones = fieldnames(data{1}); % Basado en el archivo secuencial

% Crear gráficas por cada función
for f = 1:length(funciones)
    funcion = funciones{f};
    tiempos = zeros(length(files), 1);
    
    % Obtener el tiempo del último tamaño para cada configuración
    for i = 1:length(files)
        tiempos(i) = data{i}.(funcion).tiempos(end);
    end

    % Calcular speedup
    speedup = tiempos(1) ./ tiempos;

    % Ordenar valores de threads y speedup
    [threads_sorted, sort_idx] = sort(threads);
    tiempos_sorted = tiempos(sort_idx);
    speedup_sorted = speedup(sort_idx);

    % Crear la gráfica
    figure;
    yyaxis left;
    bar(threads_sorted, tiempos_sorted, 'FaceColor', [0.4 0.6 0.8], 'BarWidth', 0.5);
    ylabel('Segundos');
    xlabel('N° de hilos');
    title([maquina funcion]);

    yyaxis right;
    plot(threads_sorted, speedup_sorted, '-o', 'LineWidth', 2, 'Color', [1 0.4 0.4]);
    ylabel('Speedup');

    % Guardar la gráfica
    %saveas(gcf, [funcion, '_speedup.png']);
end

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
