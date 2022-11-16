#pragma once

#include "Defines.h"

class Deck
{
public:
	Deck(void);
	~Deck(void);

	void deal(short int *deal);
	short int getCard(short int value);
	short int getDeckLocation();
	short int getDeckCounter();
	short int getRank(short int card);
	short int getSuit(short int card);
	void getTopFourCardsFromDeck(char[]);
	void incrementDeckLocation();
	void reset(bool randType);
	void setDeck(char deck[]);
	void setDeckCounter(short int counter);
	//void translateCard(short int card, char decodedCard[]);
private:
	void shuffleDeck(bool randType);
	short int currentDeck[80];
	short int deckLocation;
	short int deckCounter;
	short int handCounter;
};
