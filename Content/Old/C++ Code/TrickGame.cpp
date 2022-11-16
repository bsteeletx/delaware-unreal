#include "TrickGame.h"

TrickGame::TrickGame(void)
{
	trump = -1;
	lead = -1;
	trickCounter = 0;
	trickValue = 0;
	horizontalTricks = 0;
	verticalTricks = 0;
	tricksPlayed = 0;

	for (short int i = 0; i < 4; i++)
		for (short int j = 0; j < 5; j++)
			playedCards[j][i] = 0;
}

TrickGame::~TrickGame(void)
{
}

void TrickGame::reset()
{
	trump = -1;
	lead = -1;
	trickCounter = 0;
	trickValue = 0;
	horizontalTricks = 0;
	verticalTricks = 0;
	tricksPlayed = 0;

	for (short int i = 0; i < 4; i++)
		for (short int j = 0; j < 5; j++)
			playedCards[j][i] = 0;
}

void TrickGame::setLead(short int turn)
{
	lead = turn;
	bidWinner = turn;
}

short int TrickGame::getTrumpSuit()
{
	return trump;
}

void TrickGame::setTrumpSuit(short int suit)
{
	trump = suit;
}

short int TrickGame::getLead()
{
	return lead;
}

short int TrickGame::getTrickCounter()
{
	return trickCounter;
}

void TrickGame::setLeadSuit(short int suit)
{
	leadSuit = suit;
}

short int TrickGame::getLeadSuit()
{
	return leadSuit;
}

short int TrickGame::getNumOfCardsLeft(short int rank, short int suit)
{
	if (rank != -1)
		return 4 - playedCards[rank][suit];
	else
	{
		short int count = 0;

		for (short int i = 0; i < 5; i++)
		{
			count += 4 - playedCards[i][suit];
		}

		return count;
	}
}

void TrickGame::updateTrump(short int trumpPlayed)
{
	trumpLeft -= trumpPlayed;
}

short int TrickGame::getTopRank()
{
	return topRank;
}

short int TrickGame::getTopSuit()
{
	return topSuit;
}

void TrickGame::setTopRank(short int rank)
{
	topRank = rank;
}

void TrickGame::setTopSuit(short int suit)
{
	topSuit = suit;
}

short int TrickGame::getTricksPlayed()
{
	return tricksPlayed;
}

void TrickGame::playedCard(short int card, Player Selected)
{
	short int rank = (card % 20) / 4;
	short int suit = card / 20;

	if (trickCounter == 0)
	{
		leadSuit = suit;
		topSuit = suit;
		topRank = rank;
		lead = Selected.getID();
	}

	if (suit == topSuit)
	{
		if (rank < topRank)
		{//Aces are lower than Jacks
			topRank = rank;
			lead = Selected.getID();
		}
	}
	else if (suit == trump)
	{
		if (topSuit != trump)
		{
			topSuit = suit;
			topRank = rank;
			lead = Selected.getID();
		}
		else if (rank < topRank)
		{
			lead = Selected.getID();
			topRank = rank;
		}
	}

	playedCards[rank][suit]++;

	if (rank <= KING)
		trickValue++;

	trickCounter++;


	if (trickCounter == 4)
		tallyTrick();
}

void TrickGame::tallyTrick()
{
	if (getLead() % 2 == 0)
		horizontalTricks += trickValue;
	else
		verticalTricks += trickValue;

	trickValue = 0;
	trickCounter = 0;
	tricksPlayed++;
}

short int TrickGame::getHTricks()
{
	return horizontalTricks;
}

short int TrickGame::getVTricks()
{
	return verticalTricks;
}

void TrickGame::lastTrick(short int player)
{
	if (player % 2 == BLUE)
		horizontalTricks += 2;
	else
		verticalTricks += 2;
}

