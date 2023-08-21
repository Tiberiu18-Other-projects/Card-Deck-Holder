#include "magictricks.h"
#include "HashTable.h"



// ********************************************* Getters and setters ********************************************* 
unsigned short getValue(struct Card* const card)
{
	return card->value;
}

bool setVal(struct Card* card, unsigned short value)
{
	if (value >= 1 && value <= 14)
	{
		card->value = value;
		return true;
	}
	return false;
}

enum Symbol getSymbol(struct Card const* card)
{
	return card->symbol;
}

bool setSymbol(struct Card* card, enum Symbol symbol)
{
	if (symbol >= HEART && symbol <= CLUB)
	{
		card->symbol = symbol;
		return true;
	}
	return false;
}





// ********************************************* List operations ********************************************* 
void DoublyListAppend(struct LinkedList** list, void* new_data, size_t data_size)
{
	struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));

	if (!new_node)
		return;
	new_node->data = malloc(data_size);
	new_node->next = NULL;
	// Copy byte by byte data in the new node
	memcpy(new_node->data, new_data, data_size);
	if ((*list)->head == NULL)
	{
		(*list)->head = (*list)->tail = new_node;
		new_node->prev = NULL;
		(*list)->size = 1;
	}
	else
	{
		struct Node* tmp = (*list)->head;
		while (tmp->next)
		{
			tmp = tmp->next;
		}
		// Now we should be on the last position ( tail )
		new_node->prev = tmp;
		tmp->next = new_node;
		(*list)->tail = new_node;
		(*list)->size++;
	}

}


//[in] - pos => future position for the new node ( unsigned char ). Assume indexing starts from 0
struct LinkedList  DoublyListInsert(struct LinkedList list, void* new_data, size_t data_size, unsigned short pos)
{
	struct Node* tmp = list.head;
	struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
	new_node->data = malloc(data_size);
	memcpy(new_node->data, new_data, data_size);

	if (!tmp)
	{
		// List is empty
		list.head = list.tail = new_node;
		new_node->prev = NULL;
		new_node->next = NULL;
	}
	else
	{
		if (pos >= list.size)
		{
			// Insert at the end
			list.tail->next = new_node;
			new_node->next = NULL;
			new_node->prev = list.tail;
			list.tail = new_node;
		}
		else
		{
			if (pos == 0)
			{
				// Insert to the beginning
				new_node->next = list.head;
				new_node->prev = NULL;
				list.head->prev = new_node;
				list.head = new_node;
			}
			else
			{
				// General case. Inner insert
				unsigned short i = 0; 
				while (i < pos) // No need to check if tmp exists because we ruled out all possibilities above
				{
					i++;
					tmp = tmp->next; // Tmp o sa ajunga direct pe pozitia unde o sa fie new_node
				}
				tmp->prev->next = new_node;
				new_node->prev = tmp->prev;
				tmp->prev = new_node;
				new_node->next = tmp;
			}
		}
	}
	list.size++;
	return list;
}

struct LinkedList DoublyListDelete(struct LinkedList list, unsigned short idx, void (*fptr)(void** ) )
{
	struct Node* tmp = list.head;

	// We could have leveraged list.size attribute, but instead, for more practice with lists I decided not to
	if (tmp) // if list has at least one node
	{
		unsigned short int i = 0;
		while (tmp && i < idx)
		{
			i++;
			tmp = tmp->next;
		}
		if (tmp)
		{
			// If node on position idx exists; deallocate tmp
			if (!tmp->next && !tmp->prev)
			{
				// tmp is the only node from doubly list
				(*fptr)(&(tmp->data));
				free(tmp->data);
				free(tmp);
				list.head = list.tail = NULL;
			}
			else
			{
				if (!tmp->next)
				{
					// tmp is the last node on Doubly list
					list.tail = list.tail->prev;
					list.tail->next = NULL;
					(*fptr)(&(tmp->data));
					free(tmp->data);
					free(tmp);
				}
				else
				{
					if (!tmp->prev)
					{
						// tmp is the first node in doubly list
						list.head = list.head->next;
						list.head->prev = NULL;
						(*fptr)(&(tmp->data));
						free(tmp->data);
						free(tmp);
					}
					else
					{
						// General case: tmp is inner node
						struct Node* p, *q;
						q = tmp->next;
						p = tmp->prev;
						p->next = q;
						q->prev = p;
						(*fptr)(&(tmp->data));
						free(tmp->data);
						free(tmp);
					}
				}
			}
		}
	}
	if(list.size > 0)
		list.size--;
	return list;
}


// Deletes a DoublyList Node given the address
struct LinkedList DoublyListDeleteByNode(struct LinkedList list,void** p, void (*fptr)(void**))
{
	struct Node* tmp = list.head;

