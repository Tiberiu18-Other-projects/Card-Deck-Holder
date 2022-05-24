#include "errors.h"

void invalid_card()
{
	printf("The provided card is not a valid one.\n");
}

void deck_index_out_of_bounds()
{
	printf("The provided index is out of bounds for the deck list.\n");
}

void card_index_out_of_bounds(unsigned short deck_idx)
{
	printf("The provided index is out of bounds for deck %hu.\n", deck_idx);
}

void invalid_command()
{
	printf("Invalid command. Please try again.\n");
}