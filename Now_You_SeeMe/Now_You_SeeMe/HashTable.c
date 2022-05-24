#include "HashTable.h"




/*
* Hash Table Chaining implementation. It's purpose is to store association between Symbol value and Symbol name.
*/


// Add elements to htable[i] ( where htable[i] is a list )
struct SNode* push(struct SNode* list, Symbol s, const char* symbol_name)
{
	struct SNode* new_node = (struct SNode*)malloc(sizeof(struct SNode));
	new_node->s = s;
	new_node->next = list;
	new_node->symbol_name = (char*)malloc(strlen(symbol_name) + 1);
	strcpy(new_node->symbol_name, symbol_name);
	return new_node;
}


// Delete element from htable[i] ( where htable[i] is a list )
struct SNode* delete_Snode(struct SNode* list, char* symbol_name)
{
	struct SNode* tmp = list;
	if (tmp)
	{
		if (strcmp(tmp->symbol_name, symbol_name) == 0)
		{
			// The first node is the one we want to delete
			list = list->next;
			free(tmp->symbol_name);
			free(tmp);
		}
		else
		{
			if (tmp->next)
			{
				//  If there is more than 1 node in the list
				while (tmp->next && strcmp(tmp->next->symbol_name, symbol_name) != 0)
				{
					tmp = tmp->next;
				}
				// We either got out of the while loop because there are no more nodes either we found the node we were looking for
				if (tmp->next)
				{
					// We found the node we want to delete
					struct SNode* tmp2 = tmp->next;
					tmp->next = tmp2->next;
					free(tmp2->symbol_name);
					free(tmp2);
				}
			}
		}
	}
	return list;

}



unsigned short hashCalc(unsigned char htsize, const char* symbol_name)
{
	unsigned short sum = 0;
	for (unsigned short i = 0; i < strlen(symbol_name); i++)
		sum += symbol_name[i];
	return sum % htsize;
}

void HashTableInsert(struct SNode** htable, unsigned char htsize, Symbol key, const char* symbol_name)
{
	unsigned char pos = hashCalc(htsize, symbol_name);
	htable[pos] = push(htable[pos], key, symbol_name);
}



// Search through the Hash Table for a enum value given the enum name.
Symbol HtableSearchS(struct SNode** htable, unsigned char htsize, char* symbol_name)
{
	unsigned char pos = hashCalc(htsize, symbol_name);
	struct SNode* tmp = htable[pos];
	while (tmp)
	{
		if (strcmp(tmp->symbol_name, symbol_name) == 0)
			return tmp->s;
		tmp = tmp->next;
	}

	return ERROR;
}




void HtableDeleteSymbol(struct SNode** htable, unsigned char htsize, char* key)
{
	unsigned char pos = hashCalc(htsize, key);
	htable[pos] = delete_Snode(htable[pos], key);
}


void initializeHtable(struct SNode** htable, unsigned char htsize)
{
	HashTableInsert(htable, htsize, HEART, "HEART");
	HashTableInsert(htable, htsize, SPADE, "SPADE");
	HashTableInsert(htable, htsize, DIAMOND, "DIAMOND");
	HashTableInsert(htable, htsize, CLUB, "CLUB");
}