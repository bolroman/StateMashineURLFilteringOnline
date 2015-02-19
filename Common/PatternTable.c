

#include <stdlib.h>
#include <stdio.h>
#include "PatternTable.h"

#define MAX_PATTERN_LENGTH 1024

PatternTable *patterntable_create(int size) {
	PatternTable *table = (PatternTable*)malloc(sizeof(PatternTable));
	char ***pats = (char***)malloc(sizeof(char**) * size);
	char*** hoffmanCodes = (char***)malloc(sizeof(char**) * size);
	int*** hoffmanCodelength = (int***)malloc(sizeof(int**) * size);
	int*** hasAdditionalCodes = (int***)malloc(sizeof(int**) * size);
	int*** patternLengths = (int***)malloc(sizeof(int**) * size);

	HoffmanTable*** additionalCodes = (HoffmanTable***)malloc(sizeof(HoffmanTable**) * size);

	table->patterns = pats;
	table->patternsLengths = patternLengths;
	table->hoffmanCode = hoffmanCodes;
	table->hoffmanCodeLen = hoffmanCodelength;
	table->hasAdditionalCodes = hasAdditionalCodes;
	table->additionalCodes = additionalCodes;
	table->size = size;
	return table;
}

void patterntable_destroy(PatternTable *table) {
	char*** patterns;
	int*** patternsLengths;
	char*** hoffmanCodes;
	int***  hoffmanCodelength;
	int***  hasAdditionalCodes;
	HoffmanTable*** additionalCodes;
	int i, j;

	patterns = table->patterns;
	hoffmanCodes = table->hoffmanCode;
	hoffmanCodelength = table->hoffmanCodeLen;
	hasAdditionalCodes = table->hasAdditionalCodes;
	additionalCodes = table->additionalCodes;
	patternsLengths = table->patternsLengths;

	for (i = 0; i < table->size; i++) {
		if (patterns[i] == NULL)
			continue;
		j = 0;
		while (patterns[i][j] != NULL) {
			free(patterns[i][j]);
			free(hoffmanCodes[i][j]);
			free(hoffmanCodelength[i][j]);
			free(hasAdditionalCodes[i][j]);
			free(additionalCodes[i][j]);

			j++;
		}
	}
	free(patterns);
	free(table);
}

void patterntable_add_state(PatternTable *table, STATE_PTR_TYPE_WIDE state, int numPatterns) {
	char **statePatterns;
	int** statePatternsLengths;
	char **stateHoffmanCodes;
	int **stateHoffmanCodeLength;
	int** stateHasAdditionalCodes;
	HoffmanTable** stateAdditionalCodes;

	int i;

	if (numPatterns == 0)
		return;

	statePatterns = (char**)malloc(sizeof(char*) * (numPatterns + 1));
	statePatternsLengths = (int**)malloc(sizeof(int*) * (numPatterns + 1));
	stateHoffmanCodes = (char**)malloc(sizeof(char*) * (numPatterns + 1));
	stateHoffmanCodeLength = (int**)malloc(sizeof(int*) * (numPatterns + 1));
	stateHasAdditionalCodes = (int**)malloc(sizeof(int*) * (numPatterns + 1));
	stateAdditionalCodes = (HoffmanTable**)malloc(sizeof(int*) * (numPatterns + 1));


	for (i = 0; i < numPatterns + 1; i++) {
		statePatterns[i] = NULL;
		statePatternsLengths[i]  = NULL;
		stateHoffmanCodes[i] = NULL;
		stateHoffmanCodeLength[i] = NULL;
		stateHasAdditionalCodes[i] = NULL;
		stateAdditionalCodes[i] = NULL;

	}

	table->patterns[state] = statePatterns;
	table->patternsLengths[state] = statePatternsLengths;
	table->hoffmanCode[state] = stateHoffmanCodes;
	table->hoffmanCodeLen[state] = stateHoffmanCodeLength;
	table->hasAdditionalCodes[state] = stateHasAdditionalCodes;
	table->additionalCodes[state] = stateAdditionalCodes;
}

#define TO_HEX(val) \
	(((val) >= 10) ? ('A' + ((val) - 10)) : (char)('0' + (val)))

