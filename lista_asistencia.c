#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

typedef struct Alumno {
    int id;
    char nombre[100];
    int total_presentes;   
    struct Alumno *siguiente;
} Alumno;

/* Prototipos */
Alumno* crearAlumno(int id, const char *nombre);
void insertarAlumno(Alumno **inicio, int id, const char *nombre);
void pasarLista(Alumno *inicio, int dia);
void mostrarLista(Alumno *inicio);
void guardarAsistencia(Alumno *inicio, int dia);
void liberarLista(Alumno *inicio);


Alumno* crearAlumno(int id, const char *nombre) {
    Alumno *nuevo = malloc(sizeof(Alumno));
    if (nuevo == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    nuevo->id = id;
    strncpy(nuevo->nombre, nombre, sizeof(nuevo->nombre) - 1);
    nuevo->nombre[sizeof(nuevo->nombre) - 1] = '\0';
    nuevo->total_presentes = 0;
    nuevo->siguiente = NULL;
    return nuevo;
}


void insertarAlumno(Alumno **inicio, int id, const char *nombre) {
    Alumno *nuevo = crearAlumno(id, nombre);
    if (*inicio == NULL) {
        *inicio = nuevo;
    } else {
        Alumno *t = *inicio;
        while (t->siguiente != NULL) t = t->siguiente;
        t->siguiente = nuevo;
    }
}

void pasarLista(Alumno *inicio, int dia) {
    Alumno *t = inicio;
    char linea[16];
    printf("\nPasando lista para el día %d:\n", dia);

    while (t != NULL) {
        printf("¿El alumno %s (ID %d) esta presente? (s/n): ", t->nombre, t->id);
        if (fgets(linea, sizeof(linea), stdin) == NULL) break;
        char ch = linea[0];
        if (tolower((unsigned char)ch) == 's') {
            t->total_presentes++;
        }
        t = t->siguiente;
    }
}

/* Mostrar estado acumulado */
void mostrarLista(Alumno *inicio) {
    Alumno *t = inicio;
    puts("\n Reporte de asistencia");
    while (t != NULL) {
        printf("ID: %2d | %-20s | Presentes: %d\n",
               t->id, t->nombre, t->total_presentes);
        t = t->siguiente;
    }
}

void guardarAsistencia(Alumno *inicio, int dia) {
    FILE *f = fopen("asistencia.txt", "a"); // "a" = append
    if (!f) {
        perror("fopen");
        return;
    }

    time_t now = time(NULL);
    struct tm *fecha = localtime(&now);

    fprintf(f, "\n--- Día %d (%02d/%02d/%04d) ---\n",
            dia, fecha->tm_mday, fecha->tm_mon + 1, fecha->tm_year + 1900);

    Alumno *t = inicio;
    while (t != NULL) {
        fprintf(f, "ID: %d | %-20s | %s\n",
                t->id, t->nombre, (t->total_presentes >= dia ? "Presente" : "Ausente"));
        t = t->siguiente;
    }
    fclose(f);
}


void liberarLista(Alumno *inicio) {
    Alumno *t;
    while (inicio != NULL)
     {
        t = inicio;
        inicio = inicio->siguiente;
        free(t);
    }
}


int main(void) {
    Alumno *lista = NULL;
    char buffer[200];
    int n = 0;
    int dia = 0;
    int opcion;

    printf("¿Cuantos alumnos desea registrar? ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    n = atoi(buffer);

    for (int i = 1; i <= n; ++i) {
        printf("Ingrese el nombre del alumno %d: ", i);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) buffer[0] = '\0';
        buffer[strcspn(buffer, "\r\n")] = '\0';
        insertarAlumno(&lista, i, buffer);
    }

do {
        printf("\nMenu\n");
        printf("1. Pasar lista hoy\n");
        printf("2. Mostrar reporte acumulado\n");
        printf("3. Salir\n");
        printf("Seleccione una opcion: ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        opcion = atoi(buffer);

        switch (opcion) {
            case 1:
                dia++;
                pasarLista(lista, dia);
                guardarAsistencia(lista, dia);
                printf("✅ Pase de lista guardado en 'asistencia.txt'.\n");
                break;
            case 2:
                mostrarLista(lista);
                break;
            case 3:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opción no válida.\n");
        }
    } while (opcion != 3);
liberarLista(lista);
return 0;
}