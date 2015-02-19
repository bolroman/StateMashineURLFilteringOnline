/*
 * HashMap.c
 *
 *  Created on: Jan 12, 2011
 *      Author: yotamhc
 */

#include <stdlib.h>
#include <stdio.h>
#include "HashMap.h"

HashMap *hashmap_create() {
	HashMap *map = (HashMap*)malloc(sizeof(HashMap));
	map->map = NULL;
	map->hh = NULL;
	return map;
}

void hashmap_destroy(HashMap *map) {
	// if needed - free all data in hash
	HashObject *obj, *tmp;
	HashObject *hm = map->map;

	HASH_ITER(hh, hm, obj, tmp) {
		HASH_DEL(hm, obj);
		free(obj);
	}

	free(map);
}

void *hashmap_get(HashMap *map, int key) {
	HashObject *obj = NULL;
	HASH_FIND_INT(map->map, &key, obj);
	if (obj != NULL) {
		return obj->data;
	}
	return NULL;
}

void hashmap_put(HashMap *map, int key, void *value) {
	HashObject *obj = (HashObject*)malloc(sizeof(HashObject));
	obj->key = key;
	obj->data = value;
	HASH_ADD_INT(map->map, key, obj);
}

int hashmap_remove(HashMap *map, int key) {
	HashObject *hm = map->map;
	HashObject *obj = NULL;
	HASH_FIND_INT(hm, &key, obj);
	if (obj != NULL) {
		HASH_DEL(hm, obj);
		free(obj);
		return 1;
	}
	return 0;
}

int hashmap_size(HashMap *map) {
	HashObject *hm = map->map;
	int size;
	size = HASH_COUNT(hm);
	return size;
}

void hashmap_iterator_reset(HashMap *map) {
	//map->hh = &(map->map->hh);
	map->iter = map->map;
}

void *hashmap_iterator_next(HashMap *map) {
	HashObject *obj;

	obj = map->iter;
	if (obj != NULL) {
		map->iter = obj->hh.next;
		return obj->data;
	}
	return NULL;

}

HashObject *hashmap_iterator_next_entry(HashMap *map) {
	HashObject *obj;

	obj = map->iter;
	if (obj != NULL) {
		map->iter = obj->hh.next;
		return obj;
	}
	return NULL;
}

void hashmap_print(HashMap *map, PrintElement printFunc) {
	HashObject *iter;

	printf("[ ");
	for (iter = map->map; iter != NULL; iter = iter->hh.next) {
		printFunc(iter->data);
		if (iter->hh.next != NULL)
			printf(",");
	}
	printf(" ]");
}

//jenkins_one_at_a_time_hash
int hashmap_generateKeyFromString(char* key, int len)
{
	uint32_t hash, i;
	for(hash = i = 0; i < len; ++i)
	{
		hash += key[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}