short int TrickGame::followLead(Player *Selected, short int rank, short int suit, bool lTrump, bool pTrump, short int count) //unless can take lead, just follow simple rules
{
	short int currentValue = 0;
	short int cardInHand = 0;
	short int bestValue = -999;
	short int bestCard = -1;
	bool takeTrick = false;
	bool noSuit = false;
	bool noTrump = false;
	short int suitA = -1;
	short int suitB = -1;
	short int suitC = -1;
	bool flagA = false;
	bool flagB = false;
	bool flagC = false;
	bool allTrue = false;
	short int newSuitA = -1;
	short int newSuitB = -1;
	short int newSuitC = -1;
	short int numFlags = 0;
	bool lastToPlay = false;
	//bool isTrumping = false;

	if (count % 4 == 3)
		lastToPlay = true;

	if (getTricksPlayed() == 19) //one card left, just play it
		return 0;

	if (Selected->getNumOfCardsInRange(-1, leadSuit) == 0)
		noSuit = true;
	if (Selected->getNumOfCardsInRange(-1, trump) == 0)
		noTrump = true;

	if (noSuit)
	{
		if(noTrump)
		{

			for (short int i = 0; i < 4; i++)
			{
				Selected->trumpingSuit[i] = false;

				if ((i == leadSuit) || (i == trump))
				{
					if (i != 3)
						continue;
				}

				if (suitA == -1)
				{
					suitA = 0;
					if (((lead % 2) == (Selected->getID() % 2)) || allTrue)
					{
						suitA += Selected->getNumOfCardsInRange(KING, i);
						suitA += Selected->getNumOfCardsInRange(TEN, i);
						if (allTrue)
							suitA += Selected->getNumOfCardsInRange(ACE, i);
						if (suitA < 1)
							flagA = true;
					}
					if ((suitA < 1) || ((lead % 2) != (Selected->getID() % 2)))
					{
						suitA += Selected->getNumOfCardsInRange(JACK, i);
						suitA += Selected->getNumOfCardsInRange(QUEEN, i);
						if ((suitA < 1) && ((lead % 2) != (Selected->getID() % 2)))
							flagA = true;
						else if (suitA > 0)
							flagA = false;
					}
					newSuitA = i;
					if (flagA)
						numFlags++;
				}
				else if (suitB == -1)
				{
					suitB = 0;
					if ((lead % 2) == (Selected->getID() % 2) || allTrue)
					{
						suitB += Selected->getNumOfCardsInRange(KING, i);
						suitB += Selected->getNumOfCardsInRange(TEN, i);
						if (allTrue)
							suitB += Selected->getNumOfCardsInRange(ACE, i);
						if (suitB < 1)
							flagB = true;
					}
					if ((suitB < 1) || ((lead % 2) != (Selected->getID() % 2)))
					{
						suitB += Selected->getNumOfCardsInRange(JACK, i);
						suitB += Selected->getNumOfCardsInRange(QUEEN, i);
						if ((suitB < 1) && ((lead % 2) != (Selected->getID() % 2)))
							flagB = true;
						else if (suitB > 0)
							flagB = false;
					}
					newSuitB = i;
					if (flagB)
						numFlags++;
				}
				else if (suitC == -1)
				{
					suitC = 0;
					if ((lead % 2) == (Selected->getID() % 2) || allTrue)
					{
						suitC += Selected->getNumOfCardsInRange(KING, i);
						suitC += Selected->getNumOfCardsInRange(TEN, i);
						if (allTrue)
							suitC += Selected->getNumOfCardsInRange(ACE, i);
						if (suitC < 1)
							flagC = true;
					}
					if((suitC < 1) || ((lead % 2) != (Selected->getID() % 2)))
					{
						suitC += Selected->getNumOfCardsInRange(JACK, i);
						suitC += Selected->getNumOfCardsInRange(QUEEN, i);
						if ((suitC < 1) && ((lead % 2) != (Selected->getID() % 2)))
							flagC = true;
						else if (suitC > 0)
							flagC = false;
					}
					newSuitC = i;
					if (flagC)
						numFlags++;
				}
				if ((suitA <= 0) && (suitB <= 0) && (i == 3))
				{
					if (((numFlags == 3) && flagA && flagB && flagC) || ((numFlags == 2) && flagA && flagB))
					{
						if ((suitC <= 0) && (flagC || (leadSuit != trump)))
						{
							allTrue = true;
							flagA = false;
							flagB = false;
							flagC = false;
							suitA = -1;
							suitB = -1;
							suitC = -1;
							i = -1;
							numFlags = 0;
						}
					}
				}
			}

			if (flagA || flagB || flagC)
			{//if any of these are true
				if (!(flagA && flagB && flagC) && (leadSuit != trump))
				{//if not ALL are true
					if (flagA)
						suitA = -1;
					if (flagB)
						suitB = -1;
					if (flagC)
						suitC = -1;
				}
			}

			if (suitB >= suitA)
			{
				if (suitC > suitB)
					suit = newSuitC;
				else
					suit = newSuitB;
			}
			else
			{
				if (suitC >= suitA)
					suit = newSuitC;
				else
					suit = newSuitA;
			}
			rank = ACE;
		}
		else //has trump
		{
			if (leadSuit != trump) //trumping
			{
				suit = trump;
				if (topSuit == leadSuit)
					rank = JACK;
				else
				{
					rank = topRank - 1;
					if (rank < ACE)
						rank = JACK;
				}

				for (short int i = rank; i >= ACE; i--)
				{
					if (Selected->getNumOfCardsInRange(i, trump))
					{
						return Selected->getCardFromPlayerHand(i, trump);
					}
				}

				for (short int i = JACK; i >= ACE; i--)
				{
					if (Selected->getNumOfCardsInRange(i, trump))
					{
						return Selected->getCardFromPlayerHand(i, trump);
					}
				}

			}
		}
	}
	//not trumping
	for (short int j = 0; j < 20; j++)
	{
		currentValue = 0;
		cardInHand = Selected->getCard(j);

		if (cardInHand / 20 != suit) //if it's not the correct suit
		{
			if (cardInHand / 20 > suit)  //if this suit is greater, we've already passed the suit, try again (later)
				break;
			else  //keep looking
				continue;
		}

		if (rank != ACE)  //if the lead is not an ACE, you HAVE to beat it
		{
			if (cardInHand%20/4 >= rank)
				break;
		}
		//at this poiont, either the lead was a card that cannot be beaten, or the current card beats the previous one

		currentValue = cardValues[cardInHand%20/4][cardInHand/20];

		if (suit == trump)
		{
			if ((cardInHand % 20 / 4 == ACE) && ((Selected->getNumOfCardsInRange(ACE, -1) - (Selected->getNumOfCardsInRange(ACE, trump)) < 2))) //let it pass
			{ //not enough aces to jump on the ace of trump
				if (Selected->getNumOfCardsInRange(-1, trump) - Selected->getNumOfCardsInRange(ACE, trump) > 1) //unless low on trump
					currentValue -= 12;
				if (Selected->getID() % 2 == bidWinner % 2) //if partner is bidWinner, then jump on it
				{
					if (Selected->getID() != bidWinner)
						currentValue += 12;
				}
			}
		}

		if ((rank == ACE) || (rank == JACK + 1))  //jack + 1 means that the player cannot beat the current lead
		{
			if (cardInHand%20/4 <= KING)
			{//are you giving the leader a point? or is your partner trumping?  (And the person to your left not trumping
				if (lead % 2 == Selected->getID() % 2)
					currentValue += 4;
				else if ((count % 4 == 1) && pTrump)
					currentValue += 2;
				else
					currentValue -= 2;

				if (lTrump && !lastToPlay)
					currentValue -= 2;



				if ((rank == ACE) && (cardInHand%20/4 == ACE))
				{
					if ((suit != trump) && (topSuit != trump))
					{
						if ((lead % 2 == Selected->getID() % 2) && (Selected->getNumOfCardsInRange(ACE, suit) == getNumOfCardsLeft(ACE, suit)))
						{
							if (Selected->getNumOfCardsInRange(ACE, suit) > 1)
							{
								if (Selected->getNumOfCardsInRange(TEN, suit))//ACE ON AN ACE!!
								{
									if (topRank == ACE)
										currentValue += 6;
								}
								else
									currentValue -= 6;
							}
							else
								currentValue -= 6;
						}
						else
							currentValue -= 6;
					}
					else
						currentValue -= 12;
				}
			}
			else
			{
				if (lead % 2 != Selected->getID() % 2)
					currentValue += 2;
				else
					currentValue -= 2;
			}
		}
		else if (cardInHand%20/4 < rank)
		{//beating the current lead
			if (!lTrump && !pTrump)
			{
				if (cardInHand%20/4 != ACE)
				{
					if (!lastToPlay)
					{
						for (short int k = cardInHand%20/4 - 1; k >= ACE; k--)
						{//check to see if what you currently are playing cannot be beaten
							if (getNumOfCardsLeft(k, suit))
								takeTrick = false;
							else
							{
								takeTrick = true;   //there is a case where you KNOW no one left can beat you
								break;
							}
						}
					}
					else
						takeTrick = true;
				}
				else
					takeTrick = true;
			}
		}

		if (takeTrick)
		{
			if (lastToPlay)
			{
				if (rank == QUEEN)
					currentValue += 28;
				else if (rank == KING)
					currentValue += 12;
				else if (rank == TEN)
					currentValue += 4;
			}

			currentValue = currentValue + 4;
		}

		if (currentValue >= bestValue)
		{
			bestValue = currentValue;
			bestCard = j;
		}

	}

	if (bestValue == -999)
		return (followLead(Selected, JACK+1, topSuit, lTrump, pTrump, count));
	else
		return (short int) bestCard;

}

