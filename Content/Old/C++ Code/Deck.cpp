#include "template.h"

Deck::Deck(void)
{
	for (short int i = 0; i < 80; i++)
		currentDeck[i] = i;

	deckLocation = SOUTH;

	deckCounter = 0;
	handCounter = 0;
}

Deck::~Deck(void)
{
}

void Deck::reset(bool randType)
{
	for (short int i = 0; i < 80; i++)
		currentDeck[i] = i;

	shuffleDeck(randType);

	deckLocation = (deckLocation + 1) %4;

	deckCounter = 0;
	handCounter = 0;
}

void Deck::setDeck(char deck[])
{
	for (short int i = 0; i < 80; i++)
		currentDeck[i] = deck[i];
}

short int Deck::getDeckCounter(void)
{
	return handCounter-1;
}

void Deck::setDeckCounter(short int counter)
{
	handCounter = counter;
}

void Deck::shuffleDeck(bool randType)
{
	short int temp;
    short rand;
	//float debugSeed;

	//seed *= 100000.0f;

    //agk::Print(seed);
    //agk::Sync();
    //while(!agk::GetPointerPressed());

	//if (randType)
	//	agk::SetRandomSeed(handCounter++);
	//agk::SetRandomSeed((int)(seed));
	//optimized shuffle
	for (short int j  = 0; j < 14; j++)
		for(short int i = 79;i > 0; i--)
		{
            /*debugSeed = agk::Timer();
			short rand = ((int) (debugSeed * 100000.0f)) % i;*/
#if (OS != IOS)
			rand = agk::Random(0, i);
#else
            rand = arc4random() % (i+1);
#endif

			//swap
			temp = currentDeck[i];
			currentDeck[i] = currentDeck[rand];
			currentDeck[rand] = temp;
		}
}

//Unshuffled Deck Order goes A, A, A, A, 10, 10, 10, 10, K, K, etc.
/*void Deck::translateCard(short int card, char decodedCard[])
{
	short int test;
	short int suit;
	char tempString[20];

	test = (card - 1) / 4 % 5; // 0 = Ace, 1 = 10, 2 = K, 3 = Q, 4 = J
	suit = card / 20;  // 0 = Hearts, 1 = Clubs, 2 = Diamonds, 3 = Spades

	if (card == 99)
	{
		decodedCard = "No Card";
		return;
	}
	else if (test == ACE)
		strcpy(tempString, "Ace of ");
	else if (test == TEN)
		strcpy(tempString, "10 of ");
	else if (test == KING)
		strcpy(tempString, "King of ");
	else if (test == QUEEN)
		strcpy(tempString, "Queen of ");
	else
		strcpy(tempString, "Jack of ");

	if (suit == HEARTS)
		strcat(tempString, "Hearts");
	else if (suit == CLUBS)
		strcat(tempString, "Clubs");
	else if (suit == DIAMONDS)
		strcat(tempString, "Diamonds");
	else
		strcat(tempString, "Spades");

	strcpy(decodedCard, tempString);
}*/

short int Deck::getCard(short int value)
{
	return currentDeck[value];
}

void Deck::deal(short int *deal)
{
	for (short int i = 0; i < 4; i++)
		deal[i] = getCard(deckCounter++);
}

short int Deck::getDeckLocation()
{
	return deckLocation;
}

void Deck::incrementDeckLocation()
{
	deckLocation = (deckLocation + 1) % 4;
}

short int Deck::getSuit(short int card)
{
	return card/20;
}

short int Deck::getRank(short int card)
{
	return card%20/4;
}
