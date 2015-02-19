

#ifndef PATTERNTABLE_H_
#define PATTERNTABLE_H_

#include "Types.h"
#include "HashMap/HashMap.h"

typedef struct{
	char* pattern[50];
	int patternlen[50];
	unsigned char* hoffmanCode [50];
	int hoffmanCodeLen[50];
	int tableSize;
}HoffmanTable;

typedef struct {
	char ***patterns;
	int*** patternsLengths;
	int size;
	unsigned char*** hoffmanCode;
	int*** hoffmanCodeLen;
	int*** hasAdditionalCodes;
	HoffmanTable ***additionalCodes;
} PatternTable;

PatternTable *patterntable_create(int size);
void patterntable_destroy(PatternTable *table);
void patterntable_add_state(PatternTable *table, STATE_PTR_TYPE_WIDE state, int numPatterns);
void patterntable_add_pattern(PatternTable *table, STATE_PTR_TYPE_WIDE sourceState, int idx, char *pattern, int len, unsigned char* hoffmanCode, int hoffmanCodeLen);
void patterntable_update_pattern(PatternTable *table, STATE_PTR_TYPE_WIDE sourceState, int idx, char *more, int len, unsigned char* hoffmanCode, int hoffmanCodeLen);

#endif /* PATTERNTABLE_H_ */
