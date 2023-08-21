#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>


// Enum initialization
typedef enum Symbol{HEART, SPADE, DIAMOND, CLUB, ERROR} Symbol;
static char* enumStrings[] = { "HEART","SPADE","DIAMOND","CLUB", "ERROR"};



// Struct definitions
struct Node
{
	void* data;
	struct Node* next;
	struct Node* prev;
};
struct LinkedList
{
	struct Node* head;
	struct Node* tail;
	int size;
};
struct Card
{
	unsigned short int value;
	enum Symbol symbol;
};
struct Deck
{
	struct LinkedList* cardList;
};



// Getters and setters
unsigned short getValue(struct Card* const card);
bool setVal(struct Card* card, unsigned short value);
enum Symbol getSymbol(struct Card const* card);
bool setSymbol(struct Card* card, enum Symbol symbol);


// List operations
void DoublyListAppend(struct LinkedList** list, void* new_data, size_t data_size);
struct LinkedList  DoublyListInsert(struct LinkedList list, void* new_data, size_t data_size, unsigned short pos);
struct LinkedList DoublyListDelete(struct LinkedList list, unsigned short idx, void (*fptr)(void**));
struct LinkedList DoublyListDeleteByNode(struct LinkedList list, void** node, void (*fptr)(void**));
struct LinkedList ReverseDoublyList(struct LinkedList list);
void SortDeck(struct Deck** deck);
void* DoublyListSearch(struct LinkedList list, unsigned short idx);


// Printing
void printList(struct Node* node, void(*fptr)(void*));
void printCard(void* c);
void printDeck(void* dk, unsigned short deck_idx);
void printDeckList(void* dl);


// Deallocation
void deallocate(struct Node* node, void(*fptr)(void**));
void deallocateCard(void** card);
void deallocateDeck(void** dk);
void deallocateDeckList(void** dkList);