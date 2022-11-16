#include "BidGame.h"
//#include "DarkGDK.h"
#include "Defines.h"

BidGame::BidGame()
{
	bid = 49;
	meldBidding = true;
	for (int i = 0; i < 4; i++)
	{
		hasPassed[i] = false;
		hasBid[i] = false;
		passRound[i] = -1;
	}
	
	bidDecided = false;
	bidder = -1;
}

BidGame::~BidGame(void)
{
}

void BidGame::reset()
{
	bid = 49;
	meldBidding = true;
	for (int i = 0; i < 4; i++)
	{
		hasPassed[i] = false;
		hasBid[i] = false;
		passRound[i] = -1;
	}
	
	bidDecided = false;
	bidder = -1;
}

unsigned short int BidGame::getBid()
{
	return bid;
}

bool BidGame::playerBid(unsigned short int playerID)
{
	return hasBid[playerID];
}

void BidGame::setPlayerBid(unsigned short int playerID)
{
	hasBid[playerID] = true;
}

void BidGame::playerMeldBid(Player *South, Player *North, unsigned short int newBid)
{
	if (bid == 49)
		bid++;

	if (hasPassed[WEST] && hasPassed[EAST] && (newBid - 1 == bid))
	{
		South->toggleSaveBid();
		South->setMeldBid(0);
		incrementBid(newBid);
		setBidder(South->getID());
	}
	else if ((newBid > 49) && (newBid < 60))
	{
		South->setMeldBid(newBid-bid);

		incrementBid(newBid);
		setBidder(South->getID());

		if (newBid == 50)
		{
			South->incrementNumOfBids();
			South->incrementNumOfBids();
			South->incrementNumOfBids();//increment to 7 or 8 for run size
		}
	}
	else if (newBid >= 60)
	{
		if (bid < 59)
		{
			South->incrementNumOfBids();
			South->incrementNumOfBids();
			South->incrementNumOfBids();//increment to 7 or 8 for run size
		}
		else
			South->incrementNumOfBids();

		incrementBid(newBid);
		South->setMeldBid(0);
		setBidder(South->getID());
	}
	else
	{
		togglePass(South->getID());
		South->setMeldBid(0);
	}
}