short int TrickGame::playCard(Player *Selected, Player Left, Player Partner, Player Right, short int count, short int playerCard)
{
	short int cardPlayed;
	short int suit;
	short int rank;
	short int cardInHand;
	short int cardNumber;
	bool notOut = false;
	bool aceOnAce = false;

	if (playerCard == -1)
	{

		if (getTrickCounter() == 0)
		{
			cardInHand = startTrick(Selected, Left, Partner, Right);

			cardNumber = Selected->getCard(cardInHand);
			Selected->removeCardFromHand(cardInHand);
			rank = (cardNumber % 20) / 4;
			suit = cardNumber / 20;
		}
		else
		{
			suit = leadSuit;

			if (suit != topSuit)
				rank = ACE;
			else
				rank = topRank;

			cardPlayed = followLead(Selected, rank, suit, Left.trumpingSuit[suit], Partner.trumpingSuit[suit], count);
			cardNumber = Selected->getCard(cardPlayed);
			Selected->removeCardFromHand(cardPlayed);
			rank = (cardNumber % 20) / 4;
			suit = cardNumber/20;
		}
	}
	else
	{
		rank = (playerCard % 20) / 4;
		suit = playerCard / 20;
		cardNumber = playerCard;
		cardInHand = Selected->getCardFromPlayerHand(cardNumber);
		Selected->removeCardFromHand(cardInHand);
	}

	if ((Selected->getID() != lead) && (topRank == ACE) && (suit == topSuit)) //Lead was an ACE
	{
		if ((lead % 2) == (Selected->getID() % 2)) //partner
		{
			if (rank == TEN)
				Selected->known[KING][suit] = 0;
			else if (rank == QUEEN)
			{
				Selected->known[JACK][suit] = 0;
				Selected->known[TEN][suit] = 0;
				Selected->known[KING][suit] = 0;
			}
			else if (rank == JACK)
			{
				Selected->known[TEN][suit] = 0;
				Selected->known[KING][suit] = 0;
			}

		}
		else //opponent
		{
			if (rank == QUEEN)
				Selected->known[JACK][suit] = 0;
			else if (rank == KING)
			{
				Selected->known[JACK][suit] = 0;
				Selected->known[QUEEN][suit] = 0;
			}
			else if (rank == TEN)
			{
				Selected->known[KING][suit] = 0;
				Selected->known[QUEEN][suit] = 0;
				Selected->known[JACK][suit] = 0;
			}
		}

		if ((rank == ACE) && (topRank == ACE))
		{
			aceOnAce = true;

			for (short int i = JACK; i >= TEN; i--)
			{
				if (Selected->known[i][suit] > 0)
				{
					notOut = true;
					break;
				}
			}

			if (!notOut)
			{
				Selected->known[TEN][suit] = 0;
				Selected->known[KING][suit] = 0;
				Selected->known[QUEEN][suit] = 0;
				Selected->known[JACK][suit] = 0;
			}

			if(leadSuit != trump)
				Selected->trumpingSuit[leadSuit] = true;  //even if he's not out, have to convince partner to send him a K
		}
	}
	else if ((Selected->getID() != lead) && (topRank != ACE) && (suit == topSuit)) //lead was not an ace, and player couldn't play an Ace
	{
		if (rank < topRank)
		{//beat top rank
			for (short int i = rank+1; i < topRank; i++)
			{
				Selected->known[i][suit] = 0;
			}
			if ((leadSuit != trump) && (suit == trump))
				Selected->trumpingSuit[leadSuit] = true;
		}
		else if (rank >= topRank) //couldn't beat it
		{
			for (short int i = topRank-1; i >= ACE; i--)
			{
				Selected->known[i][suit] = 0;
			}
		}

		if ((leadSuit != trump) && (suit == trump))
			Selected->trumpingSuit[leadSuit] = true;

	}
	else if ((getTrickCounter() != 0) && (suit != leadSuit))  //trumped
	{
		for (short int i = ACE; i <= JACK; i++)
		{
			Selected->known[i][leadSuit] = 0;
		}
		if (leadSuit != trump)
		{
			if (Selected->getNumOfCardsInRange(-1, trump) > 0)
				Selected->trumpingSuit[leadSuit] = true;
			else if (suit == trump) //JUST played last trump, so no one knows that he's out
				Selected->trumpingSuit[leadSuit] = true;

		}

	}

	playedCard(cardNumber, *Selected);

	if (getNumOfCardsLeft(rank, suit) == 0)
		Selected->known[rank][suit] = 0;
	else if (Selected->known[rank][suit] > 1)
		Selected->known[rank][suit]--;
	else
		Selected->known[rank][suit] = -1;

	if (Selected->trumpingSuit[suit] && !aceOnAce)
		Selected->trumpingSuit[suit] = false;

	return cardNumber;
}

