#pragma once

// Misc
bool processInput(struct LinkedList** deckList, struct SNode** htable, unsigned char htSize);
void exitF(struct LinkedList** deckList, struct SNode*** htable, unsigned char htSize);

// Adding
//bool io_add_deck(struct LinkedList* deckList, struct SNode** htable, unsigned char htSize, char* token, char delim[], char buffer[], unsigned short int* cards_count);
bool io_add_deck(struct LinkedList** deckList, struct SNode** htable, unsigned char htSize, char* token, char delim[], char buffer[], unsigned short int* cards_count);
int io_add_cards(struct LinkedList* deckList, char* token, char delim[], unsigned short* deck_idx, struct SNode** htable, unsigned char htSize);

// Deleting
int io_del_deck(struct LinkedList* deckList, char* token, unsigned short int* idx, char delim[]);
int io_del_card(struct LinkedList* deckList, char* token, char delim[] , unsigned short* deck_idx, unsigned short* card_idx);


// Printing
unsigned short io_deck_number(struct LinkedList* deckList);
int io_deck_len(struct LinkedList* deckList, unsigned short* deck_idx, char* token, char delim[]);
int io_show_deck(struct LinkedList* deckList, char* token, char delim[]);
void io_show_all(struct LinkedList* deckList);


// Operations
int io_merge_decks(struct LinkedList* deckList, unsigned short* deck_idx1, unsigned short* deck_idx2, char* token, char delim[]);
int io_shuffle_deck(struct LinkedList* deckList, unsigned short* deck_idx, char* token, char delim[]);
int io_split_deck(struct LinkedList* deckList, unsigned short* deck_idx, unsigned short* split_idx, char* token, char delim[]);
int io_reverse_deck(struct LinkedList* deckList, unsigned short* deck_idx, char* token, char delim[]);
int io_sort_deck(struct LinkedList* deckList, unsigned short* deck_idx, char* token, char delim[]);





