#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    int id;
    char nombre[50];
    int presente;
    struct Alumno *siguiente;
} Alumno;

Alumno* crearAlumno(int id, const char *nombre) {
    Alumno nuevo = (Alumno)malloc(sizeof(Alumno));
    if (nuevo == NULL) {
        printf("Error al asignar memoria.\n");
        exit(1);
    }
   nuevo->id = id;
    strcpy(nuevo->nombre, nombre);
    nuevo->presente = 0; 
    nuevo->siguiente = NULL;
    return nuevo;
}

void insertarAlumno(Alumno **inicio, int id, const char *nombre) {
    Alumno *nuevo = crearAlumno(id, nombre);
    if (*inicio == NULL) {
        *inicio = nuevo;
    } else {
        Alumno *temp = *inicio;
        while (temp->siguiente != NULL) {
            temp = temp->siguiente;
        }
        temp->siguiente = nuevo;
    }
}

// Pasar lista a los alumnos
void pasarLista(Alumno *inicio) {
    Alumno *temp = inicio;
    char opcion;
    while (temp != NULL) {
        printf("¿El alumno %s (ID %d) está presente? (s/n): ", temp->nombre, temp->id);
        scanf(" %c", &opcion);
        if (opcion == 's' || opcion == 'S') {
            temp->presente = 1;
        } else {
            temp->presente = 0;
        }
        temp = temp->siguiente;
    }
}


void mostrarLista(Alumno *inicio) {
    Alumno *temp = inicio;
    printf("\n--- Lista de Asistencia ---\n");
    while (temp != NULL) {
        printf("ID: %d | Nombre: %-20s | %s\n", temp->id, temp->nombre,
               temp->presente ? "Presente" : "Ausente");
        temp = temp->siguiente;
    }
}

void liberarLista(Alumno *inicio) {
    Alumno *temp;
    while (inicio != NULL) {
        temp = inicio;
        inicio = inicio->siguiente;
        free(temp);
    }
}
