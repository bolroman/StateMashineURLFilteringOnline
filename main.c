#include <stdio.h>

#include "CommonFunctions.h"
#include "StateMachine/StateMachine.h"
#include "StateMachine/TableStateMachine.h"
#include "AhoCorasick/ACTypes.h"

//#define PATTERNS_PATH "/home/roman/workspace/dict_8000_8000"
//#define URL_PATH "/home/roman/workspace/8000_8000_urls1"

//#define PATTERNS_PATH "/home/parallels/workspace/dict_8000_8000"
//#define URL_PATH "/home/parallels/workspace/8000_8000_urls1"

void BuildAncorsData(char* path, Anchor anchorsArr[], Symbol SymbolArr[NUM_OF_SYMBOLS]);
void VerifyAllSymbols(char* path, Symbol SymbolArr[NUM_OF_SYMBOLS]);

StateMachine *createStateMachine(Anchor* anchorsArr, int numOfAnchors, int maxGotosLE, int maxGotosBM, int verbose);
int CompressURLsStateMachine (char *path,StateMachine *machine, int *num_of_urls, Symbol symbolArr[NUM_OF_SYMBOLS], unsigned char results[10000][100]);

int main(int argc, char** argv)
{
	/** argv[1] - HOME_DIR
	 *  argv[2] - ANCHOR/HUFFMAN FILE_NAME
	 */
	StateMachine *stateMachine;

	Anchor *anchorsArr;
	Symbol symbolArr[NUM_OF_SYMBOLS];
    int NUM_OF_ANCHORS=0;

	char path[100];
	memset(path,0,100);
//	strcat(path,argv[1]);
	strcat(path,"/home/roman/workspace/InputOutput/comp_reg/");
	//strcat(path,argv[2]);


	char fileList[100][256];
	int count=0;
	char dirName[100];
	memset(dirName,0,100);
	memcpy(dirName,path,100);

	GetFileList(dirName, fileList, &count);

	int i;
	for (i=0;i<count;i++)
	{
		usleep(1000);fflush(stdout);
		memset(path,0,100);

		if (StartsWith(fileList[i], "encoded") == 0)
		{
			continue;
		}

		memcpy(path,dirName,100);
		strcat(path,fileList[i]);

		FILE *f;
		f = fopen(path, "rb");
		if (!f) {
			fprintf(stderr, "Cannot read file %s\n", path);
			exit(1);
		}

		size_t read, len;
		char * line = NULL;

		while(!feof(f))
		{
		  read = getline(&line, &len, f);
		  if (read!=-1)
		  {
			  NUM_OF_ANCHORS++;
		  }
		}

		fclose(f);

		anchorsArr = (Anchor *)malloc(sizeof(Anchor)*(NUM_OF_ANCHORS+100));

		BuildAncorsData(path, anchorsArr, symbolArr);


		char path1[100] = {0};
//		strcat(path1,argv[1]);
//		strcat(path1,"/");
//		strcat(path1,argv[2]);
		strcat(path1,"/home/roman/workspace/InputOutput/random/full_domain1");

		VerifyAllSymbols(path1, symbolArr);

	   //build AC DFA
		stateMachine = createStateMachine(anchorsArr, NUM_OF_ANCHORS, 0, 0, 0);

//		printf("Start matching StateMachine\n");
//		char *input = "e-advisor.us";
//		int res = match(stateMachine, input, strlen(input), 1, NULL);
//		printf("End matching (res=%d)\n", res);

//		printf("Start matching TableStateMachine\n");
//		//char *input = "franktoncricketclub.co.uk";
//		res = match1TableMachine(tableStateMachine, input, strlen(input), 1, NULL, NULL,NULL,NULL,NULL);
//		printf("End matching (res=%d)\n", res);
//
//		return 1;

		unsigned char results[10000][100];


		int	count = 10000;
		int time = CompressURLsStateMachine(path1,stateMachine,&count,symbolArr, results);

		int leng =0;
		printf("%s %d %d %f %f\n", path, time, count, count*8.0/time, leng/(8.0*count)); fflush(stdout);
	}
	return 1;
}

void BuildAncorsData(char* path, Anchor anchorsArr[], Symbol SymbolArr[NUM_OF_SYMBOLS])
{
	FILE *f;
	f = fopen(path, "rb");
	if (!f) {
		fprintf(stderr, "Cannot read file %s\n", path);
		exit(1);
	}

	size_t read;
	char * line = NULL;
	size_t len = 0;

	int index = 0;
	char* token;
	int tokenLen;
	while(!feof(f))
	{
	  read = getline(&line, &len, f);
	  if (read!=-1)
	  {
		 token = strtok (line," ");
		 tokenLen = strlen(token);

         if(tokenLen>1)
         {
        	 anchorsArr[index].cUrl = (char*)malloc(tokenLen);
        	 memcpy(anchorsArr[index].cUrl, token, tokenLen);

        	 anchorsArr[index].dwSizeOfUrl = tokenLen;

        	 token = strtok (NULL, " ");
        	 tokenLen = atoi(strtok (NULL, " "));
        	 anchorsArr[index].HoffmanCodeLength = tokenLen;

        	 ConvertHoffmanCodeFromStringToByteArray(anchorsArr[index].cHoffmanCode, token);

        	 index++;
         }
         else
         {
        	 int symbol = (int)(token[0]);
        	 token = strtok (NULL, " ");
        	 tokenLen = atoi(strtok (NULL, " "));
        	 ConvertHoffmanCodeFromStringToByteArray(SymbolArr[symbol].symbolCode, token);
        	 SymbolArr[symbol].size = tokenLen;

         }
	  }
	}

	fclose(f);
}


void VerifyAllSymbols(char* path, Symbol SymbolArr[NUM_OF_SYMBOLS])
{
	FILE *f;
	f = fopen(path, "rb");
	if (!f) {
		fprintf(stderr, "Cannot read file %s\n", path);
		exit(1);
	}

	size_t read;
	char * line = NULL;
	size_t len = 0;

	while(!feof(f))
	{
	  read = getline(&line, &len, f);
	  if (read!=-1)
	  {
		 int i;
		 for(i=0;i<len;i++)
		 {
			 int sym = (int)(line[i]);
			 if(SymbolArr[sym].size == 0&&sym>10)
			 {
				 printf("SYMBOL NOT FOUND: %d *** %s \n", sym, line);
				 break;
			 }
		 }

	  }
	}

	fclose(f);
}