short int TrickGame::findRank(short int value, bool trumped)
//Jacks = 0, Queens, 1, Kings 2, Tens 3, Aces 4
{
	switch (value)
	{
	case 0:
		return JACK;
		break;
	case 1:
		return QUEEN;
		break;
	case 2:
		return KING;
		break;
	case 3:
		return TEN;
		break;
	case 4:
		return ACE;
		break;
	default:
		return -1;
		break;
	}

}


short int TrickGame::potentialTrickValue(Player *Selected, Player Left, Player Partner, Player Right, short int rank, short int suit)
{
	short int playValue = 0;
	short int totalValue = 0;
	short int topCardPlay = 8 - cardValues[rank][suit];
	bool lostTrick = false;
	bool trumped = false;
	bool newTrump = false;
	short int trumpRank = JACK+1;
	short int tempTrumpRank = JACK+1;
	short int points = 0;

	playValue = 8 - potPlays(Left, Right, Selected, Partner, rank, suit, &trumped, &trumpRank, lostTrick, 0);
	totalValue = playValue;

	if (trumped)  //trumped to take trick
	{
		lostTrick = true;
		topCardPlay = playValue;
		trumpRank = findRank(playValue, trumped);
		rank = JACK+1;
		tempTrumpRank = trumpRank; //for use later
		newTrump = true;
	}
	else if (playValue > topCardPlay) //played better card than lead
	{
		lostTrick = true;
		topCardPlay = playValue;
		rank = findRank(playValue, trumped);
	}

	if (findRank(playValue, trumped) <= KING)
		points += 2;

	playValue = 8 - potPlays(Partner, Left, Selected, Right, rank, suit, &trumped, &trumpRank, lostTrick, 0);
	totalValue += playValue;

	if ((trumped) && (trumpRank > tempTrumpRank))  //overtrumped or trumped to take lead
	{
		lostTrick = false;
		topCardPlay = playValue;
		trumpRank = findRank(playValue, trumped);
		tempTrumpRank = trumpRank; //for later use
		rank = JACK+1;
		newTrump = true;
	}
	else if ((!trumped) && (playValue > topCardPlay)) //played a better card than previous or lead
	{
		if (lostTrick)
			lostTrick = false;

		topCardPlay = playValue;
		rank = findRank(playValue, newTrump);
	}

	if (findRank(playValue, newTrump) <= KING)
		points += 2;

	newTrump = false;
	playValue = 8 - potPlays(Right, Left, Selected, Partner, rank, suit, &trumped, &trumpRank, lostTrick, 0);
	totalValue += playValue;

	if ((trumped) && (trumpRank > tempTrumpRank)) //overtrumped or trumped to take lead
	{
		lostTrick = true;
		newTrump = true;
	}
	else if ((!trumped) && (playValue > topCardPlay)) //played a better card than previous or lead
		lostTrick = true;

	if (findRank(playValue, newTrump) <= KING)
		points += 2;

	if (lostTrick)
		return -(totalValue + points);
	else
		return totalValue + points;
}

