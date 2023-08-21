#include "Constants.h"
#include "magictricks.h"
#include "HashTable.h"
#include "errors.h"
#include "IO.h"
#include <stdlib.h>

#define BUFFER_SIZE 256


// ******************************************************************************** MISC ********************************************************************************


// This function will serve as a switch, calling the appropiate function given the input
//[in] - List of Decks (struct LinkedList** )
//[in] - Hash Table containing Symbols and their char* name association ( struct SNode** )
//[in] - Hash Table size ( unsigned char )
//[out] - boolean value indicating the state of success
bool processInput(struct LinkedList** deckList, struct SNode** htable, unsigned char htSize)
{
	char buffer[BUFFER_SIZE];
	char* token = NULL;
	char delim[] = " \n";

	while (1)
	{
		if (!fgets(buffer, sizeof(buffer), stdin)) // If reading not possible
		{
			printf("Invalid command.\n");
			break;
		}
		
		/*
		* Status codes: -1 => Card_Index_Out_of_bounds
		* 0 => Success
		* 1 => Deck_index_out_of_bounds
		*/

		token = strtok(buffer, delim);
		if (token != NULL)
		{
			if (strcmp(token, ADD_DECK) == 0)
			{
				unsigned short int cards_count = 0;
				bool created = io_add_deck(deckList, htable, htSize, token, delim, buffer, &cards_count);
				if (created)
					printf("The deck was successfully created with %hu cards.\n", cards_count);
			}
			else if (strcmp(token, DEL_DECK) == 0)
			{
				unsigned short int idx = 0;
				int code = io_del_deck(*deckList, token, &idx, delim);
				if (code == 0)
					printf("The deck %hu was successfully deleted.\n", idx);
				else if (code == 1)
					deck_index_out_of_bounds();
			}
			else  if (strcmp(token, DEL_CARD) == 0)
			{
				unsigned short deck_idx = 0;
				unsigned short card_idx = 0;
				int code = io_del_card(*deckList, token, delim, &deck_idx, &card_idx);
				if (code == 0)
					printf("The card was successfully deleted from deck %hu.\n", deck_idx);
				else if (code == 1)
					deck_index_out_of_bounds();
				else if (code == -1)
					card_index_out_of_bounds(deck_idx);
			}
			else if (strcmp(token, ADD_CARDS) == 0)
			{
				unsigned short deck_idx = 0;
				int code = io_add_cards(*deckList, token, delim, &deck_idx, htable, HASH_TABLE_SIZE);
				if (code == 0)
					printf("The cards were successfully added to deck %hu.\n", deck_idx);
				else if (code == 1)
					deck_index_out_of_bounds();
			}
			else if (strcmp(token, DECK_NUMBER) == 0)
			{
				token = strtok(NULL, delim);
				if (token)
				{
					invalid_command();
				}
				else
					io_deck_number(*deckList);
			}
			else if (strcmp(token, DECK_LEN) == 0)
			{
				unsigned short deck_idx = 0;
				int code = io_deck_len(*deckList, &deck_idx, token, delim);
				if (code == 1)
					deck_index_out_of_bounds();

			}
			else if (strcmp(token, SHUFFLE_DECK) == 0)
			{
				unsigned short deck_idx = 0;
				int code = io_shuffle_deck(*deckList, &deck_idx, token, delim);
				if (code == 0)
					printf("The deck %hu was successfully shuffled.\n", deck_idx);
				else if (code == 1)
					deck_index_out_of_bounds();
			}
			else if (strcmp(token, MERGE_DECKS) == 0)
			{
				unsigned short deck_idx1 = 0;
				unsigned short deck_idx2 = 0;
				int code = io_merge_decks(*deckList, &deck_idx1, &deck_idx2, token, delim);
				if (code == 0)
					printf("The deck %hu and the deck %hu were successfully merged.\n", deck_idx1, deck_idx2);
				else if (code == 1)
					deck_index_out_of_bounds();
			}
			else if (strcmp(token, SPLIT_DECK) == 0)
			{
				unsigned short deck_idx = 0;
				unsigned short split_idx = 0;
				int code = io_split_deck(*deckList, &deck_idx, &split_idx, token, delim);
				if (code == 0)
					printf("The deck %hu was successfully split by index %hu.\n", deck_idx, split_idx);
				else if (code == 1)
					deck_index_out_of_bounds();
				else if (code == -1)
					card_index_out_of_bounds(deck_idx);
			}
			else if (strcmp(token, REVERSE_DECK) == 0)
			{
				unsigned short deck_idx = 0;
				int code = io_reverse_deck(*deckList, &deck_idx, token, delim);
				if (code == 0)
				{
					printf("The deck %hu was successfully reversed.\n", deck_idx);
				}
				else if (code == 1)
					deck_index_out_of_bounds();
			}
			else if (strcmp(token, SORT_DECK) == 0)
			{
				unsigned short deck_idx = 0;
				int code = io_sort_deck(*deckList, &deck_idx, token, delim);
				if (code == 0)
					printf("The deck %hu was successfully sorted.\n", deck_idx);
				else if (code == 1)
					deck_index_out_of_bounds();
			}
			else if (strcmp(token, SHOW_DECK) == 0)
			{
				int code = io_show_deck(*deckList, token, delim);
				if (code == 1)
					deck_index_out_of_bounds();
			}
			else if (strcmp(token, SHOW_ALL) == 0)
			{
				token = strtok(NULL, delim);
				if (token)
				{
					invalid_command();
				}
				else
				{
					io_show_all(*deckList);
				}
			}

			else if (strcmp(token, EXIT) == 0)
			{
				exitF(deckList, &htable, HASH_TABLE_SIZE);
				break;
			}
			else invalid_command();
		}
	}
	return 0;

}


