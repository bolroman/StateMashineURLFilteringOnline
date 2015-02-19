#ifndef COMMONFUNCTIONS_H_
#define COMMONFUNCTIONS_H_

#include <dirent.h>
#include <unistd.h>
#include <math.h>
#include <stdio.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define NUM_OF_SYMBOLS 256
#define BITS_IN_BYTE 8

typedef struct
{
	unsigned char symbolCode[100];
	int size;
} Symbol;

unsigned char ConcutBitsToSingleByte(unsigned char dest, int sizeOfDest,
		unsigned char source, int numOfBits);

void ConcutByteArrayToByteArray(unsigned char* dest, int* indexFrom,
		int* sizeOfDataInLastUsedIndex, unsigned char* source, int sizeOfSourceData);

void ConvertHoffmanCodeFromStringToByteArray(unsigned char* output, char* code);

void ConvertCodeFromStringToBits(char* convertedCode, unsigned char* code, int codeLen);

void GetFileList(char* folderPath, char fileList[100][256], int* count);

int StartsWith(const char *a, const char *b);

#endif /* COMMONFUNCTIONS_H_ */
