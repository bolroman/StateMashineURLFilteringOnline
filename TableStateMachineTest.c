#include "TableStateMachineTest.h"


int CompressURLsMachine (char *path,TableStateMachine *machine, int *num_of_urls, Symbol symbolArr[NUM_OF_SYMBOLS], unsigned char results[10000][100])
{
//	/* load URLs to Memory */
//	int number_of_urls = *num_of_urls;
//	char *urls[10000];
//	FILE *f;
//
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
//
////	FILE *file = fopen("Tableoutput.txt", "w");
//
//	for(x=0; x<number_of_urls; x++)
//	{
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
//			current = GET_NEXT_STATE(machine->table, current, urls[x][idx]);
//			P[idx+1]=P[idx]+ symbolArr[urls[x][idx]].size;
//			ymins[idx+1]=-1*urls[x][idx];
//			if (GET_1BIT_ELEMENT(machine->matches, current))
//			{
//				curNode = machine->nodeTable[current];
//				N[idx+1]=curNode;
//
//				int y;
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
//		}
//		int z=length;
//
//		int resultCodeLen=0;
//		int resultIndex=0;
//
//		while(z>0)
//		{
//			if(ymins[z]>=0)
//			{
//				ConcutByteArrayToByteArray(results[x], &resultIndex,
//						&resultCodeLen, N[z]->huffman[ymins[z]], N[z]->huffman_len[ymins[z]]);
//
//				z-=strlen(N[z]->patterns[ymins[z]]);
//			}
//			else
//			{
//				if (symbolArr[ymins[z]*-1].size != 0)
//				{
//					ConcutByteArrayToByteArray(results[x], &resultIndex,
//							&resultCodeLen, symbolArr[ymins[z]*-1].symbolCode, symbolArr[ymins[z]*-1].size);
//				}
//				z--;
//			}
//		}
//
//		//RomanOutput
////		fprintf(file, "\nURL - %s\n", urls[x]);
////		int index;
////		for(index=0;index<resultIndex;index++)
////		{
////			fprintf(file, "%02X", results[x][index]);
////		}
//		//****************
//
//	}
//
////	fclose(file);
//
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
}
