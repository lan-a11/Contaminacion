#include "funciones.h"

// Implementación de funciones del sistema de gestión y predicción de contaminación del aire

// 1. Cargar datos históricos y actuales desde archivo o manualmente
void cargarDatos(ZonaUrbana *zonas, int numZonas) {
    FILE *archivo = fopen("datos_zonas.csv", "r");
    int i, j;
    if (archivo) {
        // Formato esperado: nombre,contaminantes(4),temp,viento,humedad (por cada día)
        for (i = 0; i < numZonas; i++) {
            fscanf(archivo, "%49[^,],", zonas[i].nombre);
            for (j = 0; j < MAX_DIAS; j++) {
                fscanf(archivo, "%f,%f,%f,%f,%f,%f,%f,", &zonas[i].contaminantesHistoricos[j][0], &zonas[i].contaminantesHistoricos[j][1], &zonas[i].contaminantesHistoricos[j][2], &zonas[i].contaminantesHistoricos[j][3], &zonas[i].temperatura[j], &zonas[i].viento[j], &zonas[i].humedad[j]);
            }
        }
        fclose(archivo);
        printf("Datos cargados desde archivo.\n");
    } else {
        // Si no hay archivo, inicializar datos de ejemplo
        for (i = 0; i < numZonas; i++) {
            sprintf(zonas[i].nombre, "Zona %d", i+1);
            for (j = 0; j < MAX_DIAS; j++) {
                zonas[i].contaminantesHistoricos[j][IDX_CO2] = 800 + i*10 + j;
                zonas[i].contaminantesHistoricos[j][IDX_SO2] = 15 + i + j%3;
                zonas[i].contaminantesHistoricos[j][IDX_NO2] = 30 + i*2 + j%4;
                zonas[i].contaminantesHistoricos[j][IDX_PM25] = 20 + i + j%2;
                zonas[i].temperatura[j] = 22 + i + j%5;
                zonas[i].viento[j] = 5 + i + j%2;
                zonas[i].humedad[j] = 60 + i*2 + j%10;
            }
        }
        printf("Datos de ejemplo inicializados.\n");
    }
    // Cargar los valores actuales (último día)
    for (i = 0; i < numZonas; i++) {
        for (j = 0; j < NUM_CONTAMINANTES; j++) {
            zonas[i].contaminantesActuales[j] = zonas[i].contaminantesHistoricos[MAX_DIAS-1][j];
        }
    }
}

// 2. Calcular niveles actuales de contaminación
void calcularNivelesActuales(ZonaUrbana *zonas, int numZonas) {
    printf("\n--- Niveles actuales de contaminación por zona ---\n");
    for (int i = 0; i < numZonas; i++) {
        printf("Zona: %s\n", zonas[i].nombre);
        printf("  CO2:   %.2f ppm %s\n", zonas[i].contaminantesActuales[IDX_CO2], zonas[i].contaminantesActuales[IDX_CO2] > LIMITE_CO2 ? "(ALTO)" : "");
        printf("  SO2:   %.2f ppb %s\n", zonas[i].contaminantesActuales[IDX_SO2], zonas[i].contaminantesActuales[IDX_SO2] > LIMITE_SO2 ? "(ALTO)" : "");
        printf("  NO2:   %.2f ppb %s\n", zonas[i].contaminantesActuales[IDX_NO2], zonas[i].contaminantesActuales[IDX_NO2] > LIMITE_NO2 ? "(ALTO)" : "");
        printf("  PM2.5: %.2f ug/m3 %s\n", zonas[i].contaminantesActuales[IDX_PM25], zonas[i].contaminantesActuales[IDX_PM25] > LIMITE_PM25 ? "(ALTO)" : "");
        printf("  Temperatura: %.1f°C\n", zonas[i].temperatura[MAX_DIAS-1]);
        printf("  Viento:      %.1f km/h\n", zonas[i].viento[MAX_DIAS-1]);
        printf("  Humedad:     %.1f%%\n", zonas[i].humedad[MAX_DIAS-1]);
        printf("---------------------------------------------\n");
    }
}

