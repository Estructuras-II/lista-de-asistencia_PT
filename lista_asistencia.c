#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_DIAS 30   // límite de días (se puede cambiarlo)

typedef struct Alumno {
    int id;
    char nombre[100];
    int *asistencias;   // arreglo dinamico de asistencias por dia
    struct Alumno *siguiente;
} Alumno;


Alumno* crearAlumno(int id, const char *nombre, int dias);
void insertarAlumno(Alumno **inicio, int id, const char *nombre, int dias);
void pasarLista(Alumno *inicio, int dia);
void mostrarLista(Alumno *inicio, int dias);
void liberarLista(Alumno *inicio);

Alumno* crearAlumno(int id, const char *nombre, int dias) {
    Alumno *nuevo = malloc(sizeof(Alumno));
    if (nuevo == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    nuevo->id = id;
    strncpy(nuevo->nombre, nombre, sizeof(nuevo->nombre) - 1);
    nuevo->nombre[sizeof(nuevo->nombre) - 1] = '\0';

   
    nuevo->asistencias = calloc(dias, sizeof(int));
    if (nuevo->asistencias == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    nuevo->siguiente = NULL;
    return nuevo;
}

void insertarAlumno(Alumno **inicio, int id, const char *nombre, int dias) {
  Alumno *nuevo = crearAlumno(id, nombre, dias);
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
    printf("\n Pasando lista para el día %d:\n", dia + 1);
    while (t != NULL) {
        printf("¿El alumno %s (ID %d) está presente? (s/n): ", t->nombre, t->id);
        if (fgets(linea, sizeof(linea), stdin) == NULL) break;
        char ch = linea[0];
        if (tolower((unsigned char)ch) == 's') 
            t->asistencias[dia] = 1;
        else 
 t->asistencias[dia] = 0;
        t = t->siguiente;
    }
}

void mostrarLista(Alumno *inicio, int dias) {
    Alumno *t = inicio;
    puts("\n--- Reporte de Asistencia ---");
    printf("%-20s", "Nombre");
    for (int d = 0; d < dias; d++) {
        printf(" Dia%-3d", d + 1);
    }
    printf(" | Total\n");

    while (t != NULL) {
        int total = 0;
        printf("%-20s", t->nombre);
        for (int d = 0; d < dias; d++) {
            printf("   %s   ", t->asistencias[d] ? "P" : "A");
            total += t->asistencias[d];
        }
        printf(" | %d/%d\n", total, dias);
        t = t->siguiente;
    }
}
void liberarLista(Alumno *inicio) {
    Alumno *t;
    while (inicio != NULL) {
        t = inicio;
        inicio = inicio->siguiente;
        free(t->asistencias);
        free(t);
    }
}

int main(void) {
    Alumno *lista = NULL;
    char buffer[200];
    int n = 0, dias = 0;

    printf("¿Cuántos alumnos desea registrar? ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    n = atoi(buffer);

    printf("¿Cuántos días se quiere registrar la asistencia? ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    dias = atoi(buffer);
    if (dias <= 0 || dias > MAX_DIAS) {
        printf("Número de días inválido (máximo %d).\n", MAX_DIAS);
        return 0;
    }
for (int i = 1; i <= n; ++i) {
        printf("Ingrese el nombre del alumno %d: ", i);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) buffer[0] = '\0';
        buffer[strcspn(buffer, "\r\n")] = '\0';
        insertarAlumno(&lista, i, buffer, dias);
    }

    for (int d = 0; d < dias; d++) {
        pasarLista(lista, d);
    }

    mostrarLista(lista, dias);
    liberarLista(lista);
return 0;
}