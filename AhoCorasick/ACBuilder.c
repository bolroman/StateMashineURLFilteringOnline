#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "../Common/Flags.h"
#include "ACBuilder.h"
#include "NodeQueue.h"

#define READ_BUFFER_SIZE 1024
#define MAX_STATES 65536
#define MAX_STATES_ACTIVE
#define STATES_HARD_LIMIT (2*1048576) //524288


Node *createNewNode(ACTree *tree, Node *parent) {
	Node *node = (Node*)malloc(sizeof(Node));
	node->id = tree->size++;
	node->message = NULL;
	node->messageLength = -1;
	node->gotos = hashmap_create();
	if (node->gotos == NULL) {
		fprintf(stderr, "ERROR: Cannot create new hashmap\n");
		exit(1);
	}
	node->failure = NULL;
	node->numGotos = 0;
	node->match = 0;
	node->hasFailInto = 0;
	if (parent != NULL) {
		node->depth = parent->depth + 1;
	} else {
		node->depth = 0;
	}
	node->isFirstLevelNode = 0;
	node->isSecondLevelNode = 0;
	node->c1 = 0;
	node->c2 = 0;
	node->marked = 0;
	return node;
}

Pair *createNewPair(char c, Node *node) {
	Pair *p = (Pair*)malloc(sizeof(Pair));
	p->c = c;
	p->ptr = node;
	return p;
}

Node *acTransit(Node *node, char c, Node *root)
{
	Node *res = acGetNextNode(node, c);
	while (res==NULL)
	{
		res = node->failure;
		if(res==root)
			return res;
		node = res;
		res = acGetNextNode(res, c);
	}
	return res;
}


Node *acGetNextNode(Node *node, char c) {
	Pair *pair;
	pair = (Pair*)(hashmap_get(node->gotos, (int)c));
	if (pair == NULL)
		return NULL;
	return pair->ptr;
}


void enter(ACTree *tree, char *pattern, int len, unsigned char *huffman, int huffman_length)
{
	Node *state = tree->root;
	int j = 0, p;
	Node *next, *newState;

	while ((next = acGetNextNode(state, pattern[j])) != NULL && j < len) {
		state = next;
		j++;
	}

	for (p = j; p < len; p++) {
		newState = createNewNode(tree, state);
		Pair *pair = createNewPair(pattern[p], newState);
		hashmap_put(state->gotos, pattern[p], pair);
		state->numGotos++;
		state = newState;
	}

	// Match
	state->match = 1;
	state->numMatches = 1;
	state->message = (char*)malloc(sizeof(char) * (len + 1));
	strncpy(state->message, pattern, len);
	state->message[len] = '\0';
	state->messageLength = len;
	state->huffman = (unsigned char*)malloc(sizeof(unsigned char) * (huffman_length/8)+1);
	memset(state->huffman, 0, sizeof(char) * (huffman_length/8)+1);
	memcpy(state->huffman, huffman, (huffman_length/8)+1);
	state->huffman_len = huffman_length;
}

void constructFailures(ACTree *tree) {
	NodeQueue q;
	Node *root, *state, *r, *s;
	Pair *pair;
	HashMap *map;
	char a;
	int toL0, toL1, toL2;

	toL0 = toL1 = toL2 = 0;

	nodequeue_init(&q);

	root = tree->root;
	root->failure = root;

	map = root->gotos;
	hashmap_iterator_reset(map);
	while ((pair = hashmap_iterator_next(map)) != NULL) {
		nodequeue_enqueue(&q, pair->ptr);
		pair->ptr->failure = root;
		toL0++;
	}

	while (!nodequeue_isempty(&q)) {
		r = nodequeue_dequeue(&q);
		map = r->gotos;
		hashmap_iterator_reset(map);
		while ((pair = hashmap_iterator_next(map)) != NULL) {
			a = pair->c;
			s = pair->ptr;
			nodequeue_enqueue(&q, s);
			if (r->failure == NULL)
				r->failure = root;
			state = r->failure;
			while (state->id != 0 && acGetNextNode(state, a) == NULL) {
				state = state->failure;
			}
			state = acGetNextNode(state, a);
			if (state == NULL) {
				state = root;
			}
			state->hasFailInto++;
			s->failure = state;

			if (state->depth == 1)
				toL1++;
			else if (state->depth == 2)
				toL2++;
			else if (state == root)
				toL0++;

		}
	}

	nodequeue_destroy_elements(&q, 1);
}

void findFails(Node *node, int *l0, int *l1, int *l2) {
	Pair *pair;

	if (node->failure->depth == 1)
		(*l1)++;
	else if (node->failure->depth == 2)
		(*l2)++;
	else if (node->failure->depth == 0)
		(*l0)++;

	hashmap_iterator_reset(node->gotos);
	while ((pair = hashmap_iterator_next(node->gotos))) {
		findFails(pair->ptr, l0, l1, l2);
	}
}

