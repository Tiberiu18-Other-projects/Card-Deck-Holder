## Generic Linked List Implementation and Usage

### Summary:
Implemented Generic Linked List and a Chaining Hash Table from scratch, simulating card manipulation using console commands.
Available commands are: ADD_DECK <cards_count> => adds a deck to the deck list,
DEL_DECK <deck_index> => deletes a deck, 
DEL_CARD <deck_index> <card_index> => deletes a card from a deck,
ADD_CARDS <deck_index> <cards_count> -> adds cards to a deck, 
DECK_Number  => shows how many decks exists,
DECK_LEN <deck_index> => shows how many cards exists in a deck,
SHUFFLE_DECK <deck_index> => swaps the first and second half of a deck, 
MERGE_DECKS <deck_index1> <deck_index2> => merges the two decks resulting ONE deck and deleting the other two,
SPLIT_DECK <deck_index> <split_index> => splits a deck into two decks,
REVERSE_DECK <deck_index> => reverses cards order of a deck,
SHOW_DECK <deck_index> => shows all the cards of a deck,
SHOW_ALL => shows all the decks with all the cards,
SORT_DECK <deck_index> => sorts a deck based on cards value and symbol priority ( HEART, SPADE, DIAMOND, CLUB )
EXIT => exits the program

### Commentary:
This project gave me an insightful view on how templates works on higher-level languages 
and how C language processes data, more specifically it processes data the way the programmer wants it to
( meaning that at memory level, there are no such thing as integers, floats, strings, all data is represented the same,
the only thing that differs is how the compiler treats it ).
