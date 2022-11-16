#include "Player.h"
//#include "DarkGDK.h"

Player::Player(void)
{
	pMeld = -1;
	meldBid = -1;
	runInSuit = 4;
	roundRobin = false;
	cardsInHand = 20;

	for (short int i = 0; i < 20; i++)
		this->hand[i] = 99;

	for (short int i = 0; i < 4; i++)
		for (short int j = 0; j < 5; j++)
			known[j][i] = -1;

	for (short int i = 0; i < 4; i++)
		trumpingSuit[i] = false;

	handPlaceholder = 0;
	numberOfBids = 0;
	mHearts = false;
	mClubs = false;
	mDiamonds = false;
	mSpades = false;
	saveBid = false;
	runInHearts = false;
	runInClubs = false;
	runInDiamonds = false;
	runInSpades = false;
	doubleRun = false;
	strongestRun = 4;
	strongSuit = 4;
	lastbid=0;
}

void Player::reset()
{
	lastbid=0;
	pMeld = -1;
	meldBid = -1;
	runInSuit = 4;
	roundRobin = false;
	cardsInHand = 20;

	for (short int i = 0; i < 20; i++)
		this->hand[i] = 99;

	for (short int i = 0; i < 4; i++)
		for (short int j = 0; j < 5; j++)
			known[j][i] = -1;

	for (short int i = 0; i < 4; i++)
		trumpingSuit[i] = false;

	handPlaceholder = 0;
	numberOfBids = 0;
	mHearts = false;
	mClubs = false;
	mDiamonds = false;
	mSpades = false;
	saveBid = false;
	runInHearts = false;
	runInClubs = false;
	runInDiamonds = false;
	runInSpades = false;
	doubleRun = false;
	strongestRun = 4;
	strongSuit = 4;
}

short int Player::getNumOfBids()
{
	return numberOfBids;
}

void Player::resetKnown()
{
	for (short int i = 0; i < 5; i++)
		for (short int j = 0; j < 4; j++)
			this->known[i][j] = -1;
}

bool Player::getSuitState(short int suit)
{
	return trumpingSuit[suit];
}

bool Player::getRoundRobin()
{
	return this->roundRobin;
}

void Player::toggleSaveBid()
{
	saveBid = !saveBid;
}

bool Player::getSaveBid()
{
	return this->saveBid;
}

void Player::incrementNumOfBids()
{
	this->numberOfBids++;
}

Player::~Player(void)
{
}

void Player::setMeldBid(unsigned short int value)
{
	this->meldBid = value;
}

short int Player::getMeldBid()
{
	if (meldBid < 0)
		return 0;

	return this->meldBid;
}

void Player::addToMeld(short int x)
{
	pMeld += x;
}

void Player::clearHand(void)
{
	for (short int i = 0; i < 20; i++)
		hand[i] = -1;

	this->handPlaceholder = 0;
}

void Player::clearMeld(void)
{
	pMeld = -1;
}

void Player::dealToHand(short int *card)
{
	for (short int i = 0; i < 4; i++)
	{
		this->hand[this->getHandPlaceholder()] = card[i];
		this->incrementHandPlaceholder();
	}
}

void Player::incrementHandPlaceholder()
{
	this->handPlaceholder++;
}

short int Player::getHandPlaceholder()
{
	return this->handPlaceholder;
}

bool Player::findCard(short int card)
{
	for (short int i = 0; i < 20; i++)
		if (hand[i] == card)
			return true;

	return false;
}

short int Player::getPMeld(void)
{
	return pMeld;
}

void Player::removeCardFromHand(short int card)
{
	hand[card] = 99;
	cardsInHand--;
}

short int Player::getCard(short int value)
{
	if (value < 0)
		return 99;
	else if (value > 19)
		return 99;

	return this->hand[value];
}

short int Player::getCardFromPlayerHand(short int rank, short int suit)
{
	for (short int i = 0; i < cardsInHand; i++)
	{
		if (this->hand[i] > 80)
			break;
		if (this->hand[i] / 20 == suit)
		{
			if (this->hand[i]%20/4 == rank)
				return i;
		}
	}

	return -1;
}

short int Player::getCardFromPlayerHand(short int cardNumber)
{
	for (short int i = 0; i < cardsInHand; i++)
	{
		if (hand[i] == cardNumber)
			return i;
	}

	return -1;
}