void getL2nums(ACTree *tree) {
	Node *root = tree->root;
	Pair *pair;
	int l0, l1, l2;

	l1 = root->numGotos;
	l2 = 0;

	hashmap_iterator_reset(root->gotos);
	while ((pair = hashmap_iterator_next(root->gotos))) {
		l2 += pair->ptr->numGotos;
	}

	l0 = l1 = l2 = 0;

	findFails(root, &l0, &l1, &l2);

}

char *clone_string(char *s, int len) {
	char *res;

	if (s == NULL) {
		return NULL;
	}

	res = (char*)malloc(sizeof(char) * (len + 1));

	memcpy(res, s, len);
	res[len] = '\0';

	return res;
}

//char *concat_strings(char *s1, char *s2, int len1, int len2, int freeS1, int freeS2, int *newLen) {
//	char *res;
//
//	if (s1 == NULL) {
//		*newLen = len2;
//		return clone_string(s2, len2);
//	}
//
//	if (s2 == NULL) {
//		*newLen = len1;
//		return clone_string(s1, len1);
//	}
//
//	res = (char*)malloc(sizeof(char) * (len1 + len2 + 2));
//	memcpy(res, s1, len1);
//	res[len1] = ';';
//	memcpy(&(res[len1+1]), s2, len2);
//	res[len1+len2+1] = '\0';
//
//	if (freeS1)
//		free(s1);
//	if (freeS2)
//		free(s2);
//
//	*newLen = len1 + len2 + 1;
//	return res;
//}

void FillPatternData(Node* node, char* pattern, unsigned char* hoffmanCode, int hoffmanCodeLen)
{
	if (hoffmanCodeLen == 0)
		return;

	node->patterns[node->numOfPatterns] = (char*)malloc(sizeof(char) * (strlen(pattern) + 1));
	strncpy(node->patterns[node->numOfPatterns], pattern,  strlen(pattern) );
	node->patterns[node->numOfPatterns][ strlen(pattern)] = '\0';

	int sizeOfHoffmanCode = (hoffmanCodeLen)/8 + 1;
	node->hoffmanCodes[node->numOfPatterns] = (unsigned char*)malloc(sizeof(unsigned char) * sizeOfHoffmanCode);
	memset(node->hoffmanCodes[node->numOfPatterns],0, sizeof(unsigned char) * sizeOfHoffmanCode);
	memcpy(node->hoffmanCodes[node->numOfPatterns], hoffmanCode, sizeOfHoffmanCode);
	node->hoffmanCodesLength[node->numOfPatterns] = hoffmanCodeLen;

	node->numOfPatterns++;
}

void avoidFailToAcceptingStatesRecursive(Node *state) {
	Pair *pair;
	int newLen;

	while (state->failure->match) {
		if (state->message != 0 && strchr(state->message,';') == NULL)
		{
			FillPatternData(state, state->message, state->huffman, state->huffman_len);
		}

		state->match = 1;
		//state->message = concat_strings(state->message, state->failure->message, state->messageLength, state->failure->messageLength, 1, 0, &newLen);
		//state->messageLength = newLen;
		int i;
        for(i=0;i<state->failure->numMatches;i++)
        {
//        	if (strcmp(state->failure->message,"pr")==0)
//        		printf("%s\n",state->failure->message);

        	//RB Check why huffman code can be null
        	FillPatternData(state, state->failure->message, state->failure->huffman, state->failure->huffman_len);
        	state->numMatches++;
        }

		if (state->failure->numGotos == 0) {
			if (state->failure->hasFailInto > 0) {
				state->failure->hasFailInto--;
			}
			state->failure = state->failure->failure;
		} else {
			break;
		}
	}

	hashmap_iterator_reset(state->gotos);
	while ((pair = hashmap_iterator_next(state->gotos)) != NULL) {
		avoidFailToAcceptingStatesRecursive(pair->ptr);
	}
}

void avoidFailToAcceptingStates(ACTree *tree) {
	avoidFailToAcceptingStatesRecursive(tree->root);
}

void printPair(void *data) {
	Pair *pair;
	pair = (Pair*)data;
	if (pair->c >= 32 && pair->c < 127) {
		printf("%c:%lu", pair->c, pair->ptr->id);
	} else {
		printf("%0X:%lu", (int)((pair->c) & 0x0FF), pair->ptr->id);
	}
}

