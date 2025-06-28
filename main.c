#include <stdio.h>      // Libreria para entrada y salida (printf, scanf, etc.)
#include <string.h>     // Libreria para manejo de strings (strcpy, strcmp, etc.)
#include <stdlib.h>     // Libreria para funciones generales como atoi()
#include <time.h>       // Libreria para obtener la fecha y hora actual

#define MAX_TAREAS 100  // Define el maximo de tareas que se pueden guardar

// Estructura que define como se guarda cada tarea
typedef struct {
    char titulo[100];             // Titulo de la tarea
    char descripcion[500];       // Descripcion detallada
    char estado[20];             // Estado actual de la tarea (Pendiente, En curso, etc.)
    char fecha_creacion[11];     // Fecha de creacion (formato dd/mm/yyyy)
    char ultima_edicion[11];     // Fecha de la ultima edicion
    char vencimiento[11];        // Fecha de vencimiento
    int dificultad;              // Dificultad de la tarea (1, 2 o 3)
} Tarea;

Tarea tareas[MAX_TAREAS];       // Arreglo donde se almacenan las tareas
int cantidad_tareas = 0;        // Lleva la cuenta de tareas agregadas

// Funcion para obtener la fecha actual y guardarla como string en formato dd/mm/yyyy
void obtener_fecha_actual(char *destino) {
    time_t t = time(NULL);                   // Obtiene el tiempo actual
    struct tm tm = *localtime(&t);          // Lo convierte a formato local (dia, mes, anio, etc.)
    sprintf(destino, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);  // Lo guarda en formato string
}

// Funcion usada para ordenar tareas alfabeticamente por titulo
int comparar_titulo(const void *a, const void *b) {
    const Tarea *t1 = (const Tarea *)a;
    const Tarea *t2 = (const Tarea *)b;
    return strcmp(t1->titulo, t2->titulo);  // Compara los titulos
}

// Muestra el nivel de dificultad segun el numero (1, 2 o 3)
void mostrar_dificultad(int nivel) {
    switch (nivel) {
        case 1: printf("Dificultad: Facil\n"); break;
        case 2: printf("Dificultad: Medio\n"); break;
        case 3: printf("Dificultad: Dificil\n"); break;
        default: printf("Dificultad: ?\n"); // Por si no es ninguno valido
    }
}

// Permite al usuario ingresar una nueva tarea
void agregar_tarea() {
    Tarea nueva;

    // Ingreso de datos
    printf("Titulo: ");
    fgets(nueva.titulo, sizeof(nueva.titulo), stdin);
    nueva.titulo[strcspn(nueva.titulo, "\n")] = 0; // Elimina salto de linea

    printf("Descripcion: ");
    fgets(nueva.descripcion, sizeof(nueva.descripcion), stdin);
    nueva.descripcion[strcspn(nueva.descripcion, "\n")] = 0;

    strcpy(nueva.estado, "Pendiente"); // Estado inicial por defecto
    obtener_fecha_actual(nueva.fecha_creacion); // Fecha actual
    strcpy(nueva.ultima_edicion, nueva.fecha_creacion); // Igual a creacion al inicio

    printf("Fecha de vencimiento (dd/mm/yyyy) [opcional]: ");
    fgets(nueva.vencimiento, sizeof(nueva.vencimiento), stdin);
    nueva.vencimiento[strcspn(nueva.vencimiento, "\n")] = 0;

    printf("Dificultad (1: facil, 2: medio, 3: dificil): ");
    scanf("%d", &nueva.dificultad);
    getchar(); // Limpia buffer
    if (nueva.dificultad < 1 || nueva.dificultad > 3) nueva.dificultad = 1; // Valor por defecto

    // Guarda la tarea en el arreglo
    tareas[cantidad_tareas++] = nueva;
    printf("\x1b[32m¡Tarea agregada!\x1b[0m\n");
}

// Muestra solo los titulos y estados de todas las tareas
void mostrar_todas_las_tareas() {
    for (int i = 0; i < cantidad_tareas; i++) {
        printf("%d. %s [%s]\n", i + 1, tareas[i].titulo, tareas[i].estado);
    }
}