	// We could have leveraged list.size attribute, but instead, for more practice with lists I decided not to
	if (tmp) // if list has at least one node
	{
		while (tmp)
		{
			if (tmp->data == *p)
				break;
			tmp = tmp->next;
		}
		if (tmp)
		{
			// If node on position idx exists; deallocate tmp
			if (!tmp->next && !tmp->prev)
			{
				// tmp is the only node from doubly list
				(*fptr)(&(tmp->data));
				free(tmp->data);
				free(tmp);
				list.head = list.tail = NULL;
			}
			else
			{
				if (!tmp->next)
				{
					// tmp is the last node on Doubly list
					list.tail = list.tail->prev;
					list.tail->next = NULL;
					(*fptr)(&(tmp->data));
					free(tmp->data);
					free(tmp);
				}
				else
				{
					if (!tmp->prev)
					{
						// tmp is the first node in doubly list
						list.head = list.head->next;
						list.head->prev = NULL;
						(*fptr)(&(tmp->data));
						free(tmp->data);
						free(tmp);
					}
					else
					{
						// General case: tmp is inner node
						struct Node* p, * q;
						q = tmp->next;
						p = tmp->prev;
						p->next = q;
						q->prev = p;
						(*fptr)(&(tmp->data));
						free(tmp->data);
						free(tmp);
					}
				}
			}
		}
	}
	if (list.size > 0)
		list.size--;
	return list;
}
struct LinkedList ReverseDoublyList(struct LinkedList list)
{
	struct Node* current = list.head;
	if (current)
	{
		// If list has at least one node
		if (!current->prev && !current->next)
		{
			// tmp is the only node in the list
			return list;
		}
		else
		{
			struct Node* temp = NULL;
			while (current)
			{
				temp = current->next;
				current->next = current->prev;
				current->prev = temp;
				current = temp;
			}
			temp = list.head;
			list.head = list.tail;
			list.tail = temp;
		}
	}
	return list;
}


// [in] -> list - doubly linked list
// [in] -> index -> desired element placed on list[index] ( assuming list indexing starts from 0  )
// [out] -> struct Node* temp->data, which can be anything.
void* DoublyListSearch(struct LinkedList list, unsigned short idx)
{
	if (idx >= list.size)
		return NULL;
	else
	{
		struct Node* tmp = list.head; 
		unsigned short int i = 0;
		while (i < idx )
		{
			i++;
			tmp = tmp->next;
		}
		return tmp->data; // Note: We don't need to check if tmp is not null because we are leveraging list.size attribute
	}
}

void SortDeck(struct Deck** deck)
{
	int size = (*deck)->cardList->size;
	struct Node** v = (struct Node**)malloc(size* sizeof(struct Node*));
	struct Node* tmp = (*deck)->cardList->head;
	unsigned short k = 0;
	while (tmp)
	{
		v[k++] = tmp;
		tmp = tmp->next;
	}
	for (int i = 0; i < size - 1; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			struct Card* c1 = (struct Card*)v[i]->data;
			struct Card* c2 = (struct Card*)v[j]->data;
			if (c1->value > c2->value)
			{
				struct Node* aux = v[i];
				v[i] = v[j];
				v[j] = aux;
			}
			if (c1->value == c2->value)
			{
				if (c1->symbol > c2->symbol)
				{
					struct Node* aux = v[i];
					v[i] = v[j];
					v[j] = aux;
				}
			}
		}
	}
	for (int i = 0; i < size; i++)
	{
		DoublyListAppend(&((*deck)->cardList), v[i]->data, sizeof(struct Card));
	}
	for (int i = 0; i < size; i++)
	{
		*((*deck)->cardList) = DoublyListDelete(*((*deck)->cardList), 0, deallocateCard);
	}
	free(v);
}



// ********************************************* Printing ********************************************* 
void printList(struct Node* node, void(*fptr)(void*))
{
	while (node != NULL)
	{
		(*fptr)(node->data);
		node = node->next;
	}
}
void printCard(void* c)
{
	struct Card card = *(struct Card*)c;
	printf("\t%hu %s\n", card.value,enumStrings[card.symbol]);
}
// Deck = collection of cards
void printDeck(void* dk, unsigned short idx)
{
	printf("Deck %hu:\n", idx);
	struct Deck deck = *(struct Deck*)dk;
	struct Node* tmp = deck.cardList->head; // tmp->data is a card
	while (tmp)
	{
		printCard(tmp->data);
		tmp = tmp->next;
	}
}
void printDeckList(void* dl) {
	struct LinkedList deckList = *(struct LinkedList*)dl;
	struct Node* tmp = deckList.head;
	unsigned short k = 0;
	while (tmp)
	{
		printDeck(tmp->data, k);
		tmp = tmp->next;
		k++;
	}
}


// ********************************************* Deallocation ********************************************* 
void deallocate(struct Node* node, void(*fptr)(void**))
{
	while (node != NULL)
	{
		struct Node* tmp = node;
		node = node->next;
		(*fptr)(&(tmp->data));
		free(tmp->data);
		free(tmp);
		tmp = NULL;
	}
}


void deallocateCard(void** cd)
{
	// Have nothing to deallocate. The sole purpose of this function is to exist so I can use the above function with cards
	return;
}


void deallocateDeck(void** dk) // Getting a pointer to a Deck structure . Looks like this: originalPointer ----> deck <------- dk (if we have void* dk).
// Therefore, it's safe to assume that we need void** dk. 																														
{
	// In order to change the original Deck structure, we need to free up all deck internal cardList contens and then make deck Null. 
	struct Deck* deck = *(struct Deck**)dk;
	struct LinkedList* cardList = deck->cardList;
	struct Node* tmp;
	while (deck->cardList->head)
	{
		tmp = deck->cardList->head;
		deck->cardList->head = deck->cardList->head->next;
		deallocateCard(&(tmp->data));
		free(tmp->data); // tmp->data will always be a pointer to a Card
		free(tmp);
	}
	free(cardList);
	deck = NULL;
}

void deallocateDeckList(void** dkList)
{
	struct LinkedList* deckList = *(struct LinkedList**)dkList;
	struct Node* tmp;
	while (deckList->head)
	{
		tmp = deckList->head;
		deckList->head = deckList->head->next;
		deallocateDeck(&(tmp->data));
		free(tmp->data); //tmp->data will always be a pointer to a Deck
		free(tmp);
	}
	deckList = NULL;
}