void Player::sortHand(void)
{
	bool done = false;

	while (!done)
    {
        done = true;

        for (short int i = 0; i < 19; i++)
        {
			//special case hack for first ace of hearts
			/*if (this->hand[i] > 79)
				this->hand[i] = 0;*/

			if (this->hand[i] > this->hand[i+1])
			{

				short int temp = this->hand[i];
				this->hand[i] = this->hand[i+1];
				this->hand[i+1] = temp;

				done = false;
			}

        }
    }
}

short int Player::getCardsInHand()
{
	return cardsInHand;
}

void Player::tabulateMeld()
{
	short int marriages = 0;
	short int pinochles = 0;
	short int jacks = 0;
	short int queens = 0;
	short int kings = 0;
	short int aces = 0;
	short int heartRun;
	short int clubRun;
	short int diamondRun;
	short int spadeRun;
	short int heartMarriage = 0;
	short int clubMarriage = 0;
	short int diamondMarriage = 0;
	short int spadeMarriage = 0;
	short int suitValue = 0;
	short int numberOfRuns = 0;
	//int player = this->getID();

	//check for marriages
	heartMarriage = this->getNumOfMarriages(HEARTS);
	clubMarriage = this->getNumOfMarriages(CLUBS);
	diamondMarriage = this->getNumOfMarriages(DIAMONDS);
	spadeMarriage = this->getNumOfMarriages(SPADES);
	marriages = heartMarriage + clubMarriage + diamondMarriage + spadeMarriage;

	if (this->getID() == SOUTH)
	{
		if (!marriages)
			marriages = 0;
			//agk::printc("GOT IT!");
	}

	if (heartMarriage && clubMarriage && diamondMarriage && spadeMarriage)
	{
		this->addToMeld(2);
		this->roundRobin = true;
	}

	//check for jacks
	jacks = this->getXAround(JACK);

	//check for pinochle
	pinochles = this->getNumOfPinochles();

	//check for queens
	queens = this->getXAround(QUEEN);

	//check for kings
	kings = this->getXAround(KING);

	//check for aces
	aces = this->getXAround(ACE);

	//check for runs
	heartRun = this->checkForRuns(HEARTS);
	clubRun = this->checkForRuns(CLUBS);
	diamondRun = this->checkForRuns(DIAMONDS);
	spadeRun = this->checkForRuns(SPADES);
	numberOfRuns = heartRun + clubRun + diamondRun + spadeRun;

	//determine strongest suit and strongest run
	if (marriages)
	{
		if (heartMarriage)
		{
			suitValue = this->getSuitValue(HEARTS);
			this->setStrongSuit(HEARTS);

			this->known[KING][HEARTS] = heartMarriage;
			this->known[QUEEN][HEARTS] = heartMarriage;
			if (heartRun)
				this->setStrongestRun(HEARTS);
		}
		if (clubMarriage)
		{
			if (suitValue < this->getSuitValue(CLUBS))
			{
				suitValue = this->getSuitValue(CLUBS);
				this->setStrongSuit(CLUBS);
				if (clubRun)
					this->setStrongestRun(CLUBS);
			}

			this->known[KING][CLUBS] = clubMarriage;
			this->known[QUEEN][CLUBS] = clubMarriage;
		}
		if (diamondMarriage)
		{
			if (suitValue < this->getSuitValue(DIAMONDS))
			{
				suitValue = this->getSuitValue(DIAMONDS);
				this->setStrongSuit(DIAMONDS);

				if (diamondRun)
					this->setStrongestRun(DIAMONDS);
			}

			this->known[KING][DIAMONDS] = diamondMarriage;
			this->known[QUEEN][DIAMONDS] = diamondMarriage;
		}
		if (spadeMarriage)
		{
			if (suitValue < this->getSuitValue(SPADES))
			{
				suitValue = this->getSuitValue(SPADES);

				this->setStrongSuit(SPADES);
				if (spadeRun)
					this->setStrongestRun(SPADES);
			}

			this->known[KING][SPADES] = spadeMarriage;
			this->known[QUEEN][SPADES] = spadeMarriage;

		}
	}

	this->addToMeld(1);

	this->addToMeld(marriages * 2);

	if (jacks == 1)
		this->addToMeld(4);
	else if (jacks == 2)
		this->addToMeld(40);

	if (jacks)
		for (short int i = 0; i < 4; i++)
		{
			this->known[JACK][i] = jacks;
		}

	if (queens == 1)
		this->addToMeld(6);
	else if (queens == 2)
		this->addToMeld(60);

	if (queens > heartMarriage)
		this->known[QUEEN][HEARTS] = queens;

	if (queens > clubMarriage)
		this->known[QUEEN][CLUBS] = queens;

	if (queens > diamondMarriage)
		this->known[QUEEN][DIAMONDS] = queens;

	if (queens > spadeMarriage)
		this->known[QUEEN][SPADES] = queens;

	if (queens)
	{
		this->known[KING][HEARTS] = heartMarriage;
		this->known[KING][CLUBS] = clubMarriage;
		this->known[KING][DIAMONDS] = diamondMarriage;
		this->known[KING][SPADES] = spadeMarriage;
	}

	if (kings == 1)
		this->addToMeld(8);
	else if (kings == 2)
		this->addToMeld(80);

	if (kings > heartMarriage)
		this->known[KING][HEARTS] = kings;

	if (kings > clubMarriage)
		this->known[KING][CLUBS] = kings;

	if (kings > diamondMarriage)
		this->known[KING][DIAMONDS] = kings;

	if (kings > spadeMarriage)
		this->known[KING][SPADES] = kings;

	if ((kings) && (queens < 2))
	{
		this->known[QUEEN][HEARTS] = heartMarriage;
		this->known[QUEEN][CLUBS] = clubMarriage;
		this->known[QUEEN][DIAMONDS] = diamondMarriage;
		this->known[QUEEN][SPADES] = spadeMarriage;
	}

	if (aces == 1)
		this->addToMeld(10);
	else if (aces == 2)
		this->addToMeld(100);

	if (aces)
	{
		for (short int i = 0; i < 4; i++)
		{
			this->known[ACE][i] = aces;
		}
	}

	if (pinochles == 1)
		this->addToMeld(4);
	else if (pinochles == 2)
		this->addToMeld(30);
	else if (pinochles == 3)
		this->addToMeld(90);
	else if (pinochles == 4)
		this->addToMeld(120);

	if (pinochles > queens)
	{
		if (pinochles > spadeMarriage)
			this->known[QUEEN][SPADES] = pinochles;
	}

	if (pinochles > jacks)
		this->known[JACK][DIAMONDS] = pinochles;

	if (!pinochles)
	{
		if (this->known[QUEEN][SPADES] > 0)
			this->known[JACK][DIAMONDS] = 0;

		if (this->known[JACK][DIAMONDS] > 0)
			this->known[QUEEN][SPADES] = 0;
	}

	if (pinochles)
	{
		if (this->known[KING][SPADES] < 0)
			this->known[KING][SPADES] = 0;
	}
}