void BidGame::meldBid(Player *meldBidder, Player *partner)
{
	short meld;
	short int aces;
	unsigned short int meldBid;
	short int meldBidderID = meldBidder->getID();
	short int suit = 4;
	short int runValue;
	short int topValue;
	//int x;
	//int y;

	meld = meldBidder->getPMeld();
	meldBid = meld / 10;
	aces = meldBidder->getNumOfAces();

	if (meld % 10 == 6)
	{
		if (aces >= 4)
			meldBid++;
	}
	else if (meld % 10 == 4)
	{
		if (aces >= 8)
			meldBid++;
	}
	else if (meld % 10 == 8)
		meldBid++;

	if (getPass(partner->getID()))
	{
		takeBid(meldBidder, partner, 0);
		return;
	}

	if (!meldBid) //no meld!
	{
		if (meldBidder->getStrongestRun() != 4) //have a run ??
			suit = meldBidder->getStrongestRun();
		else if (meldBidder->getDoubleRun())
		{
			if (bid < 65)
				incrementBid(65);
			else
				incrementBid();

			meldBidder->setMeldBid(0);
			setBidder(meldBidderID);
		}
		else // you don't have a run!
		{
			suit = meldBidder->getStrongSuit();
			if (suit == HEARTS)
			{
				if (!meldBidder->checkForMarriage(HEARTS))
					suit = 4;
			}
			else if (suit == CLUBS)
			{
				if (!meldBidder->checkForMarriage(CLUBS))
					suit = 4;
			}
			else if (suit == DIAMONDS)
			{
				if (!meldBidder->checkForMarriage(DIAMONDS))
					suit = 4;
			}
			else if (suit == SPADES)
			{
				if (!meldBidder->checkForMarriage(SPADES))
					suit = 4;
			}
		}

		if (suit != 4)
			runValue = meldBidder->getSuitValue(suit);
		else
			runValue = 0;

		if (runValue >= 24)  //this is a pretty good run/marriage!
		{
			//dbText(x, y, "*Pretty Good Suit, though!*");
			//dbSync();
			//dbWait(3*SECONDS);
			////dbWait(5 * SECONDS);
			///not first bid
			if (meldBidderID != ((getDealer() + 1) % 4))
			{
				//dbText(x, y, "*Not the first bid, has my partner gone yet?*");
				//dbSync();
				//dbWait(3*SECONDS);
				////dbWait(5 * SECONDS);
				if (partner->getMeldBid() == -1) //partner has not given a meld bid
				{
					incrementBid();
					meldBidder->setMeldBid(1);
					setBidder(meldBidderID);
					return;
				}
				else //partner has given a meld bid
				{
					//dbText(x, y, "*Yes he has, let me check how high I can go...");
					//dbSync();
					//dbWait(3*SECONDS);
					////dbWait(5 * SECONDS);
					//dbText(x, y, "...hopefully I won't have to give him a meld bid*");
					//dbSync();
					//dbWait(3*SECONDS);
					if (runValue != 4)
						topValue = findTopValue(meld + 13 + ((meldBidder->getNumOfMarriages(suit) - 1) * 2), partner->getMeldBid(), aces, meldBidder->getRunSize(suit));
					else
						topValue = findTopValue(meld + (2 * meldBidder->getNumOfMarriages(suit)), partner->getMeldBid(), aces, meldBidder->getRunSize(suit));

					if (topValue > 60)
						topValue = topValue - (topValue % 5);

					//dbText(x, y, "*My Top Value is........");
					//dbSync();
					//dbWait(3*SECONDS);
					//dbPrint((double)topValue);
					//dbText(x, y, "*Is it higher than the current bid?*");
					//dbSync();
					//dbWait(3*SECONDS);
					////dbWait(5 * SECONDS);

					if (topValue > getBid())  //CHECK FOR PLAYER --AFTER-- THIS ONE!!!
					{
						if (getBid() < 60)
						{
							if (topValue >= 60) //removing any doubt of a meldbid
							{
								//dbText(x, y, "*Yes it is, and I'm removing any doubt of a meld bid by bidding...");
								if (topValue % 10 == 0)
								{
									incrementBid(60);
									meldBidder->incrementNumOfBids();
									meldBidder->incrementNumOfBids();
									meldBidder->incrementNumOfBids();//increment to 7 or 8 for run size
								//	dbPrint((double)getBid());
								}
								else
								{
									incrementBid(65);
									meldBidder->incrementNumOfBids();
									meldBidder->incrementNumOfBids();
									meldBidder->incrementNumOfBids();//increment to 7 or 8 for run size
								//	dbPrint((double)getBid());
								}

								setBidder(meldBidderID);

								meldBidder->setMeldBid(0);

								return;
							}
							else  //bidding on a run..ouch!
							{
								incrementBid();
								meldBidder->setMeldBid(1);
								setBidder(meldBidder->getID());
								return;
							}
						}
						else
						{
							return;
						}
					}
					else
					{
						if (!getPass(partner->getID()))  //if Partner has NOT passed
						{
							//dbText(x, y, "***My Partner hasn't passed...");
							//dbSync();
							//dbWait(3*SECONDS);
							////dbWait(5 * SECONDS);
							//dbText(x, y, "***Does he need to be saved?");
							//dbSync();
							//dbWait(3*SECONDS);
							////dbWait(5 * SECONDS);

							if (getPass((partner->getID() + 1) % 4))//see if he needs to be saved
							{
								//dbText(x, y, "***Yes he does....wait, is it just between us?");
								//dbSync();
								//dbWait(3*SECONDS);
								////dbWait(5 * SECONDS);
								if (getPass((meldBidder->getID() + 1) % 4))
								{
									//dbText(x, y, "***Yup, just between us, so do I want it?");
									//dbSync();
									//dbWait(3*SECONDS);
									takeBid(meldBidder, partner, 0);
									meldBidder->setMeldBid(0);
									return;
								}

								//see if you can save your partner
								if (suit != 4) //yes I can
								{
									//dbText(x, y, "***Yes I can, but is my marriage any good?");
									//dbSync();
									//dbWait(3*SECONDS);
									////dbWait(5 * SECONDS);
									//if so, make sure the marriage is "good"
									if (meldBidder->getSuitValue(suit) > 10)
									{
										meldBidder->toggleSaveBid();
										//meldBidder->incrementNumOfBids();
										incrementBid();
										meldBidder->setMeldBid(0);
										return;
									}
									else
									{
										togglePass(meldBidder->getID());
										meldBidder->setMeldBid(0);
										return;
									}
								}
								else
								{
									togglePass(meldBidder->getID());
									meldBidder->setMeldBid(0);
									return;
								}
							}
							else
							{
								togglePass(meldBidder->getID());
								meldBidder->setMeldBid(0);
								return;
							}
						}// all conditions covered
						else
						{
							togglePass(meldBidder->getID());
							meldBidder->setMeldBid(0);
							return;
						}//already an else
					}//already an else
				}//already an else
			}
			//first bid
			else if (getBid() == 49)
			{
				incrementBid();
				meldBidder->setMeldBid(0);
				meldBidder->incrementNumOfBids();
				meldBidder->incrementNumOfBids();
				meldBidder->incrementNumOfBids(); //increment to 7 or 8 for run size
				setBidder(meldBidderID);
				return;
			}//all conditions covered
		}//all conditions covered
		else //no marriage or run worth bidding on
		{
			//dbText(x, y, "***My Partner hasn't passed...");
			//dbSync();
			//dbWait(3*SECONDS);
			////dbWait(5 * SECONDS);
			//dbText(x, y, "***Does he need to be saved??");
			//dbSync();
			//dbWait(3*SECONDS);
			////dbWait(5 * SECONDS);

			if (getPass((partner->getID() + 1) % 4))//see if he needs to be saved
			{
				//dbText(x, y, "***Yes he does....Can I save him?");
				//dbSync();
				//dbWait(3*SECONDS);
				////dbWait(5 * SECONDS);
				//see if you can save your partner
				if (suit != 4) //yes I can
				{
					//dbText(x, y, "***Yes I can, but is my marriage any good?");
					//dbSync();
					//dbWait(3*SECONDS);
					////dbWait(5 * SECONDS);
					//if so, make sure the marriage is "good"
					if (meldBidder->getSuitValue(suit) > 10)
					{
						meldBidder->toggleSaveBid();
						meldBidder->incrementNumOfBids();
						incrementBid();
						meldBidder->setMeldBid(1);
						return;
					}
					else
					{
						short int tempID = meldBidder->getID();
						togglePass(tempID);
						meldBidder->setMeldBid(0);
						return;
					}
				}
				else
				{
					short int tempID = meldBidder->getID();
					togglePass(tempID);
					meldBidder->setMeldBid(0);
					return;
				}
			}
			setPass(meldBidderID);
			meldBidder->setMeldBid(0);
			return;
		}
	}
	else if (bid < 59) //does have a meld bid, and can give a meld bid
	{
		if (bid == 49)
			bid = 50;
		//dbText(x, y, "I have Meld!  I'm going to give a Meld bid of....");
		//dbSync();
		//dbWait(3*SECONDS);
		////dbWait(5 * SECONDS);
		if ((meldBidder->getID() == WEST) || (meldBidder->getID() == EAST))
		{
			if (hasPassed[NORTH] && hasPassed[SOUTH])
			{
				takeBid(meldBidder, partner, 0);
				meldBidder->setMeldBid(0);
				return;
			}
		}
		else
		{
			if (hasPassed[EAST] && hasPassed[WEST])
			{
				takeBid(meldBidder, partner, 0);
				meldBidder->setMeldBid(0);
				return;
			}
		}

		if (bid + meldBid >= 60)
		{
			//dbText(x, y, "*Wait, I can't, we're at the top already!  I'll do the best I can...");
			//dbSync();
			//dbWait(3*SECONDS);
			if (bid != 59)
			{
				incrementBid(59);
				meldBidder->setMeldBid(59 - getBid());
				setBidder(meldBidderID);
				return;
			}
			else
			{
				//dbText(x, y, "*Do I want to take the bid?*");
				//dbSync();
				//dbWait(3*SECONDS);
				takeBid(meldBidder, partner, 0);
				meldBidder->setMeldBid(0);
				return;
			}
		}
		else
		{
			incrementBid(meldBid + getBid());

			meldBidder->setMeldBid(meldBid);
			setBidder(meldBidderID);
			return;
		}
	}
	else
	{
		takeBid(meldBidder, partner, 0);
		meldBidder->setMeldBid(0);
	}
}

