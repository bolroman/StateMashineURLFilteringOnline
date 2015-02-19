/*
 * DictionaryReader.c
 *
 *  Created on: Jan 24, 2013
 *      Author: yotam
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "DictionaryReader.h"
#include "../StateMachine/TableStateMachine.h"

#define READ_BUFFER_SIZE 8192
#define MAX_ENTRIES 1048576

#define HEX_TO_INT(c) \
	(((c) > '9') ? ((c) - 'A' + 10) : ((c) - '0'))

#define TWOHEX_TO_INT(str) \
	((HEX_TO_INT((str)[1])) | ((HEX_TO_INT((str)[0])) << 4))

int dictionaryreader_read(const char *path, Dictionary *dict, RollingHashTool *hasher, TableStateMachine *machine) {
	// TODO: Read dictionary entries from file
	// For each entry, scan using machine from state 0
	// Then put it into dictionary

	FILE *f;
	char buff[READ_BUFFER_SIZE];
	char *word;
	int i;
	unsigned int lastState;
	int count, size;
	DictionaryEntry *entry;
	DictionaryEntry **entries;
	double bloom_ones_ratio, entries_to_bloom_ratio;

	f = fopen(path, "r");
	if (!f) {
		fprintf(stderr, "Error: cannot open dictionary file\n");
		return -1;
	}

	count = 0;
	word = (char*)malloc(sizeof(char) * dict->width);
	entries = (DictionaryEntry**)malloc(sizeof(DictionaryEntry*) * MAX_ENTRIES);

	while (fgets(buff, READ_BUFFER_SIZE, f) != NULL) {
		for (i = 0; i < dict->width; i++) {
			word[i] = TWOHEX_TO_INT(&(buff[i * 2]));
		}
		matchTableMachine(machine, word, dict->width, 0, NULL, NULL, NULL, &lastState);
		entry = (DictionaryEntry*)malloc(sizeof(DictionaryEntry));
		dictionaryentry_init(entry, dict);
		strncpy(entry->word, word, dict->width);
		entry->word[dict->width] = '\0';
		entry->state = lastState;

		entries[count] = entry;

		count++;

		if (count >= MAX_ENTRIES) {
			fprintf(stderr, "Too many entries in dictionary file. Only considering first %d entries.\n", count);
			break;
		}
	}

	size = dictionary_add_all_autosize(dict, hasher, entries, count);
#ifdef HASH_PROBING
	printf("Size of quadratic-probing hash table: %d entries\n", size);
#else
	printf("Size of no-collision hash table: %d entries\n", size);
#endif

	bloom_ones_ratio = fast_bloom_get_ratio(dict->bloom);
	entries_to_bloom_ratio = (double)count / BLOOM_SIZE;
#ifdef BLOOM_THIRD_FUNC
	entries_to_bloom_ratio *= 3;
#else
	entries_to_bloom_ratio *= 2;
#endif
	printf("Bloom filter ones ratio: %3.4f (%3.4f independency ratio)\n", bloom_ones_ratio, bloom_ones_ratio / entries_to_bloom_ratio);
	fast_bloom_print_hist(dict->bloom, 20);

	free(entries);
	free(word);

	fclose(f);

	return count;
}