/*void Player::tallyKnown(int number, int rank, int suit)
{
	int player = this->getID();

	if (suit == -1)
	{
		tallyKnown(number, rank, HEARTS);
		tallyKnown(number, rank, CLUBS);
		tallyKnown(number, rank, DIAMONDS);
		tallyKnown(number, rank, SPADES);
		return;
	}

	if (rank = -1)
	{
		switch (player)
		{
		case WEST:
			if (number > westKnown[ACE][suit])
				tallyKnown(number - westKnown[ACE][suit], ACE, suit);
			if (number > westKnown[TEN][suit])
				tallyKnown(number - westKnown[TEN][suit], TEN, suit);
			if (number > westKnown[KING][suit])
				tallyKnown(number - westKnown[KING][suit], KING, suit);
			if (number > westKnown[QUEEN][suit])
				tallyKnown(number - westKnown[QUEEN][suit], QUEEN, suit);
			if (number > westKnown[JACK][suit])
				tallyKnown(number - westKnown[JACK][suit], JACK, suit);
			break;
		case NORTH:
			if (number > northKnown[ACE][suit])
				tallyKnown(number - northKnown[ACE][suit], ACE, suit);
			if (number > northKnown[TEN][suit])
				tallyKnown(number - northKnown[TEN][suit], TEN, suit);
			if (number > northKnown[KING][suit])
				tallyKnown(number - northKnown[KING][suit], KING, suit);
			if (number > northKnown[QUEEN][suit])
				tallyKnown(number - northKnown[QUEEN][suit], QUEEN, suit);
			if (number > northKnown[JACK][suit])
				tallyKnown(number - northKnown[JACK][suit], JACK, suit);
			break;
		case EAST:
			if (number > eastKnown[ACE][suit])
				tallyKnown(number - eastKnown[ACE][suit], ACE, suit);
			if (number > eastKnown[TEN][suit])
				tallyKnown(number - eastKnown[TEN][suit], TEN, suit);
			if (number > eastKnown[KING][suit])
				tallyKnown(number - eastKnown[KING][suit], KING, suit);
			if (number > eastKnown[QUEEN][suit])
				tallyKnown(number - eastKnown[QUEEN][suit], QUEEN, suit);
			if (number > eastKnown[JACK][suit])
				tallyKnown(number - eastKnown[JACK][suit], JACK, suit);
			break;
		case SOUTH:
			if (number > southKnown[ACE][suit])
				tallyKnown(number - southKnown[ACE][suit], ACE, suit);
			if (number > southKnown[TEN][suit])
				tallyKnown(number - southKnown[TEN][suit], TEN, suit);
			if (number > southKnown[KING][suit])
				tallyKnown(number - southKnown[KING][suit], KING, suit);
			if (number > southKnown[QUEEN][suit])
				tallyKnown(number - southKnown[QUEEN][suit], QUEEN, suit);
			if (number > southKnown[JACK][suit])
				tallyKnown(number - southKnown[JACK][suit], JACK, suit);
			break;
		default:
			break;
		}
		return;
	}

	switch (player)
	{
	case WEST:
		westKnown[rank][suit] += number;
		break;
	case NORTH:
		northKnown[rank][suit] += number;
		break;
	case EAST:
		eastKnown[rank][suit] += number;
		break;
	case SOUTH:
		southKnown[rank][suit] += number;
		break;
	default:
		break;
	}

}*/

