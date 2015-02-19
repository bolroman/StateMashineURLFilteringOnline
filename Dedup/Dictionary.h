/*
 * Hashing.h
 *
 *  Created on: Jan 24, 2013
 *      Author: yotam
 */

#ifndef HASHING_H_
#define HASHING_H_

#include "../Common/Flags.h"
#include "hash.h"
#ifdef DICT_USES_BLOOM
#include "bloom.h"
#endif


#ifdef DICT_USES_BLOOM
#define BLOOM_SIZE 131072 //262144 //524288 // In bits
#endif

#define NO_DICTIONARY_REWRITE

#define APPLE_MEMCMP(mem1, mem2, length, result) \
	(result = memcmp(mem1, mem2, length))

#define LINUX_MEMCMP(mem1, mem2, length, result) \
	(result = __builtin_memcmp(mem1, mem2, length))

#ifdef __APPLE__
#define MY_MEMCMP APPLE_MEMCMP
#else
#define MY_MEMCMP LINUX_MEMCMP
#endif

typedef struct {
	char *word;
	int state;
} DictionaryEntry;

typedef struct {
	int size;
	DictionaryEntry **table;
#ifdef DICT_USES_BLOOM
	BLOOM *bloom;
#endif
	int width;
} Dictionary;

void dictionary_init(Dictionary *dict, int size, int width);
int dictionary_add_all_autosize(Dictionary *dict, RollingHashTool *hasher, DictionaryEntry **entries, int num_entries);
void dictionary_destroy(Dictionary *dict);

void dictionaryentry_init(DictionaryEntry *entry, Dictionary *dict);
void dictionaryentry_destroy(DictionaryEntry *entry);

#ifdef HASH_PROBING
#define DICTIONARY_GET_INNER(dict, current_word, hash, target, dummy) 										\
	{																						\
		unsigned int probe; \
		int res;																				\
		(target) = (dict)->table[hash % (dict)->size];												\
		if (target) {																			\
			 \
			MY_MEMCMP((target)->word, (current_word), (dict)->width, res);						\
			if (res != 0) {																		\
				/* Possible collision, start probing */ \
				probe = 1; \
				while (probe <= 2 * (dict)->size) { \
					(target) = (dict)->table[(hash + probe) % (dict)->size];												\
					if (target) {																			\
						/* Found something, check matching */ \
						MY_MEMCMP((target)->word, (current_word), (dict)->width, res);						\
						if (res) { \
							/* No match, continue probing */ \
							probe <<= 2; \
						} else {\
							/* Matched! */ \
							break; \
						} \
					} else { \
						(target) = NULL; \
						break; \
					} \
				} \
				if (probe > (dict)->size) { \
					/* Probing did not help, this is not in dictionary */ \
					(target) = NULL;															\
				} \
			}																					\
		}																						\
	}
#else
#define DICTIONARY_GET_INNER(dict, current_word, hash, target, dummy) 										\
	{																						\
		unsigned int probe; \
		int res;																				\
		(target) = (dict)->table[hash % (dict)->size];												\
		if (target) {																			\
			\
			MY_MEMCMP((target)->word, (current_word), (dict)->width, res);						\
			if (res != 0) {																		\
				/* Not found */ \
				(target) = NULL; \
			}																					\
		}																						\
	}
#endif


#ifdef DICT_USES_BLOOM

#define DICTIONARY_GET(dict, hash_object_ptr, current_word, hash, target, bloom_not_found) \
	{ \
		int res; \
		FAST_BLOOM_CHECK((dict)->bloom, hash_object_ptr, (current_word), (dict)->width, hash, res); \
		if (!res) { \
			target = NULL; \
			bloom_not_found = 1; \
		} else { \
			bloom_not_found = 0; \
			DICTIONARY_GET_INNER(dict, current_word, hash, target, NULL); \
		} \
	}
#else
#define DICTIONARY_GET DICTIONARY_GET_INNER
#endif

#endif /* HASHING_H_ */
