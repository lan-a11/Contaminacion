#include "funciones.h"

// Menú principal y flujo general del sistema
int main() {
    ZonaUrbana zonas[MAX_ZONAS];
    int opcion;
    int salir = 0;

    // Cargar datos iniciales (puede ser desde archivo o manual)
    cargarDatos(zonas, MAX_ZONAS);

    while (!salir) {
        printf("\n==== SISTEMA DE GESTIÓN Y PREDICCIÓN DE CONTAMINACIÓN DEL AIRE ====");
        printf("\n1. Calcular niveles actuales");
        printf("\n2. Predecir niveles futuros");
        printf("\n3. Emitir alertas");
        printf("\n4. Calcular promedios históricos");
        printf("\n5. Generar recomendaciones");
        printf("\n6. Guardar reporte");
        printf("\n0. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
                calcularNivelesActuales(zonas, MAX_ZONAS);
                break;
            case 2:
                predecirNiveles(zonas, MAX_ZONAS);
                break;
            case 3:
                emitirAlertas(zonas, MAX_ZONAS);
                break;
            case 4:
                calcularPromediosHistoricos(zonas, MAX_ZONAS);
                break;
            case 5:
                generarRecomendaciones(zonas, MAX_ZONAS);
                break;
            case 6:
                guardarReporte(zonas, MAX_ZONAS);
                break;
            case 0:
                salir = 1;
                break;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
        }
    }
    printf("\nGracias por usar el sistema.\n");
    return 0;
}