bool Player::getDoubleRun()
{
	return this->doubleRun;
}

void Player::setStrongestRun(short int suit)
{
	this->strongestRun = suit;
}

short int Player::getStrongestRun()
{
	return this->strongestRun;
}

short int Player::getNumOfRuns(void)
{
	short int count = 0;

	if (runInHearts)
		count++;
	if (runInClubs)
		count++;
	if (runInDiamonds)
		count++;
	if (runInSpades)
		count++;

	return count;
}

short int Player::getDoubleRunSuit(void)
{
	short int aces;
	short int tens;
	short int kings;
	short int queens;
	short int jacks;
	short int min;

	for (short int suit = 0; suit < 4; suit++)
	{
		aces = this->getNumOfCardsInRange(ACE, suit);
		tens = this->getNumOfCardsInRange(TEN, suit);
		kings = this->getNumOfCardsInRange(KING, suit);
		queens = this->getNumOfCardsInRange(QUEEN, suit);
		jacks = this->getNumOfCardsInRange(JACK, suit);

		min = this->minNumber(aces, tens, kings, queens);
		min = this->minNumber(min, jacks);

		if (min > 1)
			return suit;
	}

	return -1;
}

short int Player::checkForRuns(short int suit)
{
	short int aces;
	short int tens;
	short int kings;
	short int queens;
	short int jacks;
	short int min;

	aces = this->getNumOfCardsInRange(ACE, suit);
	tens = this->getNumOfCardsInRange(TEN, suit);
	kings = this->getNumOfCardsInRange(KING, suit);
	queens = this->getNumOfCardsInRange(QUEEN, suit);
	jacks = this->getNumOfCardsInRange(JACK, suit);

	min = this->minNumber(aces, tens, kings, queens);
	min = this->minNumber(min, jacks);

	if (min > 1)
		this->doubleRun = true;

	return min;
}

short int Player::getRunInSuit()
{
	return runInSuit;
}

short int Player::getRunSize(short int suit)
{
	short int total;

	if (suit == 4)
		return -100;

	total = this->getNumOfCardsInRange(ACE, suit) + this->getNumOfCardsInRange(TEN, suit) + this->getNumOfCardsInRange(KING, suit) + this->getNumOfCardsInRange(QUEEN, suit) + this->getNumOfCardsInRange(JACK, suit);

	return total;

}

void Player::setStrongSuit(short int suit)
{
	this->strongSuit = suit;
}

short int Player::getStrongSuit()
{
	return this->strongSuit;
}

bool Player::checkForMarriage(short int suit)
{
	//bool retValue = false;

	switch (suit)
	{
	case HEARTS:
		if (mHearts)
			return true;
		break;
	case CLUBS:
		if (mClubs)
			return true;
		break;
	case DIAMONDS:
		if (mDiamonds)
			return true;
		break;
	case SPADES:
		if (mSpades)
			return true;
		break;
	}

	return false;
}

