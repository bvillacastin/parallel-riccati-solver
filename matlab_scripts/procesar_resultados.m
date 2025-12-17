% Procesar Resultados de BLAS y Variantes
%maquina="Intel Xeon E5-2620 v3 2.4GHz, 12 Núcleos (DATSI)";
%maquina="AMD Ryzen 7 PRO 8840HS 3.3GHz, 8 Núcleos";
%maquina="AMD Ryzen 7 5800H 3.2GHz, 8 Núcleos";
maquina="Intel Core i5-10600KF 4.1GHz, 6 Núcleos";
% Archivos de entrada
archivos = {
    %'res_blas.txt', 'BLAS';
    'res_mos.txt', 'MS';
    'res_mpmos.txt', 'MPMS12t';
    'res_mpmos_dt.txt', 'MPMS24t';
    'res_mpmos_ht.txt', 'MPMS6t';
    'res_mpmos_qt.txt', 'MPMS3t'
    
};

% Funciones a analizar
funciones = {'solvels', 'mmul', 'mcpy', 'mset', 'mnorm', 'mmulc', 'msub',};

% Crear una estructura para almacenar los datos
datos = struct();

% Leer datos de cada archivo
for i = 1:size(archivos, 1)
    archivo = archivos{i, 1};
    etiqueta = archivos{i, 2};
    
    % Abrir el archivo y leer
    fid = fopen(archivo, 'r');
    contenido = textscan(fid, '%s', 'Delimiter', '\n');
    fclose(fid);
    
    contenido = contenido{1};
    
    for j = 1:numel(funciones)
        funcion = funciones{j};
        idx = find(contains(contenido, ['Función: ' funcion]));
        if ~isempty(idx)
            % Extraer los valores de N y los tiempos
            inicio = idx + 2;
            fin = inicio + find(cellfun(@isempty, contenido(inicio:end)) | ...
                                contains(contenido(inicio:end), 'Función:'), 1, 'first') - 2;
            
            % Manejar el caso en que sea la última función
            if isempty(fin)
                fin = numel(contenido);
            end
            
            datos_temp = contenido(inicio:fin);
            
            % Convertir las líneas en una matriz
            matriz = [];
            for k = 1:numel(datos_temp)
                linea = datos_temp{k};
                
                % Ignorar líneas de separación
                if contains(linea, '--------------------------')
                    continue;
                end
                
                % Procesar líneas válidas
                valores = sscanf(linea, '%f %f');
                if numel(valores) == 2
                    matriz = [matriz; valores'];
                else
                    fprintf('Línea malformada en %s para %s: %s\n', etiqueta, funcion, linea);
                end
            end
            
            % Guardar en la estructura
            if ~isfield(datos, funcion)
                datos.(funcion) = struct();
            end
            datos.(funcion).(etiqueta) = matriz;
        end
    end
end

% Generar gráficas para cada función
for i = 1:numel(funciones)
    funcion = funciones{i};
    figure('Name', funcion);
    hold on;
    etiquetas = fieldnames(datos.(funcion));
    
    for j = 1:numel(etiquetas)
        etiqueta = etiquetas{j};
        valores = datos.(funcion).(etiqueta);
        if size(valores, 2) == 2
            plot(valores(:, 1), valores(:, 2), '-o', 'DisplayName', etiqueta);
        else
            warning('Los datos para %s en %s no tienen el formato esperado.', etiqueta, funcion);
        end
    end
    
    title([maquina funcion]);
    xlabel('Tamaño N');
    ylabel('Tiempo (segundos)');
    legend('Location', 'northwest');
    grid on;
    hold off;
end