// 3. Realizar predicciones futuras usando promedio ponderado
void predecirNiveles(ZonaUrbana *zonas, int numZonas) {
    printf("\n--- Predicción de niveles futuros (promedio últimos 5 días) ---\n");
    for (int i = 0; i < numZonas; i++) {
        printf("Zona: %s\n", zonas[i].nombre);
        for (int c = 0; c < NUM_CONTAMINANTES; c++) {
            float suma = 0, peso = 0;
            for (int d = MAX_DIAS-5, p = 5; d < MAX_DIAS; d++, p--) {
                suma += zonas[i].contaminantesHistoricos[d][c] * p;
                peso += p;
            }
            zonas[i].prediccion[c] = suma / peso;
        }
        printf("  Predicción CO2:   %.2f ppm\n", zonas[i].prediccion[IDX_CO2]);
        printf("  Predicción SO2:   %.2f ppb\n", zonas[i].prediccion[IDX_SO2]);
        printf("  Predicción NO2:   %.2f ppb\n", zonas[i].prediccion[IDX_NO2]);
        printf("  Predicción PM2.5: %.2f ug/m3\n", zonas[i].prediccion[IDX_PM25]);
        printf("---------------------------------------------\n");
    }
}

// 4. Emitir alertas si la predicción excede los límites
void emitirAlertas(ZonaUrbana *zonas, int numZonas) {
    FILE *falerta = fopen("alertas.txt", "w");
    printf("\n--- Alertas por predicción de contaminación ---\n");
    for (int i = 0; i < numZonas; i++) {
        int hayAlerta = 0;
        printf("Zona: %s\n", zonas[i].nombre);
        for (int c = 0; c < NUM_CONTAMINANTES; c++) {
            zonas[i].alerta[c] = 0;
        }
        if (zonas[i].prediccion[IDX_CO2] > LIMITE_CO2) {
            printf("  ALERTA: CO2 excede el límite OMS!\n");
            zonas[i].alerta[IDX_CO2] = 1; hayAlerta = 1;
            if (falerta) fprintf(falerta, "Zona %s: ALERTA CO2\n", zonas[i].nombre);
        }
        if (zonas[i].prediccion[IDX_SO2] > LIMITE_SO2) {
            printf("  ALERTA: SO2 excede el límite OMS!\n");
            zonas[i].alerta[IDX_SO2] = 1; hayAlerta = 1;
            if (falerta) fprintf(falerta, "Zona %s: ALERTA SO2\n", zonas[i].nombre);
        }
        if (zonas[i].prediccion[IDX_NO2] > LIMITE_NO2) {
            printf("  ALERTA: NO2 excede el límite OMS!\n");
            zonas[i].alerta[IDX_NO2] = 1; hayAlerta = 1;
            if (falerta) fprintf(falerta, "Zona %s: ALERTA NO2\n", zonas[i].nombre);
        }
        if (zonas[i].prediccion[IDX_PM25] > LIMITE_PM25) {
            printf("  ALERTA: PM2.5 excede el límite OMS!\n");
            zonas[i].alerta[IDX_PM25] = 1; hayAlerta = 1;
            if (falerta) fprintf(falerta, "Zona %s: ALERTA PM2.5\n", zonas[i].nombre);
        }
        if (!hayAlerta) printf("  Sin alertas.\n");
        printf("---------------------------------------------\n");
    }
    if (falerta) fclose(falerta);
}

// 5. Calcular promedios históricos de los últimos 30 días
void calcularPromediosHistoricos(ZonaUrbana *zonas, int numZonas) {
    printf("\n--- Promedios históricos (últimos 30 días) ---\n");
    for (int i = 0; i < numZonas; i++) {
        printf("Zona: %s\n", zonas[i].nombre);
        for (int c = 0; c < NUM_CONTAMINANTES; c++) {
            float suma = 0;
            for (int d = 0; d < MAX_DIAS; d++)
                suma += zonas[i].contaminantesHistoricos[d][c];
            float promedio = suma / MAX_DIAS;
            printf("  Promedio %s: %.2f %s\n",
                c==IDX_CO2 ? "CO2" : c==IDX_SO2 ? "SO2" : c==IDX_NO2 ? "NO2" : "PM2.5",
                promedio,
                c==IDX_PM25 ? "ug/m3" : c==IDX_CO2 ? "ppm" : "ppb");
            if ((c==IDX_CO2 && promedio > LIMITE_CO2) ||
                (c==IDX_SO2 && promedio > LIMITE_SO2) ||
                (c==IDX_NO2 && promedio > LIMITE_NO2) ||
                (c==IDX_PM25 && promedio > LIMITE_PM25))
                printf("    (ALTO)\n");
        }
        printf("---------------------------------------------\n");
    }
}