void exitF(struct LinkedList** deckList, struct SNode*** htable, unsigned char htSize)
{
	//Deallocating htable
	for (unsigned char i = 0; i < htSize; i++)
	{
		if (*htable)
		{
			while ((*htable)[i])
			{
				HtableDeleteSymbol(*htable, htSize, (*htable)[i]->symbol_name);
			}
		}
		
	}
	deallocateDeckList((void**)deckList);

}


//********************************************** Adding **********************************************

//[in] - deckList (doubly linked list holding all the decks )
//[in] - Hash Table ( Associating Symbols with Labels ) - struct SNode** (SNode => Simply Linked List Node )
//[in] - Hash Table Size - unsigned char
//[in] - token ( For tokenization purposes )
//[in] - Deck ( Where the cards will be added ) -> If successful, this deck will be added to Deck List
//[in] - Delimitators ( for Tokenization )
//[in] - Buffer for input purposes 
//[out] - *cards_count => How many cards are inserted to the Deck
// [return] - boolean value. True => success, False => failure to insert
bool io_add_deck(struct LinkedList** deckList, struct SNode** htable, unsigned char htSize, char* token, char delim[], char buffer[], unsigned short int* cards_count)
{
	struct Deck* deck = (struct Deck*)malloc(sizeof(struct Deck));
	if (!deck)
		return false;;
	deck->cardList = (struct LinkedList*)malloc(sizeof(struct LinkedList));
	if (!deck->cardList)
		return false;
	deck->cardList->head = NULL;
	deck->cardList->tail = NULL;
	deck->cardList->size = 0;
	token = strtok(NULL, delim);
	if (!token)
	{
		invalid_command();
		return false;
	}
	*cards_count = atoi(token);

	token = strtok(NULL, delim);
	if (token)
	{
		invalid_command();
		return false;
	}

	if (*cards_count == 0)
		return false;

	
	// Should follow now on the next cards_count lines the cards: Ex. 8 HEART
	int k = 0;
	while (k < *cards_count)
	{
		fgets(buffer, BUFFER_SIZE, stdin);
		token = strtok(buffer, delim);
		if (!token)
		{
			invalid_command();
			continue;
		}
		unsigned short int card_num = atoi(token);
		token = strtok(NULL, delim);
		if (!token)
		{
			invalid_command();
			continue;
		}
		char* card_name = token;
		struct Card card;
		bool insertedVal = setVal(&card, card_num);
		Symbol symb = HtableSearchS(htable, htSize, card_name);
		bool insertedSymb = setSymbol(&card, symb);
		if (!insertedSymb || !insertedVal)
		{
			invalid_card();
			continue;
		}
		DoublyListAppend(&(deck->cardList), &card, sizeof(struct Card));
		k++;
	}

	DoublyListAppend(deckList, deck, sizeof(struct Deck));


	free(deck);
	return true;
}