char *createPattern(char *pattern, int len) {
	char buff[MAX_PATTERN_LENGTH];
	char *res;
	int i, j;


	for (i = 0, j = 0; i < len; i++)
	{
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

void patterntable_add_pattern(PatternTable *table, STATE_PTR_TYPE_WIDE sourceState, int idx, char *pattern, int len, unsigned char* hoffmanCode, int hoffmanCodeLen) {

	char *cpy;
	char tmp[1024];

	tmp[0] = ';';
	tmp[1] = '\0';
	strcat(tmp, pattern);

	if (table->patterns[sourceState][idx] != NULL)
	{
		if (strncmp(table->patterns[sourceState][idx], pattern, len)==0 || strstr(table->patterns[sourceState][idx], tmp)!=0)
		{
			return;
		}
		patterntable_update_pattern(table, sourceState, idx, pattern, len, hoffmanCode, hoffmanCodeLen);
		return;
	}

	cpy = createPattern(pattern, len);

	table->patterns[sourceState][idx] = cpy;
	table->patternsLengths[sourceState][idx] = (int*)malloc(sizeof(int));
	table->hoffmanCode[sourceState][idx] = (char*)malloc(sizeof(char) * (hoffmanCodeLen/8)+1);
	table->hoffmanCodeLen[sourceState][idx] = (int*)malloc(sizeof(int));
	table->hasAdditionalCodes[sourceState][idx] = (int*)malloc(sizeof(int));

	int patternLen = strlen(cpy);
	memcpy(table->patternsLengths[sourceState][idx], &patternLen, sizeof(int));

	int hasAdditionCodes = 0;
	memcpy(table->hasAdditionalCodes[sourceState][idx], &hasAdditionCodes, sizeof(int));

	if(hoffmanCode != 0)
	{
		memcpy(table->hoffmanCode[sourceState][idx], hoffmanCode, (hoffmanCodeLen/8)+1);
		memcpy(table->hoffmanCodeLen[sourceState][idx], &hoffmanCodeLen, sizeof(int));
	}
}

void patterntable_update_pattern(PatternTable *table, STATE_PTR_TYPE_WIDE sourceState, int idx, char *more, int len, unsigned char* hoffmanCode, int hoffmanCodeLen) {
	int oldlen, newlen, freeOld;
	char *old, *next, *new;

	new = createPattern(more, len);
	len = strlen(new);

	freeOld = 1;

	old = table->patterns[sourceState][idx];
	if (old == NULL) {
		old = "\0";
		freeOld = 0;
	}
	oldlen = strlen(old);

	newlen = oldlen + len + 1;

	next = (char*)malloc(sizeof(char) * (newlen + 1));

	strcpy(next, old);

	next[oldlen] = ';';
	strcpy(&(next[oldlen + 1]), new);

	table->patterns[sourceState][idx] = next;

	if(*table->hasAdditionalCodes[sourceState][idx] == 0)
	{
		if (hoffmanCode != 0)
		{
			//set multiple results flag
			int* hasAdditionalCodes;
			int hasCodes = 1;
			hasAdditionalCodes = (int*)malloc(sizeof(int));
			memcpy(hasAdditionalCodes,&hasCodes,sizeof(int));
			table->hasAdditionalCodes[sourceState][idx] = hasAdditionalCodes;

			HoffmanTable* hoffmanTable = (HoffmanTable*)malloc(sizeof(HoffmanTable));
			hoffmanTable->tableSize = 0;

			//add original pattern
			hoffmanTable->pattern[hoffmanTable->tableSize] = (char*)malloc(sizeof(char)*oldlen);
			memcpy(hoffmanTable->pattern[hoffmanTable->tableSize], old, oldlen+1);
			int* oldpatternLen = table->patternsLengths[sourceState][idx];
			hoffmanTable->patternlen[hoffmanTable->tableSize] = (*oldpatternLen);

			int* oldHoffmanCodeLen = table->hoffmanCodeLen[sourceState][idx];
			char* oldHoffmanCode = table->hoffmanCode[sourceState][idx];
			hoffmanTable->hoffmanCode[hoffmanTable->tableSize] = (unsigned char*)malloc(sizeof(unsigned char)*((*oldHoffmanCodeLen)/8)+1);
			memcpy(hoffmanTable->hoffmanCode[hoffmanTable->tableSize], oldHoffmanCode, ((*oldHoffmanCodeLen)/8)+1);
			hoffmanTable->hoffmanCodeLen[hoffmanTable->tableSize] = (*oldHoffmanCodeLen);
			hoffmanTable->tableSize++;

			//add new pattern
			hoffmanTable->pattern[hoffmanTable->tableSize] = (char*)malloc(sizeof(char)*len);
			memcpy(hoffmanTable->pattern[hoffmanTable->tableSize], new, len+1);
			hoffmanTable->patternlen[hoffmanTable->tableSize] = len;

			hoffmanTable->hoffmanCode[hoffmanTable->tableSize] = (unsigned char*)malloc(sizeof(unsigned char)*hoffmanCodeLen);
			memcpy(hoffmanTable->hoffmanCode[hoffmanTable->tableSize], hoffmanCode, (hoffmanCodeLen/8)+1);
			hoffmanTable->hoffmanCodeLen[hoffmanTable->tableSize] = hoffmanCodeLen;

			hoffmanTable->tableSize++;

			table->additionalCodes[sourceState][idx] = hoffmanTable;
		}
	}
	else if (hoffmanCode !=0 ){
		HoffmanTable* hoffmanTable = table->additionalCodes[sourceState][idx];
		hoffmanTable->pattern[hoffmanTable->tableSize] = (char*)malloc(sizeof(char)*len);
		memcpy(hoffmanTable->pattern[hoffmanTable->tableSize], new, len);
		hoffmanTable->patternlen[hoffmanTable->tableSize] = strlen(new);


		hoffmanTable->hoffmanCode[hoffmanTable->tableSize] = (unsigned char*)malloc(sizeof(unsigned char)*hoffmanCodeLen);
		memcpy(hoffmanTable->hoffmanCode[hoffmanTable->tableSize], hoffmanCode, (hoffmanCodeLen/8)+1);
		hoffmanTable->hoffmanCodeLen[hoffmanTable->tableSize] = hoffmanCodeLen;

		hoffmanTable->tableSize++;
	}

	if (freeOld) {
		free(old);
	}
}