unsigned short int BidGame::findTopValue(unsigned short int meld, unsigned short int partnerMeld, short int aces, short int size)
{
	return (meld + (partnerMeld * 10) + (aces * 2) + (size - ((20 - size) / 3)) + 20);
}

void BidGame::setBid(unsigned short int newBid)
{
	bid = newBid;
}

void BidGame::setPass(short int player, short int round)
{
	unsigned short count = 0;
	unsigned short notPassed = 0;
	hasPassed[player] = true;
	passRound[player] = round;

	for (int i = 0; i < 4; i++)
	{
		if (!hasPassed[i])
		{
			count++;
			notPassed = i;

			if (count > 1)
				break;
		}
	}

	if (count == 1)
	{
		toggleBidDecided();
		winner = notPassed;
	}
}

short int BidGame::getPassRound(short int player)
{
	return passRound[player];
}

void BidGame::setBidder(unsigned short int player)
{
	bidder = player % 4;
}

void BidGame::incrementBid()
{
	if (bid < 60)
		bid++;
	else
		bid += 5;
}

short int BidGame::getDealer()
{
	return dealer;
}

void BidGame::setDealer(unsigned short int value)
{
	dealer = value;
}

bool BidGame::getBidDecided()
{
	return bidDecided;
}

void BidGame::toggleBidDecided()
{
	bidDecided = !bidDecided;
}

