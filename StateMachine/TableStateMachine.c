/*
 * TableStateMachine.c
 *
 *  Created on: Jan 23, 2011
 *      Author: yotamhc
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../Common/Flags.h"
#include "../Common/BitArray/BitArray.h"
#include "TableStateMachine.h"

#define MAX_PATTERN_LENGTH 1024



TableStateMachine *createTableStateMachine(unsigned int numStates) {
	TableStateMachine *machine;
	STATE_PTR_TYPE_WIDE *table;
	unsigned char *matches;
	char **patterns;
	Node **NodeTable;
#ifdef USE_DEPTHMAP
	int *depthMap;
#endif

	machine = (TableStateMachine*)malloc(sizeof(TableStateMachine));
	table = (STATE_PTR_TYPE_WIDE*)malloc(sizeof(STATE_PTR_TYPE_WIDE) * numStates * 256);
	matches = (unsigned char*)malloc(sizeof(unsigned char) * (int)(ceil(numStates / 8.0)));
	patterns = (char**)malloc(sizeof(char*) * numStates);
	NodeTable = (Node**)malloc(sizeof(Node*) * numStates);

#ifdef USE_DEPTHMAP
	depthMap = (int*)malloc(sizeof(int) * numStates);
#endif


	memset(table, 0, sizeof(STATE_PTR_TYPE_WIDE) * numStates * 256);
	memset(matches, 0, sizeof(unsigned char) * (int)(ceil(numStates / 8.0)));
	memset(patterns, 0, sizeof(char*) * numStates);

	machine->table = table;
	machine->numStates = numStates;
	machine->matches = matches;
	machine->patterns = patterns;
#ifdef USE_DEPTHMAP
	machine->depthMap = depthMap;
#endif
	machine->nodeTable = NodeTable;
	return machine;
}

void destroyTableStateMachine(TableStateMachine *machine) {
	unsigned int i;

	for (i = 0; i < machine->numStates; i++) {
		if (machine->patterns[i]) {
			free(machine->patterns[i]);
		}
	}
	free(machine->patterns);
	free(machine->matches);
	free(machine->table);
#ifdef USE_DEPTHMAP
	free(machine->depthMap);
#endif
	free(machine);
}

void setGoto(TableStateMachine *machine, STATE_PTR_TYPE_WIDE currentState, char c, STATE_PTR_TYPE_WIDE nextState) {
	machine->table[GET_TABLE_IDX(currentState, c)] = nextState;
}

#define TO_HEX(val) \
	(((val) >= 10) ? ('A' + ((val) - 10)) : (char)('0' + (val)))

char *createPattern_TM(char *pattern, int len) {
	char buff[MAX_PATTERN_LENGTH];
	char *res;
	int i, j;

	for (i = 0, j = 0; i < len; i++) {
		if (pattern[i] >= 32 && pattern[i] < 127) {
			buff[j++] = pattern[i];
		} else {
			buff[j++] = '|';
			buff[j++] = TO_HEX((pattern[i] & 0x0F0) >> 4);
			buff[j++] = TO_HEX(pattern[i] & 0x00F);
			buff[j++] = '|';
		}
	}
	buff[j++] = '\0';
	res = (char*)malloc(sizeof(char) * j);
	strcpy(res, buff);
	return res;
}

void setMatch(TableStateMachine *machine, STATE_PTR_TYPE_WIDE state, char *pattern, int length) {
	char *cpy;
	SET_1BIT_ELEMENT(machine->matches, state, 1);
	cpy = createPattern_TM(pattern, length);
	machine->patterns[state] = cpy;
}

STATE_PTR_TYPE_WIDE getNextStateFromTable(TableStateMachine *machine, STATE_PTR_TYPE_WIDE currentState, char c) {
	return GET_NEXT_STATE(machine->table, currentState, c);
}

#ifdef DEPTHMAP_INFOCOM
static long _idx = 0;
#endif

int matchTableMachine(TableStateMachine *machine, char *input, int length, int verbose,
		long *numAccesses, long *accessesByDepth, long *accessesByState, unsigned int *lastState)
{
	return match1TableMachine(machine, input, length, verbose,
			numAccesses, accessesByDepth, accessesByState, lastState, 0);
}

int match1TableMachine(TableStateMachine *machine, char *input, int length, int verbose,
		long *numAccesses, long *accessesByDepth, long *accessesByState, unsigned int *lastState, char foundedAnchorsArr[][100])
{
	STATE_PTR_TYPE_WIDE current, next;
	STATE_PTR_TYPE_WIDE *table;
	unsigned char *matches;
	char **patterns;
	int idx;
	int res;
	int returnIndex=0;
	int sizeOfPattern;

	table = machine->table;
	matches = machine->matches;
	patterns = machine->patterns;
	idx = 0;
	current = 0;

	while (idx < length)
	{
		next = GET_NEXT_STATE(table, current, input[idx]);

		if (GET_1BIT_ELEMENT(matches, next))
		{
			if (patterns[next] != 0)
			{
				printf("%s\n", patterns[next]);
//				if (foundedAnchorsArr!=0)
//				{
//					sizeOfPattern = strlen(patterns[next]);
//					memcpy(foundedAnchorsArr[returnIndex], patterns[next], sizeOfPattern+1);
//				}

				returnIndex++;
			}

		}
		current = next;
		idx++;
	}
	if (lastState)
	{
		*lastState = current;
	}

	return returnIndex;
}

#define SCAN_SINGLE_CHAR(idx, trace, print_matches) \
	do {																			\
		next = GET_NEXT_STATE(table, current, input[(idx)]);						\
																					\
		if (trace) {																\
			int val;																\
			printf("Current state: %d, next char: ", current);						\
			val = (int)((input[(idx)]) & 0x0FF);									\
			if (val >= 32 && val < 127) {											\
				printf("%c", input[(idx)]);											\
			} else {																\
				printf("|%0X|", val);												\
			}																		\
			printf(", %s, next state: %d\n", (GET_1BIT_ELEMENT(matches, next) ? "matched" : "not matched"), next);	\
		}																			\
		if (GET_1BIT_ELEMENT(matches, next)) {										\
			/* It's a match! */														\
			res = 1;																\
			if (print_matches) {													\
				if (verbose) {														\
					printf("%s\n", patterns[next]);									\
					if (trace) {													\
									\
					}																\
				}																	\
			}																		\
		}																			\
		current = next;																\
		(idx)++;																	\
	} while (0)

