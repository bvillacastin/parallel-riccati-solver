% Script para graficar tiempos de ejecucion desde "tiempos.txt"
%maquina="Intel Xeon E5-2620 v3 2.4GHz, 12 Núcleos (DATSI)";
%maquina="AMD Ryzen 7 PRO 8840HS 3.3GHz, 8 Núcleos";
%maquina="AMD Ryzen 7 5800H 3.2GHz, 8 Núcleos";
maquina="Intel Core i5-10600KF 4.1GHz, 6 Núcleos"
% Leer el archivo linea por linea
fid = fopen('tiempos.txt', 'r');
if fid == -1
    error('No se pudo abrir el archivo tiempos.txt.');
end

% Leer encabezado
header = fgetl(fid); % Leer la primera linea como encabezado
header = strrep(header, 'Versión', 'Version'); % Sustituir caracteres especiales
header = strrep(header, 'Tiempo (s)', 'Tiempo'); % Eliminar parentesis y caracteres especiales

% Leer datos del archivo
data = textscan(fid, '%s %d %f', 'Delimiter', '\t'); % Leer columnas: Version, N, Tiempo
fclose(fid);

% Crear tabla para procesar los datos
tabla = table(data{1}, data{2}, data{3}, 'VariableNames', {'Version', 'N', 'Tiempo'});

% Identificar las versiones unicas
versiones_unicas = unique(tabla.Version);

% Crear figura para graficar
figure;
hold on;

% Colores y marcadores para cada version
colores = {'-o', '-s', '-^', '--d', ':x'}; % Ajusta estilos segun las versiones

% Iterar sobre cada version y graficar sus datos correspondientes
for i = 1:length(versiones_unicas)
    version_actual = versiones_unicas{i};
    
    % Filtrar datos para la version actual
    filtro = strcmp(tabla.Version, version_actual);
    tamanos_filtrados = tabla.N(filtro);
    tiempos_filtrados = tabla.Tiempo(filtro);
    
    % Graficar
    plot(tamanos_filtrados, tiempos_filtrados, colores{i}, 'DisplayName', version_actual);
end

% Configuracion de la grafica
title([maquina 'Comparacion de Tiempos de Ejecucion']); % Sin tildes para evitar errores
xlabel('Tamano N'); % Sin tildes para evitar errores
ylabel('Tiempo (segundos)');
legend('Location', 'northwest');
grid on;

% Guardar la grafica como archivo PNG
%saveas(gcf, 'tiempos_comparacion.png');