bool BidGame::getPass(unsigned short int player)
{
	return hasPassed[player];
}

short int BidGame::getBidWinner()
{
	return winner;
}

void BidGame::playerTakeBid(Player *South, Player *North, short int round, unsigned short int newBid)
{
	if (newBid > 49)
	{
		if (!getPass(NORTH))  //if Partner has NOT passed
		{
			if (North->getNumOfBids() == 0) //partner has only meld bidded (or passed)
			{
				if (getPass((NORTH + 1) % 4))//see if he needs to be saved
				{
					South->toggleSaveBid();
				}
			}
		}
		South->incrementNumOfBids();
		incrementBid(newBid);
		setBidder(SOUTH);
		return;
	}
	else
		setPass(SOUTH, round);
}

void BidGame::takeBid(Player *taker, Player *teammate, short int round)
{
	short int teammateID;
	short int bestR = 0;
	short int bestM = 0;
	short int sizeOfSuit = 0;
	short int teammateRunSize;
	bool mBetterThanR = false;
	short int topWRun = 0;
	short int topWMarriage = 0;
	short int aces = taker->getNumOfAces();
	//short int takerID = taker->getID();

	teammateID = teammate->getID();

	bestM = taker->getStrongSuit();
	bestR = taker->getStrongestRun();

	if (bestR < 4)
	{
		sizeOfSuit = taker->getRunSize(bestR);

		if (taker->getSuitValue(bestM) > taker->getSuitValue(bestR))
			mBetterThanR = true;
	}
	else
		mBetterThanR = true;

	if (bestM == 4)
	{
		setPass(taker->getID(), round);
		return;
	}

	if (taker->getDoubleRun())
	{
		taker->incrementNumOfBids();
		incrementBid();
		setBidder(taker->getID());
		return;
	}

	//check to see if parnter has passed
	if (!getPass(teammateID))  //if Partner has NOT passed
	{
		if (teammate->getNumOfBids() == 0) //partner has only meld bidded (or passed)
		{
			if (getPass((teammateID + 1) % 4))//see if he needs to be saved
			{
				//see if you can save your partner
				if (bestM != 4) //yes I can
				{
					//if so, make sure the marriage is "good"
					if (taker->getSuitValue(bestM) > 10)
					{
						taker->toggleSaveBid();
						taker->incrementNumOfBids();
						incrementBid();
						setBidder(taker->getID());
						return;
					}
					else //marriage sucks
					{
						setPass(taker->getID(), round);
						return;
					}
				}
			}
			//partner does not need saving, do you even want it?
			else if (!sizeOfSuit && !mBetterThanR) //if no run or marriage, no
			{
				setPass(taker->getID(), round);
				return;
			}

			if (mBetterThanR)
			{
				topWMarriage = findTopValue(taker->getPMeld() + (2 * taker->getNumOfMarriages(bestM)), teammate->getMeldBid(), aces, taker->getRunSize(bestM));
				if (topWMarriage > 60)
					topWMarriage = topWMarriage - (topWMarriage % 5);
			}

			if (bestR < 4)
			{
				if (taker->getDoubleRun())
					topWRun = findTopValue(taker->getPMeld() + 146 + (2 * (taker->getNumOfMarriages(bestR) - 2)), teammate->getMeldBid(), aces, taker->getRunSize(bestR));
				else
					topWRun = findTopValue(taker->getPMeld() + 13 + (2 * (taker->getNumOfMarriages(bestR) - 1)), teammate->getMeldBid(), aces, taker->getRunSize(bestR));
				topWRun = topWRun - (topWRun % 5);
			}

			if (mBetterThanR)
			{
				if (topWMarriage > getBid())
				{
					if (getBid() < 60)
					{
						if (getBid() == 58)
						{
							if (topWMarriage % 10 == 0)
								incrementBid(60);
							else
								incrementBid();
						}
						else if (getBid() == 59)
						{
							if (topWMarriage % 10 == 0)
								incrementBid();
							else
								incrementBid(65);
						}
						else
							incrementBid();

						taker->incrementNumOfBids();
						setBidder(taker->getID());
						return;
					}
					else
					{
						if (topWMarriage - getBid() == 10)
							incrementBid(getBid() + 10);
						else
							incrementBid();

						taker->incrementNumOfBids();
						setBidder(taker->getID());
						return;
					}
				}
				else
					taker->setStrongSuit(bestR);
			}

			if (topWRun > getBid())
			{
				if (getBid() < 60)
				{
					if (getBid() == 58)
					{
						if (topWRun % 10 == 0)
							incrementBid(60);
						else
							incrementBid();
					}
					else if (getBid() == 59)
					{
						if (topWRun % 10 == 0)
							incrementBid();
						else
							incrementBid(65);
					}
					else
						incrementBid();

					taker->incrementNumOfBids();
					setBidder(taker->getID());
					return;
				}
				else
				{
					if (topWRun - getBid() == 10)
						incrementBid(getBid() + 10);
					else
						incrementBid();

					taker->incrementNumOfBids();
					setBidder(taker->getID());
					return;
				}
			}
			else //top value is not high enough
			{
				setPass(taker->getID(), round);
				return;
			}
		}
		else  //partner indicated he had a run, determine if your run is better
		{
			teammateRunSize = ((teammate->getNumOfBids() + taker->getNumOfBids()) * 2) + 4;

			if (bestR < 4)
			{
				topWRun = findTopValue(taker->getPMeld() + 13 + (2 * (taker->getNumOfMarriages(bestR) - 1)), teammate->getMeldBid(), aces, taker->getRunSize(bestR));
				topWRun = topWRun - (topWRun % 5);
			}
			else
			{//partner has a run, pass
				setPass(taker->getID(), round);
				return;
			}

			if (teammate->getSaveBid() || taker->getSaveBid())
				teammateRunSize--;

			if (teammateRunSize > sizeOfSuit)
			{
				setPass(taker->getID(), round);
				return;
			}
			else if (topWRun > getBid())
			{
				short int modValue = 10;

				switch (taker->getID() % 2)
				{
				case WEST:
					if (hasPassed[NORTH] && hasPassed[SOUTH])
						modValue = 5;
					break;
				case NORTH:
					if (hasPassed[WEST] && hasPassed[EAST])
						modValue = 5;
					break;
				}

				if (getBid() < 60)
				{

					if (modValue == 10)
					{
						if (getBid() == 58)
						{

							if (topWRun % modValue == 0)
								incrementBid(60);
							else
								incrementBid();
						}
						else if (getBid() == 59)
						{
							if (topWRun % modValue == 0)
								incrementBid();
							else
								incrementBid(65);
						}
						else
							incrementBid();
					}
					else
						incrementBid();

					taker->incrementNumOfBids();
					setBidder(taker->getID());
					return;
				}
				else
				{
					if (topWRun - getBid() == modValue)
						incrementBid(getBid() + modValue);
					else
						incrementBid();

					taker->incrementNumOfBids();
					setBidder(taker->getID());
					return;
				}
			}
			else //top value is not high enough
			{
				setPass(taker->getID(), round);
				return;
			}
		}
	}
	else  //partner has passed, no need to save him
	{
		if (mBetterThanR)
		{
			topWMarriage = findTopValue(taker->getPMeld() + (2 * taker->getNumOfMarriages(bestM)), teammate->getMeldBid(), aces, taker->getRunSize(bestM));
			if (topWMarriage > 60)
				topWMarriage = topWMarriage - (topWMarriage % 5);
		}

		topWRun = findTopValue(taker->getPMeld() + 13 + (2 * (taker->getNumOfMarriages(bestR) - 1)), teammate->getMeldBid(), aces, taker->getRunSize(bestR));
		topWRun = topWRun - (topWRun % 5);

		if (mBetterThanR)
		{
			if (topWMarriage > getBid())
			{
				if (getBid() < 60)
				{
					if (getBid() == 58)
					{
						if (topWMarriage % 10 == 0)
							incrementBid(60);
						else
								incrementBid();
					}
					else if (getBid() == 59)
					{
						if (topWMarriage % 10 == 0)
							incrementBid();
						else
							incrementBid(65);
					}
					else
						incrementBid();

					taker->incrementNumOfBids();
					setBidder(taker->getID());
					return;
				}
				else
				{
					if (topWMarriage - getBid() == 10)
						incrementBid(getBid() + 10);
					else
						incrementBid();

					taker->incrementNumOfBids();
					setBidder(taker->getID());
					return;
				}
			}
			else //best Marriage not good enough
				taker->setStrongSuit(bestR);
		}

		if (topWRun > getBid())
		{
			if (getBid() < 60)
			{
				if (getBid() == 58)
				{
					if (topWRun % 10 == 0)
						incrementBid(60);
					else
						incrementBid();
				}
				else if (getBid() == 59)
				{
					if (topWRun % 10 == 0)
						incrementBid();
					else
						incrementBid(65);
				}
				else
					incrementBid();

				taker->incrementNumOfBids();
				setBidder(taker->getID());
				return;
			}
			else
			{
				if (topWRun - getBid() == 10)
					incrementBid(getBid() + 10);
				else
					incrementBid();

				taker->incrementNumOfBids();
				setBidder(taker->getID());
				return;
			}
		}
		else //top value is not high enough
		{
			setPass(taker->getID(), round);
			return;
		}
	}
}

short int BidGame::getBidder()
{
	return bidder;
}

void BidGame::incrementBid(unsigned short int newBid)
{
	bid = newBid;
}

short int BidGame::getBestSuit(short int w, short int x)
{
	if (w > x)
		return 0;
	else
		return 1;
}

short int BidGame::getBestSuit(short int w, short int x, short int y, short int z)
{
	if (w > x)
		if (w > y)
			if (w > z)
				return 0;
			else
				return 3;
		else if (y > z)
			return 2;
		else
			return 3;
	else if (x > y)
		if (x > z)
			return 1;
		else
			return 3;
	else if (y > z)
		return 2;
	else
		return 3;
}


void BidGame::togglePass(short int passer)
{
	setPass(passer);
}
