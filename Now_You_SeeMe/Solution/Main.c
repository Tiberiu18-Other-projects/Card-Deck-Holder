#include <stdio.h>
#include "magictricks.h"
#include "HashTable.h"
#include "IO.h"


int main()
{
	struct LinkedList* deckList = (struct LinkedList*)malloc(sizeof(struct LinkedList));
	if (!deckList)
		return 1;
	deckList->head = NULL;
	deckList->tail = NULL;
	deckList->size = 0;
	struct SNode** htable;
	htable = (struct SNode**)malloc(HASH_TABLE_SIZE * sizeof(struct SNode*));
	if (!htable)
		return 1;
	for (int i = 0; i < HASH_TABLE_SIZE; i++)
		htable[i] = NULL;
	initializeHtable(htable, HASH_TABLE_SIZE);
	processInput(&deckList, htable, HASH_TABLE_SIZE);
	

	
	free(htable);
	free(deckList);
	return 0;
}