//[in] - deckList (doubly linked list holding all the decks )
//[in] - token ( For tokenization purposes )
//[in] - Delimitators ( for Tokenization )
//[out] - Deck Index=> The deck where cards will be inserted
//[in] - Hash Table ( Associating Symbols with Labels ) - struct SNode** (SNode => Simply Linked List Node )
//[in] - Hash Table Size - unsigned char
// [return] - int value denominating exit code. 
int io_add_cards(struct LinkedList* deckList, char* token, char delim[], unsigned short* deck_idx, struct SNode** htable, unsigned char htSize)
{
	token = strtok(NULL, delim);
	if (!token)
	{
		invalid_command();
		return 2;
	}
	*deck_idx = atoi(token);
	token = strtok(NULL, delim);
	if (!token)
	{
		invalid_command();
		return 2;
	}
	
	unsigned short nr_cards = atoi(token);
	

	token = strtok(NULL, delim);
	if (token)
	{
		invalid_command();
		return 2;
	}

	if (*deck_idx >= deckList->size)
		return 1;

	char buffer[BUFFER_SIZE];
	struct Deck* deck = (struct Deck*)DoublyListSearch(*deckList, *deck_idx);
	int k = 0;
	while (k < nr_cards)
	{
		Symbol s = ERROR;
		unsigned short val = 0;
		fgets(buffer, BUFFER_SIZE, stdin);
		token = strtok(buffer, delim);
		val = atoi(token);
		token = strtok(NULL, delim);
		if (!token)
		{
			invalid_card();
			continue;
		}
		char* sym_n = token;
		s = HtableSearchS(htable, htSize, sym_n);
		struct Card card;
		bool insertedVal = setVal(&card, val);
		if (!insertedVal)
		{
			invalid_card();
			continue;
		}
		bool insertedSymb = setSymbol(&card, s);
		if (!insertedSymb)
		{
			invalid_card();
			continue;
		}
		DoublyListAppend(&(deck->cardList), &card, sizeof(struct Deck));
		k++;

	}
	return 0;
}
// *********** END *********



// ********************************************** Deleting **********************************************

//[in] - deckList (doubly linked list holding all the decks )
//[in] - token ( For tokenization purposes )
//[out] - Deck index => The deck that will be deleted
//[in] - Delimitators ( for Tokenization )
// [return] - int value denominating exit code. 
int io_del_deck(struct LinkedList* deckList, char* token, unsigned short int* idx, char delim[])
{
	token = strtok(NULL, delim);
	if (!token)
	{
		invalid_command();
		return 2;
	}
	
	*idx = atoi(token);

	token = strtok(NULL, delim);
	if (token)
	{
		invalid_command();
		return 2;
	}
	// if idx not in range(size) then error
	if (*idx + 1 <= deckList->size)
	{
		*deckList = DoublyListDelete(*deckList, *idx, deallocateDeck);
		return 0;
	}
	return 1;

}


//[in] - deckList (doubly linked list holding all the decks );
//[in] - token ( For tokenization purposes );
//[in] - Delimitators ( for Tokenization );
//[out] - Deck index => The deck that we will delete cards from;
//[out] - Card index => The card that will be deleted from the deck;
// [return] - int value denominating exit code. 
int io_del_card(struct LinkedList* deckList, char* token, char delim[], unsigned short* deck_idx, unsigned short* card_idx)
{
	token = strtok(NULL, delim);
	if (!token)
	{
		invalid_command();
		return 2;
	}
	*deck_idx = atoi(token);
	token = strtok(NULL, delim);
	if (!token)
	{
		invalid_command();
		return 2;
	}
	
	*card_idx = atoi(token);
	if (*deck_idx >= deckList->size)
		return 1;

	token = strtok(NULL, delim);
	if (token)
	{
		invalid_command();
		return 2;
	}
	struct Deck* deck = (struct Deck*)DoublyListSearch(*deckList, *deck_idx); // Get the deckList[idx] deck
	if (!deck)
		return 1;
	if (*card_idx >= deck->cardList->size)
		return -1;
	*(deck->cardList) = DoublyListDelete(*(deck->cardList), *card_idx, deallocateCard);
	if (deck->cardList->size == 0)
	{
		*deckList = DoublyListDelete(*deckList, *deck_idx, deallocateDeck);
	}
	return 0;
}



