#pragma once
#include "magictricks.h"
#define HASH_TABLE_SIZE 20


struct SNode
{
	Symbol s;
	char* symbol_name;
	struct SNode* next;
};



struct SNode* push(struct SNode* list, Symbol s, const char* symbol_name);

struct SNode* delete_Snode(struct SNode* list, char* symbol_name);



unsigned short hashCalc(unsigned char htsize, const char* symbol_name);

void HashTableInsert(struct SNode** htable, unsigned char htsize, Symbol key, const char* symbol_name);



Symbol HtableSearchS(struct SNode** htable, unsigned char htsize, char* symbol_name);



void HtableDeleteSymbol(struct SNode** htable, unsigned char htsize, char* key);

void initializeHtable(struct SNode** htable, unsigned char htsize);

