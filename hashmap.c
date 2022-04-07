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
    long hashI = hash(key, map->capacity);
    Pair **buckets = map->buckets;
    while (1)
    {
        if (buckets[hashI] == NULL || buckets[hashI]->key == NULL)
        {
            buckets[hashI] = createPair(key, value);
            map->current = hashI;
            map->size++;
            break;
        }
        hashI = (hashI + 1) % map->capacity;
    }
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)

    Pair** oldBuckets = map->buckets;
    Pair** buckets = (Pair **) malloc (sizeof(Pair *) * map->capacity * 2);
    map->buckets = buckets;

    for (int i = 0; i < map->capacity; i++)
    {
        Pair *bucket = oldBuckets[i];
        if (bucket != NULL && bucket->key != NULL)
        {
            insertMap(map, bucket->key, bucket->value);
        }
    }
    map->capacity *= 2;
    free(oldBuckets);
    return;
}


HashMap * createMap(long capacity) {
    HashMap * map = (HashMap*) malloc (sizeof(HashMap));
    map->buckets = (Pair **) malloc (sizeof(Pair *) * capacity);
    map->capacity = capacity;
    map->current = -1;
    map->size = 0;
    return map;
}

void eraseMap(HashMap * map,  char * key) {    
    searchMap(map, key);
    map->buckets[map->current]->key = NULL;
    map->size--;
    return;
}

Pair * searchMap(HashMap * map,  char * key) {   
    long hashI = hash(key, map->capacity);
    while (map->buckets[hashI] != NULL)
    {
        if (strcmp(map->buckets[hashI]->key, key) == 0)
        {
            map->current = hashI;
            return map->buckets[hashI];
        }
        hashI = (hashI + 1) % map->capacity;
    }

    return NULL;
}

Pair * firstMap(HashMap * map) {
    if (map->size == 0) return NULL;
    for (int i = 0; i < map->capacity; i++)
    {
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL)
        {
            map->current = i;
            break;
        }
    }
    return map->buckets[map->current];
}

Pair * nextMap(HashMap * map) {
    if (map->size == 0) return NULL;
    int i = (map->current + 1) % map->capacity;
    while (1)
    {
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL)
        {
            map->current = i;
            break;
        }
        i++;
        if (i == (map->capacity + 1)) return NULL;
    }
    return map->buckets[map->current];
}