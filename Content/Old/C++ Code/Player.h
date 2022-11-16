#pragma once

#include "Sprite.h"
#include "Defines.h"

class Player
{
public:
	Player(void);
	~Player(void);
	void addToMeld(short int x);
	short int checkForRuns(short int suit);
	bool checkForMarriage(short int suit);
	void clearHand(void);
	void clearMeld(void);
	void dealToHand(short int *card);
	bool getDoubleRun();
	short int getDoubleRunSuit(void);
	short int getKnownOfSuit(short int suit);
	bool findCard(short int card);
	short int getCard(short int value);
	short int getCardFromPlayerHand(short int rank, short int suit);
	short int getCardFromPlayerHand(short int cardNumber);
	short int getHandPlaceholder();
	short int getID();
	short int getMeldBid();
	short int getNumOfAces();
	short int getNumOfBids();
	short int getNumOfCardsInRange(short int rank, short int suit);
	short int getNumOfMarriages(short int suit);
	short int getNumOfPinochles();
	short int getNumOfRuns();
	short int getPMeld(void);
	bool getRoundRobin();
	short int getRunInSuit();
	short int getRunSize(short int suit);
	bool getSaveBid();
	short int getStrongestRun();
	short int getStrongSuit();
	bool getSuitState(short int suit);
	short int getSuitValue(short int suit);
	short int getXAround(short int rank);
	short int handPlaceholder;
	void incrementHandPlaceholder();
	void incrementNumOfBids();
	short int minNumber(short int w, short int x);
	short int minNumber(short int w, short int x, short int y, short int z);
	void reset();
	void resetKnown();
	void removeCardFromHand(short int card);
	void setMeldBid(unsigned short int value);
	void setPlayerID(short int newID);
	void sortHand();
	void setStrongestRun(short int suit);
	void setStrongSuit(short int suit);
	void tabulateMeld();
	void toggleSaveBid();
	short int known[5][4];
	//int newKnown[5][4];
	bool trumpingSuit[4];
	short int getCardsInHand();
	void setlastbid(unsigned short int amount);
	unsigned short getlastbid();
private:
	bool doubleRun;
	bool roundRobin;
	short int numOfAces;
	short int hand[20];
	short int meldBid;
	short int runInSuit;  //binary
	short int numberOfBids;
	short int playerID;
	short int pMeld;
	bool runInHearts;
	bool runInClubs;
	bool runInSpades;
	bool runInDiamonds;
	short int mHearts;
	short int mClubs;
	short int mSpades;
	short int mDiamonds;
	bool saveBid;
	short int strongSuit;
	short int strongestRun;
	short int numOfRuns;
	short int numOfBids;  //non-meld
	short int cardsInHand;
	short int lastbid;
};