#define SCAN_SINGLE_CHAR_SHORT(idx, trace, print_matches) \
	do {																			\
		\
		next = GET_NEXT_STATE(table, current, input[(idx)]);						\
																					\
		if (GET_1BIT_ELEMENT(matches, next)) {										\
			/* It's a match! */														\
			res = 1;																\
			if (print_matches) {													\
				if (verbose) {														\
					printf("%s\n", patterns[next]);									\
					if (trace) {													\
										\
					}																\
				}																	\
			}																		\
		}																			\
		current = next;																\
		(idx)++;																	\
	} while (0)

int matchDictionaryTableMachine(TableStateMachine *machine, char *input, int length, int verbose,
			Dictionary *dict, int dict_word_size, RollingHashTool *hasher, long *totalSkipped, long *totalLeftBorderChars, long *totalLeftBorders,
			long *dictGetTries, long *memcmpTries, long *memcmpFails) {
	STATE_PTR_TYPE_WIDE current, next;
	STATE_PTR_TYPE_WIDE *table;
	unsigned char *matches;
	char **patterns;
	int idx;
	int res;
	int bloom_not_found;

#ifdef COUNT_DICTIONARY_SKIPPED_BYTES
	int skipped, noskip;
#endif
#ifdef COUNT_MEMCMP_FAILURES
	int memcmp_tries, memcmp_fails, dict_get_tries;
#endif

	DictionaryEntry *entry;
	unsigned long hash;
	int potentialJump;
	int j;

	int trace = 0;
	int print_matches = 0;

	res = 0;
	table = machine->table;
	matches = machine->matches;
	patterns = machine->patterns;
	idx = 0;
	current = 0;

	HASH_WORD_NOROLL_16(hasher, input, dict_word_size, hash);

#ifdef COUNT_DICTIONARY_SKIPPED_BYTES
	skipped = 0;
#endif
#ifdef TRACE_STATE_MACHINE
	trace = 1;
#endif
#ifdef PRINT_MATCHES
	print_matches = 1;
#endif
#ifdef COUNT_MEMCMP_FAILURES
	memcmp_tries = 0;
	memcmp_fails = 0;
	dict_get_tries = 0;
#endif


	while (idx < length) {

#ifdef TEST_BLOOM_PERF_ONLY

		int val;
		FAST_BLOOM_CHECK(dict->bloom, hasher, &(input[idx]), 16, hash, val);


		if (val) {
			idx += 16;
			skipped += val;
			//val++;
		} else {
			idx++;
		}

		HASH_WORD_NOROLL_16(hasher, &(input[idx]), dict_word_size, hash);


#else
		if (idx + dict_word_size < length) {
			DICTIONARY_GET(dict, hasher, &(input[idx]), hash, entry, bloom_not_found);
#ifdef COUNT_MEMCMP_FAILURES
			dict_get_tries++;
			if (!bloom_not_found) {
				memcmp_tries++;

			}
#endif

			if (entry) {
				// Word is in dictionary!
				potentialJump = entry->state;
	#ifdef COUNT_DICTIONARY_SKIPPED_BYTES
				noskip = 0;
	#endif
				for (j = idx; j < idx + dict_word_size; ) {
					if (machine->depthMap[current] <= (j - idx)) {
						break;
					}
	#ifdef COUNT_DICTIONARY_SKIPPED_BYTES
					noskip++;
	#endif
					SCAN_SINGLE_CHAR_SHORT(j, trace, print_matches);
				}
				if (j < idx + dict_word_size) {
					// Jumping using the dictionary
					current = potentialJump;
					idx += dict_word_size;
				} else {
					// Not jumping anywhere eventually
					idx = j;
				}
	#ifdef COUNT_DICTIONARY_SKIPPED_BYTES
				skipped += dict_word_size - noskip;
				(*totalLeftBorderChars) += noskip;
				(*totalLeftBorders)++;
	#endif
				// Update hash
				HASH_WORD_NOROLL_16(hasher, &(input[idx]), dict_word_size, hash);
				continue;
			}
#ifdef COUNT_MEMCMP_FAILURES
			else {
				if (!bloom_not_found) {
					memcmp_fails++;

				}
			}
#endif
		}
		// Not in dictionary (or not enough characters left and ending is not in dictionary)
		SCAN_SINGLE_CHAR_SHORT(idx, trace, print_matches);
		if (idx + dict_word_size < length) {
			HASH_WORD_NOROLL_16(hasher, &(input[idx]), dict_word_size, hash);
		}
#endif
	}

#ifdef COUNT_DICTIONARY_SKIPPED_BYTES
	if (totalSkipped) {
		*totalSkipped += skipped;
	}
#endif
#ifdef COUNT_MEMCMP_FAILURES
	if (memcmpFails) {
		*memcmpFails += memcmp_fails;
	}
	if (memcmpTries) {
		*memcmpTries += memcmp_tries;
	}
	if (dictGetTries) {
		*dictGetTries += dict_get_tries;
	}
#endif
	return res;
}
