/*
 * Dictionary.c
 *
 *  Created on: Jan 24, 2013
 *      Author: yotam
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Dictionary.h"

#define max(x,y) (((x) > (y)) ? (x) : (y))

void dictionary_init(Dictionary *dict, int size, int width) {
	int i;
	dict->size = size;
	if (size > 0) {
		dict->table = (DictionaryEntry **)malloc(sizeof(DictionaryEntry*) * size);
		for (i = 0; i < size; i++) {
			dict->table[i] = NULL;
		}
	} else {
		dict->table = NULL;
	}
	dict->width = width;
#ifdef DICT_USES_BLOOM
	dict->bloom = fast_bloom_create(BLOOM_SIZE);//, 2, sax_hash, sdbm_hash);
	if (!(dict->bloom)) {
		fprintf(stderr, "FATAL ERROR: Cannot create bloom filter\n");
		exit(1);
	}
#endif
}

int dictionary_add_all_autosize(Dictionary *dict, RollingHashTool *hasher, DictionaryEntry **entries, int num_entries) {
	int i;
	int size, times;
	int problem;
	DictionaryEntry **table;
	unsigned long hash, idx;
#ifdef HASH_PROBING
	unsigned int probe;
#endif

	size = num_entries * 2;
	times = 2;

	srand(time(0));

	if (dict->table) {
		free(dict->table);
	}

	while (1) {
		table = (DictionaryEntry **)malloc(sizeof(DictionaryEntry*) * size);
		for (i = 0; i < size; i++) {
			table[i] = NULL;
		}

		problem = 0;
		for (i = 0; i < num_entries; i++) {
			HASH_WORD_NOROLL_16(hasher, entries[i]->word, dict->width, hash);
			idx = hash % size;
			if (table[idx]) {
				// Collision
#ifdef HASH_PROBING
				probe = 1;
				while (table[(hash + probe) % size] && probe <= size) {
					probe <<= 2;
				}
				if (table[(hash + probe) % size]) {
					fprintf(stderr, "Could not create hash, increasing size (entry: %d, hash: %lu, hash_idx: %lu, hash size: %d, entries: %d)\n", i, hash, idx, size, num_entries);
					problem = 1;
					break;

				} else {
					table[(hash + probe) % size] = entries[i];
				}
#else
				fprintf(stderr, "Could not create hash, increasing size (entry: %d, hash: %lu, hash_idx: %lu, hash size: %d, entries: %d)\n", i, hash, idx, size, num_entries);
				problem = 1;
				break;
#endif
			} else {
				table[idx] = entries[i];
			}
		}

		// If not ok, free table and increase size
		if (problem) {
			free(table);
			size += num_entries;
			times++;
			if (size > max(10 * num_entries, 1048576)) {
				fprintf(stderr, "FATAL ERROR: Hash table too big\n");
				exit(1);
			}
		} else {
			break;
		}
	}

	dict->table = table;
	dict->size = size;
#ifdef DICT_USES_BLOOM
	for (i = 0; i < num_entries; i++) {
		fast_bloom_add(dict->bloom, hasher, entries[i]->word, dict->width);
	}
#endif
	return size;
}

void dictionary_destroy(Dictionary *dict) {
	int i;
	for (i = 0; i < dict->size; i++) {
		if (dict->table[i] != NULL) {
			dictionaryentry_destroy(dict->table[i]);
			free(dict->table[i]);
		}
	}
	free(dict->table);
#ifdef DICT_USES_BLOOM
	fast_bloom_destroy(dict->bloom);
#endif
}

void dictionaryentry_init(DictionaryEntry *entry, Dictionary *dict) {
	entry->word = (char*)malloc(sizeof(char) * (dict->width + 1));
}

void dictionaryentry_destroy(DictionaryEntry *entry) {
	free(entry->word);
}

