#include "CommonFunctions.h"

unsigned char ConcutBitsToSingleByte(unsigned char dest, int sizeOfDest,
		unsigned char source, int numOfBits)
{
	unsigned char result;
	result = ((dest>>(8-(sizeOfDest+numOfBits)))|(source>>(8-numOfBits)))<<(8-(sizeOfDest+numOfBits));

	return result;
}

void ConcutByteArrayToByteArray(unsigned char* dest, int* indexFrom,
		int* sizeOfDataInLastUsedIndex, unsigned char* source, int sizeOfSourceData)
{
	unsigned char tempSource[100];
	memcpy(tempSource, source, (sizeOfSourceData/8)+1);

	int sourceIndex = 0;
	int numOfRemainingToCopyBitsInSourceIndex = MIN(8,sizeOfSourceData);
	unsigned char sourceToCopy;
	int bitsToCopy;

	while (sizeOfSourceData>0)
	{
		sourceToCopy = tempSource[sourceIndex];//<<(8-numOfRemainingToCopyBitsInSourceIndex);

		bitsToCopy = MIN(numOfRemainingToCopyBitsInSourceIndex,(8-(*sizeOfDataInLastUsedIndex)));

		dest[*indexFrom]=ConcutBitsToSingleByte(dest[*indexFrom],
				*sizeOfDataInLastUsedIndex,sourceToCopy,bitsToCopy);

		tempSource[sourceIndex]=tempSource[sourceIndex]<<bitsToCopy;

		*sizeOfDataInLastUsedIndex += bitsToCopy;
		if (*sizeOfDataInLastUsedIndex==8)
		{
			(*indexFrom)++;
			(*sizeOfDataInLastUsedIndex)=0;
		}
		numOfRemainingToCopyBitsInSourceIndex-=bitsToCopy;
		sizeOfSourceData-=bitsToCopy;

		if (numOfRemainingToCopyBitsInSourceIndex==0)
		{
			sourceIndex++;
			numOfRemainingToCopyBitsInSourceIndex=MIN(8,sizeOfSourceData);
		}

	}

}

void ConvertHoffmanCodeFromStringToByteArray(unsigned char* output, char* code)
{
	int codeLen = strlen(code);
	int index = 0;
	int codeIndex = 0;

	while (codeLen != 0)
	{
		int i;
		for (i=0;i<8;++i)
		{
			output[index]|=(code[codeIndex]=='1')<<(7-i);

			codeIndex++;
			codeLen--;
			if (codeLen == 0) break;
		}

		index++;
	}
}

//this function converts string of ones and zeros to bits
void ConvertCodeFromStringToBits(char* convertedCode, unsigned char* code, int codeLen)
{
	int compURLIndex=0;
	int fullCodeStrIndex = 0;
	int quit=0;
	int i;
	char compressedURL = 0;
	int outputIndex=0;

	while(1)
	{
		compressedURL=0;
		for (i = 0; i < BITS_IN_BYTE; ++i )
		{
			if (fullCodeStrIndex==codeLen)
			{
				quit=1;
				break;
			}
			compressedURL |= (code[fullCodeStrIndex] == '1') << (BITS_IN_BYTE-1 - i);
			fullCodeStrIndex++;
		}
		memcpy(convertedCode+outputIndex,&compressedURL,1);
		outputIndex+=1;

		if(quit==1)
		{
			break;
		}
		compURLIndex++;
	}
}

void GetFileList(char* folderPath, char fileList[100][256], int* count)
{
  DIR *d;
  struct dirent *dir;
  d = opendir(folderPath);

  if (d)
  {
	while ((dir = readdir(d)) != NULL)
	{
	  memcpy(fileList[(*count)],dir->d_name,strlen(dir->d_name));
	  (*count)++;
	}

	closedir(d);
  }
}

int StartsWith(const char *a, const char *b)
{
   if(strncmp(a, b, strlen(b)) == 0) return 1;
   return 0;
}


