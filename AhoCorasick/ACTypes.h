#ifndef ACTYPES_H_
#define ACTYPES_H_

#include "../Common/HashMap/HashMap.h"

struct st_node;

typedef struct {
	char c;
	struct st_node *ptr;
} Pair;

typedef struct st_node {
	int realID;
	int id;
	int numGotos;
	int match;
	int numMatches;
	char *message;
	int messageLength;
	unsigned char* huffman;
	int huffman_len;
	HashMap *gotos;
	struct st_node *failure;
	int hasFailInto; // TRUE if some other node fails into this node
	int depth;
	char c1, c2;
	int isFirstLevelNode;
	int isSecondLevelNode;
	int marked;
	char* patterns[20];
	unsigned char* hoffmanCodes[20];
	int hoffmanCodesLength[20];
	int numOfPatterns;

} Node;

typedef struct {
	Node *root;
	unsigned int size;
} ACTree;

#endif /* ACTYPES_H_ */