// 6. Generar recomendaciones automáticas según riesgo
void generarRecomendaciones(ZonaUrbana *zonas, int numZonas) {
    printf("\n--- Recomendaciones automáticas por zona ---\n");
    for (int i = 0; i < numZonas; i++) {
        printf("Zona: %s\n", zonas[i].nombre);
        int riesgo = 0;
        for (int c = 0; c < NUM_CONTAMINANTES; c++)
            if (zonas[i].alerta[c]) riesgo = 1;
        if (riesgo) {
            printf("  - Reducir tráfico vehicular\n");
            printf("  - Evitar actividades al aire libre\n");
            printf("  - Usar mascarilla si es necesario\n");
        } else {
            printf("  - Niveles dentro de lo normal.\n");
        }
        printf("---------------------------------------------\n");
    }
}

// 7. Guardar reportes en archivos .txt o .csv
void guardarReporte(ZonaUrbana *zonas, int numZonas) {
    FILE *archivo = fopen("reporte_zonas.csv", "w");
    int i, j;
    if (!archivo) {
        printf("No se pudo crear el archivo de reporte.\n");
        return;
    }
    fprintf(archivo, "Zona,CO2 (ppm),SO2 (ppb),NO2 (ppb),PM2.5 (ug/m3),Temp (C),Viento (km/h),Humedad (%%),Pred_CO2,Pred_SO2,Pred_NO2,Pred_PM2.5,Alerta_CO2,Alerta_SO2,Alerta_NO2,Alerta_PM2.5\n");
    printf("\n================= REPORTE DE ZONAS =================\n");
    printf("%-10s %-10s %-10s %-10s %-12s %-8s %-10s %-10s\n", "Zona", "CO2", "SO2", "NO2", "PM2.5", "Temp", "Viento", "Humedad");
    printf("--------------------------------------------------------------------------------\n");
    for (i = 0; i < numZonas; i++) {
        fprintf(archivo, "%s", zonas[i].nombre);
        for (j = 0; j < NUM_CONTAMINANTES; j++)
            fprintf(archivo, ",%.2f", zonas[i].contaminantesActuales[j]);
        fprintf(archivo, ",%.2f,%.2f,%.2f", zonas[i].temperatura[MAX_DIAS-1], zonas[i].viento[MAX_DIAS-1], zonas[i].humedad[MAX_DIAS-1]);
        for (j = 0; j < NUM_CONTAMINANTES; j++)
            fprintf(archivo, ",%.2f", zonas[i].prediccion[j]);
        for (j = 0; j < NUM_CONTAMINANTES; j++)
            fprintf(archivo, ",%d", zonas[i].alerta[j]);
        fprintf(archivo, "\n");
        // Presentación en consola
        printf("%-10s %-10.2f %-10.2f %-10.2f %-12.2f %-8.1f %-10.1f %-10.1f\n",
            zonas[i].nombre,
            zonas[i].contaminantesActuales[IDX_CO2],
            zonas[i].contaminantesActuales[IDX_SO2],
            zonas[i].contaminantesActuales[IDX_NO2],
            zonas[i].contaminantesActuales[IDX_PM25],
            zonas[i].temperatura[MAX_DIAS-1],
            zonas[i].viento[MAX_DIAS-1],
            zonas[i].humedad[MAX_DIAS-1]
        );
        printf("  Predicción: CO2=%.2f, SO2=%.2f, NO2=%.2f, PM2.5=%.2f\n", zonas[i].prediccion[IDX_CO2], zonas[i].prediccion[IDX_SO2], zonas[i].prediccion[IDX_NO2], zonas[i].prediccion[IDX_PM25]);
        printf("  Alertas: CO2=%s, SO2=%s, NO2=%s, PM2.5=%s\n",
            zonas[i].alerta[IDX_CO2] ? "SI" : "NO",
            zonas[i].alerta[IDX_SO2] ? "SI" : "NO",
            zonas[i].alerta[IDX_NO2] ? "SI" : "NO",
            zonas[i].alerta[IDX_PM25] ? "SI" : "NO");
        printf("--------------------------------------------------------------------------------\n");
    }
    fclose(archivo);
    printf("\nReporte guardado en reporte_zonas.csv\n");
}