short int TrickGame::potPlays(Player Selected, Player SelectedP, Player *Starter, Player StarterP, short int rank, short int suit, bool *trumped, short int *trumpRank, bool lostTrick, short int loopType)
{
	short int startRank = JACK;
	short int endRank = ACE;
	short int numOfCardsLeft = 0;

	if ((Selected.getSuitState(suit)) && (loopType != 2)) //if Selected is trumping suit, loopType == 2 means new trump
	{
		rank = *trumpRank;
		suit = trump;
		*trumped = true;
	}
	else if ((rank > ACE) && !loopType && !(*trumped)) //not trumping, Ace not led, first time through, not currently trumped
	{
		if ((!SelectedP.trumpingSuit[suit]) && (!Starter->trumpingSuit[suit]) && (!StarterP.trumpingSuit[suit]))
		{ // no one is trumping that you know of
			if (Selected.known[ACE][suit] > 0)  //if he has it, he'll take it
				return cardValues[ACE][suit];

			if (((Starter->getID() + 1) % 4) == Selected.getID()) //Selected is to the Left of Starter
			{
				short int temp = 0;
				temp += Starter->getNumOfCardsInRange(ACE, suit);  //counting how many known Ace's are out there
				if (StarterP.known[ACE][suit] > 0)
					temp += StarterP.known[ACE][suit];  //continuing the count
				if (SelectedP.known[ACE][suit] > 0)
					temp += SelectedP.known[ACE][suit]; //finalizing the count

				if (getNumOfCardsLeft(ACE, suit) - temp > 0) //checking to see if they're not all accounted for
					return cardValues[ACE][suit];  //one must assume that it'll get played
				else if (getNumOfCardsLeft(ACE, suit) == SelectedP.known[ACE][suit]) //Selected's Partner will take trick
				{
					if (rank > KING)
						startRank = KING;
					else
						startRank = rank;
				}
			}
			else //selected's P has already played, just play lowest card possible
				startRank = rank;
		}
		else //someone is trumping it, find out who
		{
			if (SelectedP.trumpingSuit[suit])
				startRank = KING;
			else
				startRank = JACK;
		}
		endRank = ACE;
	}
	else if ((rank == ACE) && (loopType != 2) && !(*trumped))
	{//Ace was played, not a new trump, not currently trumped
		if (Selected.getID() % 2 == lead % 2)
		{//if partner of the current leader
			if (loopType != 1 && !lostTrick)
			{//if he hasn't already gone through this section, and they haven't already lost the trick
				startRank = KING;
				endRank = TEN;
			}
			else
			{//either don't have any Kings or Tens, OR already lost the trick
				startRank = JACK;
				endRank = rank;
			}
		}
		else if (!lostTrick)
		{//not partner of current leader, and leader hasn't lost the trick
			startRank = JACK;
			endRank = rank;
		}
		else
		{//not partner of current leader, and leader HAS lost the trick
			startRank = KING;
			endRank = rank;
		}
	}

	for(short int i = startRank; i >= endRank; i--) //play lowest card in range
	{
		if (Selected.known[i][suit] == 0)//already know Selected does not have this card
			continue;

		short int temp = 0;

		if (Selected.known[i][suit] > 0) //already know Selected DOES have this card
			return cardValues[i][suit];

		if (SelectedP.known[i][suit] > 0)  //counting how many cards are out there
			temp += SelectedP.known[i][suit];

		if (StarterP.known[i][suit] > 0)
			temp += StarterP.known[i][suit];

		temp += Starter->getNumOfCardsInRange(i, suit);

		numOfCardsLeft = getNumOfCardsLeft(i, suit) - temp;

		if (numOfCardsLeft > 0)
			return cardValues[i][suit];
	}

	if (loopType == 2)
		return 0;
	else if (startRank == KING)
		return potPlays(Selected, SelectedP, Starter, StarterP, rank, suit, trumped, trumpRank, lostTrick, 1); //try from the J
	else if (startRank == JACK)
		return potPlays(Selected, SelectedP, Starter, StarterP, *trumpRank, trump, trumped, trumpRank, lostTrick, 2); //has to trump
	else
		return 0;
}

