//#include <stdio.h>
//#include <sys/time.h>
//
//#include "StateMachine/StateMachine.h"
//#include "StateMachine/StateMachineGenerator.h"
//#include "StateMachine/TableStateMachine.h"
//#include "StateMachine/TableStateMachineGenerator.h"
//#include "AhoCorasick/ACBuilder.h"
//#include "AhoCorasick/ACTypes.h"
//#include "Common/BitArray/BitArray.h"
//
//#include <dirent.h>
//#include <unistd.h>
//
////#define PATTERNS_PATH "/home/roman/workspace/dict_8000_8000"
////#define URL_PATH "/home/roman/workspace/8000_8000_urls1"
//
////#define PATTERNS_PATH "/home/parallels/workspace/dict_8000_8000"
////#define URL_PATH "/home/parallels/workspace/8000_8000_urls1"
//
//#define DEFAULT_MAX_GOTOS_LE 4
//#define DEFAULT_MAX_GOTOS_BM 64
//
//#define NUM_OF_SYMBOLS 256
//#define BITS_IN_BYTE 8
//
//void BuildAncorsData(char* path, Anchor anchorsArr[], char SymbolArr[NUM_OF_SYMBOLS][100]);
//void VerifyAllSymbols(char* path, char SymbolArr[NUM_OF_SYMBOLS][100]);
//int  CompressURLsMachine (char *path,TableStateMachine *machine, int *urls_count, char symbolArr[NUM_OF_SYMBOLS][100], char results[10000][400]);
//void ConvertCodeFromStringToBits(char* convertedCode, unsigned char* code, int codeLen);
//
//void GetFileList(char* folderPath, char fileList[100][256], int* count);
//int StartsWith(const char *a, const char *b);
//
//int main(int argc, char** argv)
//{
//	/** argv[1] - HOME_DIR
//	 *  argv[2] - ANCHOR/HUFFMAN FILE_NAME
//	 */
//	TableStateMachine *machine;
//
//	Anchor *anchorsArr;
//	char symbolArr[NUM_OF_SYMBOLS][100];
//    int NUM_OF_ANCHORS=0;
//
//	char path[100];
//	memset(path,0,100);
//	strcat(path,argv[1]);
//	strcat(path,"/comp_reg/");
//	//strcat(path,argv[2]);
//
//
//	char fileList[100][256];
//	int count=0;
//	char dirName[100];
//	memset(dirName,0,100);
//	memcpy(dirName,path,100);
//
//	GetFileList(dirName, fileList, &count);
//
//	int i;
//	for (i=0;i<count;i++)
//	{
//		usleep(1000);fflush(stdout);
//		memset(path,0,100);
//
//		if (StartsWith(fileList[i], "encoded") == 0)
//		{
//			continue;
//		}
//
//		memcpy(path,dirName,100);
//		strcat(path,fileList[i]);
//
//		FILE *f;
//		f = fopen(path, "rb");
//		if (!f) {
//			fprintf(stderr, "Cannot read file %s\n", path);
//			exit(1);
//		}
//
//		size_t read, len;
//		char * line = NULL;
//
//		while(!feof(f))
//		{
//		  read = getline(&line, &len, f);
//		  if (read!=-1)
//		  {
//			  NUM_OF_ANCHORS++;
//		  }
//		}
//
//		fclose(f);
//
//	//	printf("%d",NUM_OF_ANCHORS ); fflush(stdout);
//
//		anchorsArr = (Anchor *)malloc(sizeof(Anchor)*(NUM_OF_ANCHORS+100));
//
//	//	printf("BEFORE"); fflush(stdout);
//				//prepare data for AC
//		BuildAncorsData(path, anchorsArr, symbolArr);
//
//
//		char path1[100] = {0};
//		strcat(path1,argv[1]);
//		strcat(path1,"/");
//		strcat(path1,argv[3]);
//
//		VerifyAllSymbols(path1, symbolArr);
//
//	   //build AC DFA
//		//machine = createStateMachine(anchorsArr,DEFAULT_MAX_GOTOS_LE, DEFAULT_MAX_GOTOS_BM, 0);
//		machine = generateTableStateMachine(anchorsArr,0);
//
//		char results[10000][400];
//
//		int	count = 10000;
//		int	time = CompressURLsMachine(path1,machine,&count,symbolArr, results);
//		int leng =0;
//		int v;
//		for(v=0;v<10000; v++)
//		{
//			leng+=strlen(results[v]);
//		}
//		printf("%s %d %d %f %f\n", path, time, count, count*8.0/time, leng/(8.0*count)); fflush(stdout);
//
//	}
//	//destroyTableStateMachine(machine);
//	return 1;
//}
//
//int CompressURLsMachine (char *path,TableStateMachine *machine, int *num_of_urls, char symbolArr[NUM_OF_SYMBOLS][100], char results[10000][400])
//{
//	/* load URLs to Memory */
//	int number_of_urls = *num_of_urls;
//	char *urls[10000];
//	FILE *f;
//	f = fopen(path, "rb");
//	if (!f) {
//		fprintf(stderr, "Cannot read file %s\n", path);
//		exit(1);
//	}
//	size_t read;
//	char * line = NULL;
//	size_t len = 0;
//    int next = 0;
//	while(!feof(f))
//	{
//	  read = getline(&line, &len, f);
//	  if (read!=-1)
//	  {
//		urls[next]= (char*)malloc(len*sizeof(char)+1);
//		strncpy(urls[next], line, len);
//		urls[next][len] = '\0';
//		next++;
//		if(next==number_of_urls) {
//			break;
//		}
//	  }
//	}
//
//	fclose(f);
//
//	int z;
//	for(z=0;z<number_of_urls;z++)
//	{
//
//		int length = strlen(urls[z]);
//		urls[z][length-1]=0;
//	}
//
//	struct timeval stop, start;
//	gettimeofday(&start, NULL);
//
//	int x;
//	for(x=0;x<number_of_urls;x++)
//	{
//
//		int length = strlen(urls[x]);
//		int idx = 0;
//		int current = 0;
//		Node *curNode;
//        int P[100];
//        Node *N[100];
//        int ymins[100];
//
//        while (idx < length)
//		{
//           current = GET_NEXT_STATE(machine->table, current, urls[x][idx]);
//            P[idx+1]=P[idx]+strlen(symbolArr[urls[x][idx]]);
//			ymins[idx+1]=-1*urls[x][idx];
//			if (GET_1BIT_ELEMENT(machine->matches, current))
//			{
//	            curNode = machine->nodeTable[current];
//	            N[idx+1]=curNode;
//
//	            int y;
//				for (y=0; y<curNode->numMatches; y++)
//				{
//					if(P[idx-strlen(curNode->patterns[y])+1]+curNode->huffman_len[y]<P[idx+1])
//					{
//						ymins[idx+1]=y;
//					}
//				}
//			}
//			if(ymins[idx+1]>=0)
//			{
//				P[idx+1]=P[idx-strlen(curNode->patterns[ymins[idx+1]])+1]+curNode->huffman_len[ymins[idx+1]];
//			}
//
//			idx++;
//
//		}
//		int z=length;
//
//		while(z>0)
//		{
//			if(ymins[z]>=0)
//			{
//				strcat(results[x],N[z]->huffman[ymins[z]]);
//				z-=strlen(N[z]->patterns[ymins[z]]);
//			}
//			else
//			{
//				strcat(results[x],symbolArr[ymins[z]*-1]);
//				z--;
//			}
//		}
//
//	}
//
//	gettimeofday(&stop, NULL);
//    int count = 0;
//
//    int c;
//    for(c=0;c<number_of_urls;c++)
//    {
//    	count+=strlen(urls[c]);
//    }
//	*num_of_urls=count;
//
//	return 1000000*(stop.tv_sec-start.tv_sec)+ stop.tv_usec - start.tv_usec;
//}
//
//void BuildAncorsData(char* path, Anchor anchorsArr[], char SymbolArr[NUM_OF_SYMBOLS][100])
//{
//	FILE *f;
//	f = fopen(path, "rb");
//	if (!f) {
//		fprintf(stderr, "Cannot read file %s\n", path);
//		exit(1);
//	}
//
//	size_t read;
//	char * line = NULL;
//	size_t len = 0;
//
//	int index = 0;
//	char* token;
//	int tokenLen;
//	while(!feof(f))
//	{
//	  read = getline(&line, &len, f);
//	  if (read!=-1)
//	  {
//		 token = strtok (line," ");
//		 tokenLen = strlen(token);
//         if(tokenLen>1)
//         {
//        	 anchorsArr[index].cUrl = (char*)malloc(tokenLen);
//        	 memcpy(anchorsArr[index].cUrl, token, tokenLen);
//
//        	 anchorsArr[index].dwSizeOfUrl = tokenLen;
//
//        	 token = strtok (NULL, " ");
//        	 tokenLen = atoi(strtok (NULL, " "));
//        	 anchorsArr[index].HoffmanCodeLength = tokenLen;
//        	 memcpy(anchorsArr[index].cHoffmanCode, token, tokenLen);
//        	 index++;
//         }
//         else
//         {
//        	 int symbol = (int)(token[0]);
//        	 token = strtok (NULL, " ");
//        	 tokenLen = atoi(strtok (NULL, " "));
//        	 memcpy(SymbolArr[symbol], token, tokenLen);
//        	 SymbolArr[symbol][tokenLen]=0;
//         }
//	  }
//	}
//
//	fclose(f);
//}
//
//
//void VerifyAllSymbols(char* path, char SymbolArr[NUM_OF_SYMBOLS][100])
//{
//	FILE *f;
//	f = fopen(path, "rb");
//	if (!f) {
//		fprintf(stderr, "Cannot read file %s\n", path);
//		exit(1);
//	}
//
//	size_t read;
//	char * line = NULL;
//	size_t len = 0;
//
//	while(!feof(f))
//	{
//	  read = getline(&line, &len, f);
//	  if (read!=-1)
//	  {
//		 int i;
//		 for(i=0;i<len;i++)
//		 {
//			 int sym = (int)(line[i]);
//			 if(strlen(SymbolArr[sym])==0&&sym>10)
//			 {
//				 printf("SYMBOL NOT FOUND: %d *** %s \n", sym, line);
//				 break;
//			 }
//		 }
//
//	  }
//	}
//
//	fclose(f);
//}
//
////this function converts string of ones and zeros to bits
//void ConvertCodeFromStringToBits(char* convertedCode, unsigned char* code, int codeLen)
//{
//	int compURLIndex=0;
//	int fullCodeStrIndex = 0;
//	int quit=0;
//	int i;
//	char compressedURL = 0;
//	int outputIndex=0;
//
//	while(1)
//	{
//		compressedURL=0;
//		for (i = 0; i < BITS_IN_BYTE; ++i )
//		{
//			if (fullCodeStrIndex==codeLen)
//			{
//				quit=1;
//				break;
//			}
//			compressedURL |= (code[fullCodeStrIndex] == '1') << (BITS_IN_BYTE-1 - i);
//			fullCodeStrIndex++;
//		}
//		memcpy(convertedCode+outputIndex,&compressedURL,1);
//		outputIndex+=1;
//
//		if(quit==1)
//		{
//			break;
//		}
//		compURLIndex++;
//	}
//}
//
//void GetFileList(char* folderPath, char fileList[100][256], int* count)
//{
//  DIR *d;
//  struct dirent *dir;
//  d = opendir(folderPath);
//
//  if (d)
//  {
//	while ((dir = readdir(d)) != NULL)
//	{
//	  memcpy(fileList[(*count)],dir->d_name,strlen(dir->d_name));
//	  (*count)++;
//	}
//
//	closedir(d);
//  }
//}
//
//int StartsWith(const char *a, const char *b)
//{
//   if(strncmp(a, b, strlen(b)) == 0) return 1;
//   return 0;
//}
//