short int Player::getSuitValue(short int suit)
{
	short int total;

	total = (this->getNumOfCardsInRange(ACE, suit) * 5) + (this->getNumOfCardsInRange(TEN, suit) * 4) + (this->getNumOfCardsInRange(KING, suit) * 3) + (this->getNumOfCardsInRange(QUEEN, suit) * 2) + this->getNumOfCardsInRange(JACK, suit);

	return total;
}

short int Player::getXAround(short int rank)
{
	short int hearts;
	short int clubs;
	short int diamonds;
	short int spades;

	hearts = this->getNumOfCardsInRange(rank, HEARTS);
	clubs = this->getNumOfCardsInRange(rank, CLUBS);
	diamonds = this->getNumOfCardsInRange(rank, DIAMONDS);
	spades = this->getNumOfCardsInRange(rank, SPADES);

	if (rank == ACE)
		numOfAces = hearts + clubs + diamonds + spades;

	return this->minNumber(hearts, clubs, diamonds, spades);
}


short int Player::getNumOfPinochles()
{
	short int queens;
	short int jacks;

	queens = this->getNumOfCardsInRange(QUEEN, SPADES);
	jacks = this->getNumOfCardsInRange(JACK, DIAMONDS);

	return this->minNumber(queens, jacks);
}

short int Player::getNumOfMarriages(short int suit)
{
	short int kings;
	short int queens;
	//check for marriages
	kings = this->getNumOfCardsInRange(2, suit);

	if (!kings)
		return 0;

	queens = this->getNumOfCardsInRange(3, suit);

	if (this->minNumber(kings, queens))
	{
		if (suit == HEARTS)
			this->mHearts = true;
		if (suit == CLUBS)
			this->mClubs = true;
		if (suit == DIAMONDS)
			this->mDiamonds = true;
		if (suit == SPADES)
			this->mSpades = true;
	}

	return this->minNumber(kings, queens);

//	if (kings < queens)
//		return kings;
//	else
//		return queens;
}

short int Player::minNumber(short int w, short int x)
{
	if (w < x)
		return w;
	else
		return x;
}

short int Player::minNumber(short int w, short int x, short int y, short int z)
{
	if (w < x)
		if (w < y)
			if (w < z)
				return w;
			else
				return z;
		else if (y < z)
			return y;
		else
			return z;
	else if (x < y)
		if (x < z)
			return x;
		else
			return z;
	else if (y < z)
		return y;
	else
		return z;
}

short int Player::getNumOfCardsInRange(short int rank, short int suit)
{
	short int count = 0;
	short int targetCardMin = (suit * 20) + (rank * 4);
	short int targetCardMax = targetCardMin + 4;
	short int sFlag = 0;

	if (rank == -1)
	{
		targetCardMin = (suit * 20);
		targetCardMax = ((suit + 1) * 20);
	}
	if (suit == -1)
	{
		suit = 0;
		targetCardMin = (rank * 4);
		sFlag = 1;
	}

	while (sFlag < 5)
	{
		if (sFlag)
		{
			targetCardMin = (rank * 4) + (20 * (sFlag - 1));
			targetCardMax = targetCardMin + 4;
		}

		for (short int i = 0; i < 20; i++)
			if (this->hand[i] >= targetCardMin)
			{
				if (this->hand[i] < targetCardMax)
					count++;
				else
					break;
			}
		if(sFlag > 0)
			sFlag++;
		else
			sFlag = 5;
	}

	return count;
}

short int Player::getKnownOfSuit(short int suit)
{
	short int count = -1;
	short int zeroCount = 0;

	for (short int i = ACE; i <= JACK; i++)
	{
		if (this->known[i][suit] > 0)
		{
			if (count < 0)
				count = this->known[i][suit];
			else
				count += this->known[i][suit];
		}
		else if (this->known[i][suit] == 0)
			zeroCount++;
	}

	if (zeroCount == 5)
		return 0;
	else
		return count;
}

short int Player::getID()
{
	return playerID;
}

void Player::setPlayerID(short int newID)
{
	this->playerID = newID;
}

short int Player::getNumOfAces()
{
	return this->numOfAces;
}
void Player::setlastbid(unsigned short int amount)
{
	lastbid = amount;
}
unsigned short Player::getlastbid()
{
	return lastbid;
}
