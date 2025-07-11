#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constantes
#define MAX_ZONAS 5
#define MAX_DIAS 30
#define MAX_NOMBRE 50
#define NUM_CONTAMINANTES 4

// Límites OMS (ejemplo, puedes ajustar los valores)
#define LIMITE_CO2 1000
#define LIMITE_SO2 20
#define LIMITE_NO2 40
#define LIMITE_PM25 25

// Índices de contaminantes
#define IDX_CO2 0
#define IDX_SO2 1
#define IDX_NO2 2
#define IDX_PM25 3

// Estructura para una zona urbana
typedef struct {
    char nombre[MAX_NOMBRE];
    float contaminantesHistoricos[MAX_DIAS][NUM_CONTAMINANTES]; // [día][contaminante]
    float contaminantesActuales[NUM_CONTAMINANTES];
    float temperatura[MAX_DIAS];
    float viento[MAX_DIAS];
    float humedad[MAX_DIAS];
    float prediccion[NUM_CONTAMINANTES];
    int alerta[NUM_CONTAMINANTES];
} ZonaUrbana;

// Prototipos de funciones
void cargarDatos(ZonaUrbana *zonas, int numZonas);
void calcularNivelesActuales(ZonaUrbana *zonas, int numZonas);
void predecirNiveles(ZonaUrbana *zonas, int numZonas);
void emitirAlertas(ZonaUrbana *zonas, int numZonas);
void calcularPromediosHistoricos(ZonaUrbana *zonas, int numZonas);
void generarRecomendaciones(ZonaUrbana *zonas, int numZonas);
void guardarReporte(ZonaUrbana *zonas, int numZonas);

#endif // FUNCIONES_H