short int TrickGame::startTrick(Player *Selected, Player Left, Player Partner, Player Right)
{
	short int currentLook;
	short int currentLookValue;
	//int points = 0;
	short int bestValue = -999;
	short int cardToPlay = -1;
	//int suitState = -1;
	short int suit;
	short int rank;
	short int largeSuit = -1;
	short int smallSuit = -1;
	short int lgSuit = -1;
	short int smSuit = 99;
	short int total = 0;
	bool LOutOfTrump = false;
	bool POutOfTrump = false;
	bool ROutOfTrump = false;
	bool endOfSequence = false;
	bool zeroFlag = false;
	short int unknowns = 0;

	if (!tricksPlayed)
		setLead(Selected->getID());

	for (short int i = HEARTS; i <= SPADES; i++)
	{
		short int temp;

		if (i == trump)
			continue;

		temp = Selected->getNumOfCardsInRange(-1, i);
		if (temp > lgSuit)
		{
			largeSuit = i;
			lgSuit = temp;
		}
		else if (temp < smSuit)
		{
			smallSuit = i;
			smSuit = temp;
		}
	}

	for(short int i = 0; i < 20; i++)
	{
		currentLook = Selected->getCard(i);

		if (currentLook > 80)
			break;

		rank = (currentLook % 20) / 4;
		suit = currentLook / 20;

		total = 0;
		unknowns = 0;
		zeroFlag = false;

		if (Left.getKnownOfSuit(trump) == 0)
			LOutOfTrump = true;
		else
			LOutOfTrump = false;

		if (Partner.getKnownOfSuit(trump) == 0)
			POutOfTrump = true;
		else
			POutOfTrump = false;

		if (Right.getKnownOfSuit(trump) == 0)
			ROutOfTrump = true;
		else
			ROutOfTrump = false;

		for (short int j = 0; j <= JACK; j++)
			total += playedCards[j][suit];

	/*	if (suit != trump)
		{
			int left = 0;
			int partner = 0;
			int right = 0;

			left = Left.getKnownOfSuit(suit);
			partner = Partner.getKnownOfSuit(suit);
			right = Right.getKnownOfSuit(suit);

			if (left == 0 && !LOutOfTrump)
				zeroFlag = true;
			else if (left > 0)
				total += left;
			else
				unknowns++;

			if (partner > 0)
				total += partner;
			else if (partner < 0)
				unknowns++;

			if (right == 0 && !ROutOfTrump)
				zeroFlag = true;
			else if (right > 0)
				total += right;
			else
				unknowns++;

			for (int k = 0; k < 5; k++)
			{
				total += playedCards[k][suit];
			}
		}*/

		if (LOutOfTrump) //if out of trump, frees up a possibility
			total--;

		if (POutOfTrump)
			total--;

		if (ROutOfTrump)
			total--;

		currentLookValue = 0;

		if (Partner.trumpingSuit[suit] && !((Left.trumpingSuit[suit]) || (Right.trumpingSuit[suit])))
		{
			if (rank == KING)
				currentLookValue += 512;
			else if (rank == TEN)
				currentLookValue += 256;
			else if (rank == ACE)
				currentLookValue -= 8;
			else if (rank == QUEEN)
				currentLookValue += 128;
			else
				currentLookValue += 64;
		}
		else if (Left.trumpingSuit[suit] || Right.trumpingSuit[suit])
		{
			if (Left.trumpingSuit[suit] != Right.trumpingSuit[suit])
			{
				if (rank == KING)
					currentLookValue -= 64;
				else if (rank == TEN)
					currentLookValue -= 128;
				else if (rank == ACE)
					currentLookValue -= 256;
				else
					currentLookValue -= 32;
			}
		}

		if (zeroFlag || (((20 - total) / 3) < 1))
		{
			if (rank == KING)
				currentLookValue -= 64;
			else if (rank == TEN)
				currentLookValue -= 128;
			else if (rank == ACE)
				currentLookValue -= 256;
			else if (rank == QUEEN)
				currentLookValue -= 32;
			else
				currentLookValue -= 16;
		}

		currentLookValue -= cardValues[rank][suit];

		currentLookValue += potentialTrickValue(Selected, Left, Partner, Right, rank, suit);

		if (suit == largeSuit)
		{
			if (rank <= TEN)
				currentLookValue -= 8;
			else if (rank >= QUEEN)
				currentLookValue += 8;
		}
		else if (suit == smallSuit)
		{
			if (rank <= TEN)
				currentLookValue += 8;
			else if (rank >= QUEEN)
				currentLookValue -= 8;
		}

		if ((rank == KING) && ((getNumOfCardsLeft(TEN, suit) || (getNumOfCardsLeft(ACE, suit)))))
			currentLookValue -= 2;
		else if ((rank == TEN) && (getNumOfCardsLeft(ACE, suit)))
			currentLookValue -= 4;

		if ((suit != trump) && ((!Left.trumpingSuit[suit]) || (!Partner.trumpingSuit[suit]) || (!Right.trumpingSuit[suit])))
		{
			if (rank == ACE)
				currentLookValue += 1;
			else if ((rank == TEN) && (getNumOfCardsLeft(ACE, suit) == Selected->getNumOfCardsInRange(ACE, suit)))
				currentLookValue += 2;
			else if ((rank == KING) && (getNumOfCardsLeft(ACE, suit) + getNumOfCardsLeft(TEN, suit) == Selected->getNumOfCardsInRange(ACE, suit) + Selected->getNumOfCardsInRange(TEN, suit)))
				currentLookValue += 4;
			else if ((rank == QUEEN) && (getNumOfCardsLeft(ACE, suit) + getNumOfCardsLeft(TEN, suit) + getNumOfCardsLeft(KING, suit) == Selected->getNumOfCardsInRange(ACE, suit) + Selected->getNumOfCardsInRange(TEN, suit) + Selected->getNumOfCardsInRange(KING, suit)))
				currentLookValue += 8;
			else if ((rank == JACK) && (getNumOfCardsLeft(ACE, suit) + getNumOfCardsLeft(TEN, suit) + getNumOfCardsLeft(KING, suit) + getNumOfCardsLeft(QUEEN, suit) == Selected->getNumOfCardsInRange(ACE, suit) + Selected->getNumOfCardsInRange(TEN, suit) + Selected->getNumOfCardsInRange(KING, suit) + Selected->getNumOfCardsInRange(QUEEN, suit)))
				currentLookValue += 16;
		}

		if ((20 - tricksPlayed) - Selected->getNumOfCardsInRange(-1, trump) <= 1)
		{
			endOfSequence = true;

			if (tricksPlayed == 18)
			{
				if (suit != trump)
					currentLookValue += 256;
			}
			else
			{
				if (suit == trump)
				{
					if (rank == ACE)
						currentLookValue += 128;
					else if (rank == TEN)
						currentLookValue += 64;
					else if (rank == KING)
						currentLookValue += 32;
					else if (rank == QUEEN)
						currentLookValue += 16;
					else
						currentLookValue += 8;
				}
				else
					currentLookValue -= 256;
			}
		}

		//bid winner logic
		if (Selected->getID() == bidWinner)
		{
			if (Selected->getNumOfCardsInRange(ACE, -1) == Selected->getNumOfCardsInRange(ACE, trump))
			{
				//Played all Non-Trump Aces
				if ((Selected->getNumOfCardsInRange(-1, trump) - Selected->getNumOfCardsInRange(ACE, trump)) > 1)
				{//have enough trump to continue
					if ((getNumOfCardsLeft(ACE, trump) - Selected->getNumOfCardsInRange(ACE, trump)) > 0)
					{//still Non-Trump Aces out there
						if (suit == trump)
						{
							if (Partner.known[ACE][trump] == (getNumOfCardsLeft(ACE, trump) - Selected->getNumOfCardsInRange(ACE, trump)))
							{
								if ((rank == KING) || (rank == TEN))
									currentLookValue += 16;
							}
							else if ((Left.known[ACE][trump] > 0) && ((rank == JACK) || ((rank == QUEEN) && (Selected->getNumOfCardsInRange(JACK, trump) == 0))))
								currentLookValue += 16;
							else if ((Left.known[ACE][trump] < 0) && (rank == QUEEN))
								currentLookValue += 16;
							else if ((Left.known[ACE][trump] < 0) && (rank == JACK))
								currentLookValue += 8;
							else if (rank < QUEEN)
								currentLookValue -= 256;  //out of Jacks and Queens, give up looking for the Aces
						}
					}
					else //all Aces are accounted for, play your losers
					{
						if (suit != trump)
						{
							if ((rank == QUEEN) || (rank == JACK))
								currentLookValue += 8;
						}
						else if ((suit == trump) && !endOfSequence)
								currentLookValue -= 128;

						if ((rank == ACE) && (suit == trump))
							currentLookValue -= 128;
					}
				}
				else //play your ACE of trump!
				{
					if ((rank == ACE) && (suit == trump))
						currentLookValue += 32;
				}
			}
			else //make sure you don't play trump
			{
				if ((suit == trump) && !endOfSequence)
					currentLookValue -= 32;
				else if ((suit != trump) && (rank == ACE))
					currentLookValue++;
			}
		}

		if (Partner.getID() == bidWinner) //bidwinner partner's logic
		{
			if (suit == trump)
			{
				if (rank == ACE)
					currentLookValue += 64;
				else if (!endOfSequence)
					currentLookValue -= 256;
			}

			if (Selected->getNumOfCardsInRange(ACE, -1) == 0)
			{
				if (Left.known[ACE][suit] > 0)
				{
					if (rank == JACK) //Know there's an Ace to the left
						currentLookValue += 64;
					else if ((Selected->getNumOfCardsInRange(JACK, suit) == 0) && (rank == QUEEN))
						currentLookValue += 32;
				}
				else if (Left.known[ACE][suit] < 0)
				{
					if (rank == QUEEN) //Don't know
						currentLookValue += 64;
					else if ((Selected->getNumOfCardsInRange(QUEEN, suit) == 0) && (rank == JACK))
						currentLookValue += 32;
				}

			}
			else if ((suit != trump) && (rank == ACE))
				currentLookValue += 8;
		}

		//opposition logic
		if ((Partner.getID() != bidWinner) && (Selected->getID() != bidWinner))
		{
			if (suit == trump)
			{
				if (Selected->getNumOfCardsInRange(-1, trump) <= (Selected->getNumOfCardsInRange(ACE, trump) + 1))
				{
					if (rank == ACE)
						currentLookValue += 32; //running low, play your ACE
				}
				else if (rank == ACE)
					currentLookValue -= 128; //not running low on trump, keep your ACE and all other trump
				else if (!endOfSequence)
					currentLookValue -= 64;
			}
			else if (Selected->getNumOfCardsInRange(ACE, -1) == Selected->getNumOfCardsInRange(ACE, trump))
			{
				//run out of aces to play
				if (Left.known[ACE][suit] > 0)
				{
					if (rank == JACK) //Know there's an Ace to the left
						currentLookValue += 32;
					else if ((Selected->getNumOfCardsInRange(JACK, suit) == 0) && (rank == QUEEN))
						currentLookValue += 16;
				}
				else if (Left.known[ACE][suit] < 0)
				{
					if (rank == QUEEN) //Don't know
						currentLookValue += 32;
					else if ((Selected->getNumOfCardsInRange(QUEEN, suit) == 0) && (rank == JACK))
						currentLookValue += 16;
				}
				else if (rank == KING)
					currentLookValue += 16;
				else if (rank == TEN)
					currentLookValue += 8;
			}
			else if (rank == ACE)
				currentLookValue += 16;
		}
		if (currentLookValue >= bestValue)
		{
			bestValue = currentLookValue;
			cardToPlay = i;
		}
	}

	return cardToPlay;
}

void TrickGame::buildDecisionTree()
{
	for(short int i = 0; i < 5; i++)
		for (short int j = 0; j < 4; j++)
			cardValues[i][j] = i;
								//Jacks = 4, Queens = 3, Kings = 2, Tens = 1, Aces 0

//	for(int i = ACE; i <= JACK; i++)
//		cardValues[i][getTrumpSuit()] = cardValues[i][getTrumpSuit()] - pow(2.0, JACK-i+1);
	//of Trump:  Jacks = 33, Queens = 27, Kings = 21, Tens = 7, Aces = -13
}

void TrickGame::addBulkPoints(short team, short points)
{
	if (team == RED)
		verticalTricks += points;
	else
		horizontalTricks += points;
}