// ********************************************** Printing **********************************************

// Prints the current number of decks in our Deck List.
unsigned short io_deck_number(struct LinkedList* deckList)
{
	printf("The number of decks is %hu.\n", deckList->size);
	return deckList->size;
}


// Prints the number of cards from a Deck.
//[in] - deckList (doubly linked list holding all the decks )
//[out] - Deck index => The deck that we will delete cards from
//[in] - token ( For tokenization purposes )
//[in] - Delimitators ( for Tokenization )
// [return] - int value denominating exit code. 
int io_deck_len(struct LinkedList* deckList, unsigned short* deck_idx,char* token, char delim[])
{
	token = strtok(NULL, delim);
	if (!token)
	{
		invalid_command();
		return 2;
	}
	*deck_idx = atoi(token);

	token = strtok(NULL, delim);
	if (token)
	{
		invalid_command();
		return 2;
	}
	struct Deck* deck = (struct Deck*)DoublyListSearch(*deckList, *deck_idx);
	if(!deck)
		return 1;
	printf("The length of deck %hu is %hu.\n", *deck_idx, deck->cardList->size);
	return 0;
	
}


// Prints all the cards within a Deck.
//[in] - deckList (doubly linked list holding all the decks )
//[in] - token ( For tokenization purposes )
//[in] - Delimitators ( for Tokenization )
// [return] - int value denominating exit code. 
int io_show_deck(struct LinkedList* deckList, char* token, char delim[])
{
	token = strtok(NULL, delim);
	if (!token)
	{
		invalid_command();
		return 2;
	}
	unsigned short deck_idx = atoi(token);

	token = strtok(NULL, delim);
	if (token)
	{
		invalid_command();
		return 2;
	}
	if (deck_idx > deckList->size)
		return 1;
	struct Deck* deck = (struct Deck*)DoublyListSearch(*deckList, deck_idx);
	if (!deck)
		return 1;
	printDeck(deck, deck_idx);
	return 0;
}

// Prints all the decks from the deck list.
void io_show_all(struct LinkedList* deckList)
{
	printDeckList(deckList);
}



// ********************************************** Operations **********************************************

// Merges two decks, forming a new deck and deletes the old ones.
//[in] - deckList (doubly linked list holding all the decks )
//[out] - Deck Index 1. The first Deck to extract cards from.
//[out] - Deck Index 2. The second Deck to extract cards from.
//[in] - token ( For tokenization purposes )
//[in] - Delimitators ( for Tokenization )
// [return] - int value denominating exit code. 
int io_merge_decks(struct LinkedList* deckList, unsigned short* deck_idx1, unsigned short* deck_idx2, char* token, char delim[])
{
	token = strtok(NULL, delim);

	// Check for bad input format
	if (!token)
	{
		invalid_command();
		return 2;
	}
	*deck_idx1 = atoi(token);


	if (!token)
	{
		invalid_command();
		return 2;
	}
	token = strtok(NULL, delim);
	*deck_idx2 = atoi(token);

	token = strtok(NULL, delim);
	if (token)
	{
		invalid_command();
		return 2;
	}


	// Find the two decks 
	struct Deck* deck1 = (struct Deck*)DoublyListSearch(*deckList, *deck_idx1);
	struct Deck* deck2 = (struct Deck*)DoublyListSearch(*deckList, *deck_idx2);
	if (!deck1 || !deck2)
		return 1;

	// Initialize the Deck holding both decks merged
	struct Deck* merged_deck = (struct Deck*)malloc(sizeof(struct Deck));
	merged_deck->cardList = (struct LinkedList*)malloc(sizeof(struct LinkedList));

	// If for some reason the memory allocation failed
	if (!merged_deck)
		return 2;

	merged_deck->cardList->head = NULL;
	merged_deck->cardList->tail = NULL;
	merged_deck->cardList->size = 0;

	// Start off with two helper pointers
	struct Node* tmp1 = deck1->cardList->head;
	struct Node* tmp2 = deck2->cardList->head;
	while (tmp1 && tmp2)
	{
		// While both decks have cards
		// Add 1 card from the first one and one card from the second one
		DoublyListAppend(&(merged_deck->cardList), tmp1->data, sizeof(struct Card));
		DoublyListAppend(&(merged_deck->cardList), tmp2->data, sizeof(struct Card));


		tmp1 = tmp1->next;
		tmp2 = tmp2->next;
	}
	if (tmp1)
	{
		// If deck2 has no more cards available, insert the rest from deck1
		while (tmp1)
		{
			DoublyListAppend(&(merged_deck->cardList), tmp1->data, sizeof(struct Card));
			tmp1 = tmp1->next;
		}
	}
	if (tmp2)
	{
		// The other way around ( see above )
		while (tmp2)
		{
			DoublyListAppend(&(merged_deck->cardList), tmp2->data, sizeof(struct Card));
			tmp2 = tmp2->next;
		}
	}

	// Add the fresh created deck 
	
	DoublyListAppend(&deckList, merged_deck, sizeof(struct Deck));
	free(merged_deck);
	free(merged_deck->cardList);
	// And delete the two used decks.
	*deckList = DoublyListDeleteByNode(*deckList, (void**)&deck1, deallocateDeck);
	*deckList = DoublyListDeleteByNode(*deckList, (void**)&deck2, deallocateDeck);


	
	
	// If it reached the end, then it means the merge was successful
	return 0;

}


