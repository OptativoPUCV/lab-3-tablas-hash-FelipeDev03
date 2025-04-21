#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}

void insertMap(HashMap * map, char * key, void * value) {
    long posicion = hash(key, map->capacity);
    long inicio = posicion;  // Guardamos la posición inicial para evitar bucles infinitos

    while (1) {
        Pair * current = map->buckets[posicion];

        // Casilla vacía: se puede insertar
        if (current == NULL) {
            Pair * nuevo = malloc(sizeof(Pair));
            nuevo->key = key;
            nuevo->value = value;
            map->buckets[posicion] = nuevo;
            map->size++;
            map->current = posicion;
            return;
        }

        // Casilla inválida (par borrado): también se puede insertar
        if (current->key == NULL) {
            current->key = key;
            current->value = value;
            map->size++;
            map->current = posicion;
            return;
        }

        // Si la clave ya existe, no insertamos
        if (is_equal(current->key, key)) return;

        // Avanzar con comportamiento circular
        posicion = (posicion + 1) % map->capacity;

        // Si dimos una vuelta completa, salimos
        if (posicion == inicio) return;  // Mapa lleno o sin lugar disponible
    }
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)


}


HashMap * createMap(long capacity) {
    // Reservar mapa
    HashMap* mapa = (HashMap*)malloc(sizeof(HashMap));
    if (mapa == NULL) return NULL;

    // Reservar arreglo
    mapa->buckets = (Pair**) calloc(capacity, sizeof(Pair*));
    if (mapa->buckets == NULL) return NULL;

    // Inicializar variables
    mapa->size = 0;
    mapa->capacity = capacity;
    mapa->current = -1;

    return mapa;
}

void eraseMap(HashMap * map,  char * key) {    


}

Pair * searchMap(HashMap * map,  char * key) {   


    return NULL;
}

Pair * firstMap(HashMap * map) {

    return NULL;
}

Pair * nextMap(HashMap * map) {

    return NULL;
}