void printNode(Node *node) {
	int i, val;
	printf("Node ID: %d, Depth: %d, Gotos: ", node->id, node->depth);
	hashmap_print(node->gotos, &printPair);
	printf(", Failure: %d, Match: %d", node->failure->id, node->match);
	if (node->match) {
		printf(" (message: ");//, node->message);
		for (i = 0; i < node->messageLength; i++) {
			if (node->message[i] >= 32 && node->message[i] < 127) {
				printf("%c", node->message[i]);
			}else {
				val = (int)((node->message[i]) & 0x0FF);
				printf("|");
				if (val < 16) {
					printf("0");
				}
				printf("%X|", val);
			}
		}
 		printf(" [length: %d])", node->messageLength);
 		int i;
 		for(i=0;i<node->numMatches;i++)
 		{
 			//printf("pattern %d: %s [huffman:%s-len:%d] * ", i, node->patterns[i], node->huffman[i], node->huffman_len[i] );
 		}
	}
	if (node->hasFailInto) {
		printf(" *Has %d fail(s) into it*", node->hasFailInto);
	}
	printf("\n");
}

void acPrintTree(ACTree *tree) {
	NodeQueue queue;
	Node *node;
	Pair *pair;
	int i;

	nodequeue_init(&queue);

	printf("Printing Aho-Corasick tree:\n");

	nodequeue_enqueue(&queue, tree->root);

	while (!nodequeue_isempty(&queue)) {
		node = nodequeue_dequeue(&queue);
		for (i = 0; i < node->depth; i++) {
			printf("-");
		}
		printNode(node);

		hashmap_iterator_reset(node->gotos);
		while ((pair = hashmap_iterator_next(node->gotos)) != NULL) {
			nodequeue_enqueue(&queue, pair->ptr);
		}
	}

	nodequeue_destroy_elements(&queue, 1);
}


#define MAX_PATTERNS -1

void acBuildTree(ACTree *tree, Anchor anchorsArr[], int avoidFailToLeaves, int mixID)
{
	tree->size = 0;
	tree->root = createNewNode(tree, NULL);
	int arrSize = sizeof(anchorsArr)/sizeof(anchorsArr[0]);
	int i;
	int count = 0;
	int count2 = 0;

	char buff[READ_BUFFER_SIZE];

	//Roman
	for(i=0; i< 4554;i++)
	{
		if(anchorsArr[i].dwSizeOfUrl==0)
		{
			break;
		}
		memcpy(buff, anchorsArr[i].cUrl,anchorsArr[i].dwSizeOfUrl);
		buff[anchorsArr[i].dwSizeOfUrl] = '\0';

#ifdef MAX_STATES_ACTIVE
		if (tree->size + anchorsArr[i].dwSizeOfUrl <= MAX_STATES) {
#endif

			enter(tree, buff, anchorsArr[i].dwSizeOfUrl,  anchorsArr[i].cHoffmanCode, anchorsArr[i].HoffmanCodeLength);
			count2++;
#ifdef MAX_STATES_ACTIVE
		}
#endif
		count++;
	}


	constructFailures(tree);

	getL2nums(tree);

	if (avoidFailToLeaves) {
		avoidFailToAcceptingStates(tree);
	}

}

void acBuildTreeASCII(ACTree *tree, Anchor anchorsArr[],int numOfAnchors, int avoidFailToLeaves, int mixID) {
	tree->size = 0;
	tree->root = createNewNode(tree, NULL);
	int i;
	int count = 0;
	char buff[READ_BUFFER_SIZE];

	for(i=0; i< numOfAnchors;i++)
	{
		if(anchorsArr[i].dwSizeOfUrl==0)
		{
			break;
		}
		memcpy(buff, anchorsArr[i].cUrl,anchorsArr[i].dwSizeOfUrl);
		buff[anchorsArr[i].dwSizeOfUrl] = '\0';

		enter(tree, buff, anchorsArr[i].dwSizeOfUrl,  anchorsArr[i].cHoffmanCode, anchorsArr[i].HoffmanCodeLength);
		count++;
	}

	constructFailures(tree);

	getL2nums(tree);

	if (avoidFailToLeaves)
	{
		avoidFailToAcceptingStates(tree);
	}
}

void acDestroyNodesRecursive(Node *node) {
	Pair *pair;
	Node *child;
	HashMap *map = node->gotos;
	hashmap_iterator_reset(map);

	while ((pair = (Pair*)hashmap_iterator_next(map)) != NULL) {
		child = pair->ptr;
		acDestroyNodesRecursive(child);
		free(pair);
	}
	if (node->message) {
		free(node->message);
	}
	int i;
	for(i=0;i<node->numMatches;i++)
	{
//		if(node->patterns[i]){
//			free(node->patterns[i]);
//		}
	}
	hashmap_destroy(map);
	free(node);
}

void acDestroyTreeNodes(ACTree *tree) {
	acDestroyNodesRecursive(tree->root);
}
