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
    // Obtener posición y guardarlo como inicio
    long posicion = hash(key, map->capacity);
    long inicio = posicion;

    while (1) {
        Pair * pairActual = map->buckets[posicion];

        // Inserción con casilla vacia
        if (pairActual == NULL) {
            Pair * pairNuevo = malloc(sizeof(Pair));
            pairNuevo->key = key;
            pairNuevo->value = value;
            map->buckets[posicion] = pairNuevo;
            map->size++;
            map->current = posicion;
            return;
        }

        // Inserción con key nulo
        if (pairActual->key == NULL) {
            pairActual->key = key;
            pairActual->value = value;
            map->size++;
            map->current = posicion;
            return;
        }

        // Si la clave es repetida, no se inserta
        if (is_equal(pairActual->key, key)) return;

        // Avanzar con comportamiento circular
        posicion = (posicion + 1) % map->capacity;

        // Con mapa sin casilla disponible, no insertamos cuando volvemos al inicio
        if (posicion == inicio) return;  
    }
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)

    Pair** oldBuckets = map->buckets;
    long oldCapacity = map->capacity;
    map->capacity *= 2;

    map->buckets = map->buckets = (Pair**) calloc(map->capacity, sizeof(Pair*));

    map->size = 0;

    for (int i = 0; i < oldCapacity; i++){
        if (oldBuckets[i] != NULL)
            insertMap(map, oldBuckets[i]->key, oldBuckets[i]->value);
    }

    free(oldBuckets);
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
    // Obtener posición 
    long posicion = hash(key, map->capacity);

    while (map->buckets[posicion] != NULL){
        // Si la clave ingresada coincide con la actual, marcamos el key de la posición como NULL
        if (is_equal(map->buckets[posicion]->key, key)){
            map->buckets[posicion]->key = NULL;
            map->size--;
            return;
        }
        // Si no, avanzamos a siguiente posición
        posicion = (posicion + 1) % map->capacity;
    }
}

Pair * searchMap(HashMap * map,  char * key) {
    // Obtener posición 
    long posicion = hash(key, map->capacity);

    // Recorrer hasta encontrar casilla vacia o al terminar el arreglo
    while (map->buckets[posicion] != NULL){

        // Actualizar current
        map->current = posicion;

        // Si la clave ingresada coincide con la actual, retornamos el Pair
        if (is_equal(map->buckets[posicion]->key, key))
            return map->buckets[posicion];

        // Si no, avanzamos a siguiente posición
        posicion = (posicion + 1) % map->capacity;
    }

    // Clave no encontrada
    return NULL;
}

Pair * firstMap(HashMap * map) {
    // Iniciamos el current con 0
    map->current = 0;

    // Recorremos el arreglo hasta el final de la capacidad
    while (map->current != map->capacity-1){
        // Si la casilla no es vacia y el key no este marcado NULL, retornamos el pair
        if (map->buckets[map->current] != NULL && map->buckets[map->current]->key != NULL)
            return map->buckets[map->current];

        // Si no, avanzamos el current a la siguiente posición
        map->current++;
    }

    // No hay pair valido
    return NULL;
}

Pair * nextMap(HashMap * map) {
    // Empezamos a recorrer con la siguiente posición
    map->current++;

    // Recorrer hasta el final del arreglo
    while (map->current != map->capacity-1){
        // Si la casilla no es vacia y el key no este marcado NULL, retornamos el pair
        if (map->buckets[map->current] != NULL && map->buckets[map->current]->key != NULL)
            return map->buckets[map->current];

        // Si no, avanzamos el current a la siguiente posición
        map->current++;
    }

    // No hay pair valido
    return NULL;
}
