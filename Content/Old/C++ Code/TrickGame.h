#pragma once

#include "Player.h"
#include "Defines.h"

class TrickGame
{
public:
	TrickGame();
	~TrickGame(void);

	void addBulkPoints(short team, short points);
	void buildDecisionTree();
	short int findRank(short int value, bool trumped);
	short int followLead(Player *Selected, short int rank, short int suit, bool lTrump, bool pTrump, short int count);
	short int getHTricks();
	short int getLead();
	short int getLeadSuit();
	short int getNumOfCardsLeft(short int rank, short int suit);
	short int getTopRank();
	short int getTopSuit();
	short int getTrickCounter();
	short int getTricksPlayed();
	short int getTrumpSuit();
	short int getVTricks();
	void lastTrick(short int player);
	short int playCard(Player *Selected, Player Left, Player Partner, Player Right, short int count, short int playerCard = -1);
	void playedCard(short int card, Player Selected);
	short int potentialTrickValue(Player *Selected, Player Left, Player Partner, Player Right, short int rank, short int suit);
	short int potPlays(Player Selected, Player SelectedP, Player *Starter, Player StarterP, short int rank, short int suit, bool *trumped, short int *trumpRank, bool lostTrick, short int loopType);
	void reset();
	void setLead(short int turn);
	void setLeadSuit(short int suit);
	void setTopRank(short int rank);
	void setTopSuit(short int suit);
	void setTrumpSuit(short int suit);
	short int startTrick(Player *, Player, Player, Player);
	void tallyTrick();
	void updateTrump(short int trumpPlayed);
private:
	short int trump;
	short int lead;
	short int playedCards[5][4];
	short int trumpLeft;
	short int topRank;
	short int topSuit;
	short int leadSuit;
	short int verticalTricks;
	short int horizontalTricks;
	short int trickValue;
	short int trickCounter;
	short int cardValues[5][4];
	short int tricksPlayed;
	short int bidWinner;
};