// Shuffles a deck. Example: 1, 2, 3, 4, 5 => 3, 4, 5, 1, 2
//[in] - deckList (doubly linked list holding all the decks )
//[out] - Deck Index . The deck that will be shuffled.
//[in] - token ( For tokenization purposes )
//[in] - Delimitators ( for Tokenization )
// [return] - int value denominating exit code. 
int io_shuffle_deck(struct LinkedList* deckList, unsigned short* deck_idx, char* token, char delim[])
{
	token = strtok(NULL, delim);
	if (!token)
	{
		invalid_command();
		return 2;
	}
	
	*deck_idx = atoi(token);
	token = strtok(NULL, delim);
	if (token)
	{
		invalid_command();
		return 2;
	}
	struct Deck* deck = (struct Deck*)DoublyListSearch(*deckList, *deck_idx);
	if (!deck)
		return 1;
	unsigned short n1 = deck->cardList->size / 2;
	unsigned short n2 = 0;
	if (deck->cardList->size % 2 != 0)
		n2 = deck->cardList->size / 2 + 1;
	else
		n2 = deck->cardList->size / 2;
	struct Deck* new_deck = (struct Deck*)malloc(sizeof(struct Deck)); // Will insert the nodes in new order within this new Deck
	new_deck->cardList = (struct LinkedList*)malloc(sizeof(struct LinkedList));
	new_deck->cardList->head = NULL;
	new_deck->cardList->tail = NULL;
	new_deck->cardList->size = 0;
	struct Node* tmpTail = deck->cardList->tail;
	int k2 = n2 - 1;
	while (k2 > 0) // Assume that our list is: 1 -> 2 -> 3 -> 4 -> 5. 
	{
		// The purpose of this while is to go back until tmpTail is 3.
		tmpTail = tmpTail->prev;
		k2--;
	}
	while (tmpTail) // Then go up and insert 3 -> 4 -> 5
	{
		
		DoublyListAppend(&(new_deck->cardList), tmpTail->data, sizeof(struct Card));
		tmpTail = tmpTail->next;
	}
	int k1 = n1;
	struct Node* tmpHead = deck->cardList->head;
	while (k1 > 0) // Finally, insert 1 -> 2 at the end. We will get 3->4->5 + 1 -> 2
	{
		DoublyListAppend(&(new_deck->cardList), tmpHead->data, sizeof(struct Card));
		tmpHead = tmpHead->next;
		k1--;
	}
	deallocate(deck->cardList->head, deallocateCard);
	deck->cardList = new_deck->cardList;

	free(new_deck);
	return 0;
}