// Permite cambiar el estado de una tarea existente
void cambiar_estado_tarea() {
    int indice;
    mostrar_todas_las_tareas();

    printf("\nIngrese el numero de la tarea para cambiar su estado: ");
    scanf("%d", &indice);
    getchar();

    if (indice < 1 || indice > cantidad_tareas) {
        printf("Tarea no valida.\n");
        return;
    }

    printf("Nuevo estado (Pendiente / En curso / Terminada): ");
    fgets(tareas[indice - 1].estado, sizeof(tareas[indice - 1].estado), stdin);
    tareas[indice - 1].estado[strcspn(tareas[indice - 1].estado, "\n")] = 0;

    obtener_fecha_actual(tareas[indice - 1].ultima_edicion); // Actualiza fecha
    printf("Estado actualizado con exito.\n");
}

// Muestra las tareas filtradas por estado o todas
void ver_mis_tareas() {
    int opcion;
    do {
        printf("\n--- Ver mis tareas ---\n");
        printf("1. Todas\n2. Pendientes\n3. En curso\n4. Terminadas\n5. Volver\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        getchar();

        if (opcion == 5) break;

        Tarea filtradas[MAX_TAREAS];
        int cantidad_filtradas = 0;

        // Filtra segun el estado
        for (int i = 0; i < cantidad_tareas; i++) {
            if (opcion == 1 ||
               (opcion == 2 && strcmp(tareas[i].estado, "Pendiente") == 0) ||
               (opcion == 3 && strcmp(tareas[i].estado, "En curso") == 0) ||
               (opcion == 4 && strcmp(tareas[i].estado, "Terminada") == 0)) {
                filtradas[cantidad_filtradas++] = tareas[i];
            }
        }

        // Muestra las tareas filtradas
        if (cantidad_filtradas == 0) {
            printf("No se encontraron tareas con ese criterio.\n");
        } else {
            qsort(filtradas, cantidad_filtradas, sizeof(Tarea), comparar_titulo); // Ordena
            for (int i = 0; i < cantidad_filtradas; i++) {
                printf("\nTarea %d: %s\n", i + 1, filtradas[i].titulo);
                printf("Estado: %s\nDescripcion: %s\nCreacion: %s\n", filtradas[i].estado, filtradas[i].descripcion, filtradas[i].fecha_creacion);
                printf("Ultima edicion: %s\nVencimiento: %s\n", filtradas[i].ultima_edicion, filtradas[i].vencimiento[0] ? filtradas[i].vencimiento : "Sin datos");
                mostrar_dificultad(filtradas[i].dificultad);
            }
        }
    } while (opcion != 5);
}

// Busca una tarea por palabra clave en el titulo
void buscar_tarea() {
    char clave[100];
    int encontrada = 0;

    printf("Titulo a buscar: ");
    fgets(clave, sizeof(clave), stdin);
    clave[strcspn(clave, "\n")] = 0;

    for (int i = 0; i < cantidad_tareas; i++) {
        if (strstr(tareas[i].titulo, clave)) {
            printf("\nTarea: %s\nEstado: %s\nDescripcion: %s\nCreacion: %s\n", tareas[i].titulo, tareas[i].estado, tareas[i].descripcion, tareas[i].fecha_creacion);
            printf("Ultima edicion: %s\nVencimiento: %s\n", tareas[i].ultima_edicion, tareas[i].vencimiento[0] ? tareas[i].vencimiento : "Sin datos");
            mostrar_dificultad(tareas[i].dificultad);
            encontrada = 1;
        }
    }

    if (!encontrada) printf("No se encontraron tareas con ese titulo.\n");
}

// Guarda las tareas en un archivo binario para no perderlas al cerrar
void guardar_tareas() {
    FILE *f = fopen("tareas.dat", "wb");
    if (!f) {
        printf("Error al guardar.\n");
        return;
    }
    fwrite(tareas, sizeof(Tarea), cantidad_tareas, f);
    fclose(f);
    printf("Tareas guardadas.\n");
}

// Carga las tareas desde el archivo si existe
void cargar_tareas() {
    FILE *f = fopen("tareas.dat", "rb");
    if (!f) return;
    cantidad_tareas = fread(tareas, sizeof(Tarea), MAX_TAREAS, f);
    fclose(f);
}

// Permite editar todos los campos de una tarea existente
void editar_tarea_completa() {
    int indice;
    mostrar_todas_las_tareas();
    printf("\nIngrese el numero de la tarea que desea editar: ");
    scanf("%d", &indice);
    getchar();

    if (indice < 1 || indice > cantidad_tareas) {
        printf("Tarea no valida.\n");
        return;
    }

    Tarea *t = &tareas[indice - 1];
    char entrada[500];

    printf("Editar titulo (actual: %s): ", t->titulo);
    fgets(entrada, sizeof(entrada), stdin);
    if (entrada[0] != '\n') {
        entrada[strcspn(entrada, "\n")] = 0;
        strcpy(t->titulo, entrada);
    }

    printf("Editar descripcion (actual: %s): ", t->descripcion);
    fgets(entrada, sizeof(entrada), stdin);
    if (entrada[0] != '\n') {
        entrada[strcspn(entrada, "\n")] = 0;
        strcpy(t->descripcion, entrada);
    }

    printf("Editar estado (Pendiente / En curso / Terminada / Cancelada): ");
    fgets(entrada, sizeof(entrada), stdin);
    if (entrada[0] != '\n') {
        entrada[strcspn(entrada, "\n")] = 0;
        strcpy(t->estado, entrada);
    }

    printf("Editar vencimiento (actual: %s): ", t->vencimiento);
    fgets(entrada, sizeof(entrada), stdin);
    if (entrada[0] != '\n') {
        entrada[strcspn(entrada, "\n")] = 0;
        strcpy(t->vencimiento, entrada);
    }

    printf("Editar dificultad (1: facil, 2: medio, 3: dificil): ");
    fgets(entrada, sizeof(entrada), stdin);
    int diff = atoi(entrada);
    if (diff >= 1 && diff <= 3) {
        t->dificultad = diff;
    }

    obtener_fecha_actual(t->ultima_edicion);
    printf("Tarea editada con exito.\n");
}

// Elimina una tarea del arreglo
void eliminar_tarea() {
    int indice;
    mostrar_todas_las_tareas();
    printf("\nIngrese el numero de la tarea que desea eliminar: ");
    scanf("%d", &indice);
    getchar();

    if (indice < 1 || indice > cantidad_tareas) {
        printf("Numero invalido.\n");
        return;
    }

    // Desplaza las tareas una posicion para eliminar
    for (int i = indice - 1; i < cantidad_tareas - 1; i++) {
        tareas[i] = tareas[i + 1];
    }
    cantidad_tareas--;
    printf("Tarea eliminada.\n");
}

// Muestra el menu principal de opciones
void mostrar_menu() {
    printf("\n--- Menu Principal ---\n");
    printf("1. Ver mis tareas\n");
    printf("2. Buscar una tarea\n");
    printf("3. Agregar una tarea\n");
    printf("4. Cambiar estado de tarea\n");
    printf("5. Editar una tarea\n");
    printf("6. Eliminar una tarea\n");
    printf("7. Salir\n");
    printf("Seleccione una opcion: ");
}

// Funcion principal donde se ejecuta el programa
int main() {
    int opcion;
    cargar_tareas(); // Carga las tareas guardadas anteriormente

    while (1) {
        mostrar_menu();
        scanf("%d", &opcion);
        getchar();

        switch (opcion) {
            case 1: ver_mis_tareas(); break;
            case 2: buscar_tarea(); break;
            case 3: agregar_tarea(); break;
            case 4: cambiar_estado_tarea(); break;
            case 5: editar_tarea_completa(); break;
            case 6: eliminar_tarea(); break;
            case 7: guardar_tareas(); printf("\x1b[32m\n\n\n¡Hasta luego!\x1b[0m\n"); return 0;
            default: printf("Opcion invalida.\n");
        }

        printf("\nPresione ENTER para continuar...");
        getchar(); // Espera que el usuario presione Enter
        system("clear"); // Limpia la consola (en Linux/macOS)
    }
}
