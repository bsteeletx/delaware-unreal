#pragma once

#include "Player.h"
#include "Defines.h"

class PinochleGame
{
public:
	PinochleGame(void);
	~PinochleGame(void);
	void addToScore(short int team, short int amount);
	void buildDecisionTree();
	void checkForSaves(short int hTricks, short int vTricks, short int bidWinnerID, unsigned short int finalBid);
	void countMeld();
	void determineAmountToGet(Player *West, Player *North, Player *East, Player *South, short int trump, short int bidWinner, unsigned short int bid);
	short determineNewMeld(Player *Selected, short int trump);
	unsigned short int getAmountToSave(short int team);
	short int getPrevState(void);
	short int getScore(short int team);
	short int getState();
	bool isStateActive(short int state);
	bool getSpecificAnimState(void);
	void meldRun(Player *Selected, short int number, short int suit);
	void reset();
	void resetState();
	void resetGame();
	void resetHand();
	void startTrickGame();
	void toggleXState(short int state);
	short horizontalMeld;
	short verticalMeld;
private:
	bool stateLoading;
	bool stateStart;
	bool stateOptions;
	bool stateInGameMenu;
	bool stateInGameOptions;
	bool stateWaitingForAnimation;
	bool stateBidding;
	bool stateMelding;
	bool stateSortingHands;
	bool stateDealing;
	bool statePlaying;
	bool stateMenuScreen;
	bool stateIncrementing;
	bool stateGrowBackground;
	bool stateScoreboard;
	unsigned short int vAmountToGet;
	unsigned short int hAmountToGet;
	short int horizontalScore;
	short int verticalScore;
	short int prevState;
};