// Splits the deck by an index. (This operation will insert a new Deck overall, because splitting a deck results in two decks).
//[in] - deckList (doubly linked list holding all the decks )
//[out] - Deck Index . The deck that will be split.
//[in] - token ( For tokenization purposes )
//[in] - Delimitators ( for Tokenization )
// [return] - int value denominating exit code. 
int io_split_deck(struct LinkedList* deckList, unsigned short* deck_idx, unsigned short* split_idx, char* token, char delim[])
{
	token = strtok(NULL, delim);
	//Check for good input
	if (!token)
	{
		invalid_command();
		return 2;
	}
	*deck_idx = atoi(token);

	token = strtok(NULL, delim);
	if (!token)
	{
		invalid_command();
		return 2;
	}
	*split_idx = atoi(token);

	token = strtok(NULL, delim);
	if (token)
	{
		invalid_command();
		return 2;
	}

	struct Deck* deck = (struct Deck*)DoublyListSearch(*deckList, *deck_idx);

	// If deck not found
	if (!deck)
		return 1;

	// No changes in the deckList
	if (*split_idx == 0)
		return 0;

	

	// If there are less cards in the Deck than the given split index
	if (*split_idx >= deck->cardList->size)
		return -1;

	// The second part of the deck after the split will go in this Deck
	struct Deck* dd = (struct Deck*)malloc(sizeof(struct Deck));
	if (!dd)
		return 2;
	dd->cardList = (struct LinkedList*)malloc(sizeof(struct LinkedList));
	if (!dd->cardList)
		return 2;
	dd->cardList->head = NULL;
	dd->cardList->tail = NULL;
	dd->cardList->size = 0;


	struct Node* tmp = deck->cardList->head;
	unsigned short k = 0;

	// Those cards will remain in the current Deck
	while (k < *split_idx)
	{
		k++;
		tmp = tmp->next;
	}

	//The next cards will go to the new_deck
	while (tmp)
	{
		struct Node* tmp2 = tmp->next;
		DoublyListAppend(&(dd->cardList), tmp->data, sizeof(struct Card));
		*(deck->cardList) = DoublyListDeleteByNode(*(deck->cardList), &tmp->data, deallocateCard);
		tmp = tmp2;
	}

	*deckList = DoublyListInsert(*deckList, dd, sizeof(struct Deck), *deck_idx + 1);
	free(dd);
	return 0;
}


// Reverses a Deck. Example: 1->2->3 => 3->2->1
//[in] - deckList (doubly linked list holding all the decks )
//[out] - Deck Index . The deck that will be reversed.
//[in] - token ( For tokenization purposes )
//[in] - Delimitators ( for Tokenization )
// [return] - int value denominating exit code. 
int io_reverse_deck(struct LinkedList* deckList, unsigned short* deck_idx, char* token, char delim[])
{
	token = strtok(NULL, delim);
	//Check for good input
	if (!token)
	{
		invalid_command();
		return 2;
	}
	*deck_idx = atoi(token);
	token = strtok(NULL, delim);
	if (token)
	{
		invalid_command();
		return 2;
	}

	struct Deck* deck = (struct Deck*)DoublyListSearch(*deckList, *deck_idx);
	if (!deck)
		return 1;

	*(deck->cardList )= ReverseDoublyList(*(deck->cardList));
	return 0;

}


// Sorts a deck based by card value ( [1,14] ) or, in case they are equal, based on symbol ( HEART => the best, CLUB => the worst ). 
//[in] - deckList (doubly linked list holding all the decks )
//[out] - Deck Index . The deck that will be sorted.
//[in] - token ( For tokenization purposes )
//[in] - Delimitators ( for Tokenization )
// [return] - int value denominating exit code. 
int io_sort_deck(struct LinkedList* deckList, unsigned short* deck_idx, char* token, char delim[])
{
	token = strtok(NULL, delim);
	//Check for good input
	if (!token)
	{
		invalid_command();
		return 2;
	}
	*deck_idx = atoi(token);
	token = strtok(NULL, delim);
	if (token)
	{
		invalid_command();
		return 2;
	}
	struct Deck* deck = (struct Deck*)DoublyListSearch(*deckList, *deck_idx);

	// If deck not found
	if (!deck)
		return 1;
	SortDeck(&deck);
	return 0;
}










