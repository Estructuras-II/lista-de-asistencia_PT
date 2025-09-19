#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Alumno {
    int id;
    char nombre[50];
    int presente; 
    struct Alumno *siguiente;
} Alumno;


Alumno* crearAlumno(int id, const char *nombre);
void insertarAlumno(Alumno **inicio, int id, const char *nombre);
void pasarLista(Alumno *inicio);
void mostrarLista(Alumno *inicio);
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
    nuevo->presente = 0;
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

void pasarLista(Alumno *inicio) {
    Alumno *t = inicio;
    char linea[16];
    while (t != NULL) {
        printf("¿El alumno %s (ID %d) está presente? (s/n): ", t->nombre, t->id);
        if (fgets(linea, sizeof(linea), stdin) == NULL) break;
        char ch = linea[0];
        if (tolower((unsigned char)ch) == 's') t->presente = 1;
        else t->presente = 0;
        t = t->siguiente;
        
    }
}

void mostrarLista(Alumno *inicio) {
    Alumno *t = inicio;
    puts("\n Lista de Asistencia");
    while (t != NULL) {
        printf("ID: %2d | Nombre: %-30s | %s\n",
               t->id, t->nombre, (t->presente ? "Presente" : "Ausente"));
        t = t->siguiente;
    }
}

void liberarLista(Alumno *inicio) {
    Alumno *t;
    while (inicio != NULL) {
        t = inicio;
        inicio = inicio->siguiente;
        free(t);
    }
}

int main(void) {
    Alumno *lista = NULL;
    char buffer[200];
    int n = 0;
  printf("¿Cuántos alumnos desea registrar? ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    n = atoi(buffer);
    if (n <= 0) {
        printf("Número inválido.\n");
        return 0;
    }

    for (int i = 1; i <= n; ++i) {
        printf("Ingrese el nombre del alumno %d: ", i);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) buffer[0] = '\0';
       
        buffer[strcspn(buffer, "\r\n")] = '\0';
        insertarAlumno(&lista, i, buffer);
    }

    pasarLista(lista);
    mostrarLista(lista);
    liberarLista(lista);
    return 0;
}