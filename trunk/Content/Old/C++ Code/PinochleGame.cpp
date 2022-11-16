#include "PinochleGame.h"
//#include "template.h"
#include "BidGame.h"

PinochleGame::PinochleGame(void)
{
	stateStart = false;
    stateOptions = false;
	stateInGameMenu = false;
	stateInGameOptions = false;
	stateGrowBackground = false;
	stateWaitingForAnimation = false;
	stateBidding = false;
	stateSortingHands = false;
	stateDealing = false;
	statePlaying = false;
	stateMelding = false;
	stateMenuScreen = true;
	stateLoading = true;
	stateIncrementing = false;
	stateScoreboard = false;
	horizontalMeld = -1;
	verticalMeld = -1;
	horizontalScore = 0;
	verticalScore = 0;
	hAmountToGet = -1;
	vAmountToGet = -1;
}

void PinochleGame::reset(void)
{
	stateStart = false;
	stateWaitingForAnimation = false;
	stateBidding = false;
	stateSortingHands = false;
	stateDealing = false;
	statePlaying = false;
	stateMelding = false;
	stateMenuScreen = true;
	stateInGameMenu = false;
	stateInGameOptions = false;
	stateGrowBackground = false;
	stateIncrementing = false;
	stateScoreboard = false;
	horizontalMeld = -1;
	verticalMeld = -1;
	horizontalScore = 0;
	verticalScore = 0;
	hAmountToGet = -1;
	vAmountToGet = -1;
}

void PinochleGame::resetState(void)
{
	stateStart = false;
	stateLoading = false;
	stateWaitingForAnimation = false;
	stateBidding = false;
	stateSortingHands = false;
	stateDealing = false;
	statePlaying = false;
	stateMelding = false;
	stateMenuScreen = true;
	stateInGameMenu = false;
	stateInGameOptions = false;
	stateGrowBackground = false;
	stateIncrementing = false;
	stateScoreboard = false;
}

void PinochleGame::resetGame(void)
{
	stateStart = false;
	stateWaitingForAnimation = false;
	stateBidding = false;
	stateSortingHands = false;
	stateDealing = false;
	statePlaying = false;
	stateMelding = false;
	stateMenuScreen = false;
	stateInGameMenu = false;
	stateInGameOptions = false;
	stateGrowBackground = false;
	stateLoading = false;
	stateScoreboard = false;
	horizontalMeld = -1;
	verticalMeld = -1;
	horizontalScore = 0;
	verticalScore = 0;
	hAmountToGet = -1;
	vAmountToGet = -1;
}

void PinochleGame::resetHand(void)
{
	horizontalMeld = -1;
	verticalMeld = -1;
	hAmountToGet = -1;
	vAmountToGet = -1;
}

PinochleGame::~PinochleGame(void)
{
}

short int PinochleGame::getState()
{
	if (stateLoading)
		return LOADING;
	if (stateMenuScreen)
		return MENU;
	if (stateOptions)
		return OPTIONS;
	if (stateInGameMenu)
		return INGAMEMENU;
	if (stateInGameOptions)
		return INGAMEOPTIONS;
	if (stateGrowBackground)
		return GROWBACKGROUND;
	if (stateWaitingForAnimation)
		return ANIMATION;
	if (stateStart)
		return START;
	if (stateDealing)
		return DEALING;
	if (stateSortingHands)
		return SORTING;
	if (stateBidding)
		return BIDDING;
	if (stateMelding)
		return MELDING;
	if (statePlaying)
		return PLAYING;
	if (stateScoreboard)
		return SCOREBOARD;

    if (prevState != -1) //fix an issue where no states are active
        return prevState;

	return -1;
}

bool PinochleGame::isStateActive(short int state)
{
	switch (state)
	{
	case LOADING:
		if (stateLoading)
			return true;
		break;
	case MENU:
		if(stateMenuScreen)
			return true;
        break;
	case OPTIONS:
		if (stateOptions)
			return true;
		break;
	case START:
		if(stateStart)
			return true;
		break;
	case INGAMEMENU:
		if (stateInGameMenu)
			return true;
		break;
	case INGAMEOPTIONS:
		if (stateInGameOptions)
			return true;
		break;
	case GROWBACKGROUND:
		if (stateGrowBackground)
			return true;
		break;
	case DEALING:
		if(stateDealing)
			return true;
		break;
	case ANIMATION:
		if(stateWaitingForAnimation)
			return true;
		break;
	case SORTING:
		if (stateSortingHands)
			return true;
		break;
	case BIDDING:
		if (stateBidding)
			return true;
		break;
	case MELDING:
		if (stateMelding)
			return true;
		break;
	case PLAYING:
		if (statePlaying)
			return true;
		break;
	case SCOREBOARD:
		if (stateScoreboard)
			return true;
		break;
	default:
		break;
	}

	return false;

}
bool PinochleGame::getSpecificAnimState(void)
{
	if (statePlaying)
		return true;
	else
		return false;
}


void PinochleGame::toggleXState(short int state)
{
	short temp = getState();

    if (state == temp)
        prevState = temp;

	if (state == LOADING)
		stateLoading = !stateLoading;
	else if (state == START)
		stateStart = !stateStart;
	else if (state == OPTIONS)
		stateOptions = !stateOptions;
	else if (state == BIDDING)
		stateBidding = !stateBidding;
	else if (state == PLAYING)
		statePlaying = !statePlaying;
	else if (state == ANIMATION)
		stateWaitingForAnimation = !stateWaitingForAnimation;
	else if (state == SORTING)
		stateSortingHands = !stateSortingHands;
	else if (state == DEALING)
		stateDealing = !stateDealing;
	else if (state == MELDING)
		stateMelding = !stateMelding;
	else if (state == MENU)
		stateMenuScreen = !stateMenuScreen;
	else if (state == INGAMEMENU)
		stateInGameMenu = !stateInGameMenu;
	else if (state == INGAMEOPTIONS)
		stateInGameOptions = !stateInGameOptions;
	else if (state == GROWBACKGROUND)
		stateGrowBackground = !stateGrowBackground;
	else if (state == SCOREBOARD)
		stateScoreboard = !stateScoreboard;
}

short int PinochleGame::getPrevState(void)
{
	short int curState = getState();

	if ((curState != ANIMATION) && (curState != OPTIONS))
		return -1;
	else
		return prevState;
}

unsigned short int PinochleGame::getAmountToSave(short int team)
{
	if (team == 0)
		return hAmountToGet;
	else
		return vAmountToGet;
}

void PinochleGame::determineAmountToGet(Player *West, Player *North, Player *East, Player *South, short int trump, short int bidWinner, unsigned short int bid)
{
	horizontalMeld = determineNewMeld(West, trump);
	horizontalMeld += determineNewMeld(East, trump);
	verticalMeld = determineNewMeld(North, trump);
	verticalMeld += determineNewMeld(South, trump);

	if (horizontalMeld < 20)
	{
		horizontalMeld = 0;
		West->resetKnown();
		East->resetKnown();
	}
	if (verticalMeld < 20)
	{
		North->resetKnown();
		South->resetKnown();
		verticalMeld = 0;
	}

	if((bidWinner == WEST) || (bidWinner == EAST))
	{
		if (bid - horizontalMeld < 20)
			hAmountToGet = 20;
		/*else if (horizontalMeld < 20) don't need to do this at all
		{
			//addToScore(BLUE, -bid); //ALREADY DOING THIS
			if (verticalMeld >= 20)
				addToScore(RED, verticalMeld);
		}*/
		else
			hAmountToGet = bid - horizontalMeld;

		vAmountToGet = 20;
	}
	else if((bidWinner == NORTH) || (bidWinner == SOUTH))
	{
		if (bid - verticalMeld < 20)
			vAmountToGet = 20;
		/*else if (verticalMeld < 20)
		{
			//addToScore(RED, -bid); //ALREADY DOING THIS
			if (horizontalMeld >= 20)
				addToScore(BLUE, horizontalMeld);
		}*/
		else
			vAmountToGet = bid - verticalMeld;

		hAmountToGet = 20;
	}

}

void PinochleGame::checkForSaves(short int hTricks, short int vTricks, short int bidWinnerID, unsigned short int finalBid)
{
	short int saveH = hTricks - hAmountToGet;
	short int saveV = vTricks - vAmountToGet;

	if (saveH >= 0)
		addToScore(BLUE, hTricks + horizontalMeld);
	else
	{
		if (bidWinnerID % 2 == BLUE)
			addToScore(BLUE, -finalBid);
	}

	if (saveV >= 0)
		addToScore(RED, vTricks + verticalMeld);
	else
	{
		if (bidWinnerID % 2 == RED)
			addToScore(RED, -finalBid);
	}
}

void PinochleGame::addToScore(short int team, short int amount)
{
	if (team == BLUE)
		horizontalScore += amount;
	else if (team == RED)
		verticalScore += amount;
}

short int PinochleGame::getScore(short int team)
{
	if (team == BLUE)
		return horizontalScore;
	if (team == RED)
		return verticalScore;

	return -1;
}

short PinochleGame::determineNewMeld(Player *Selected, short int trump)
{
	unsigned short int newMeld = 0;

	newMeld += Selected->getPMeld();
	if (!Selected->getRoundRobin())
		newMeld += (Selected->getNumOfMarriages(trump)) * 2;
	if (Selected->checkForRuns(trump))
	{
		if (Selected->getDoubleRun())
		{
			meldRun(Selected, 2, trump);
			newMeld += 142;
		}
		else
		{
			newMeld += 11;
			meldRun(Selected, 1, trump);
		}
	}

	Selected->addToMeld(newMeld - Selected->getPMeld());
	return Selected->getPMeld();
}

void PinochleGame::meldRun(Player *Selected, short int number, short int suit)
{
	//int player = Selected->getID();

	for (short int i = 0; i < 5; i++)
	{
		if (!Selected->getDoubleRun())
		{
			if (Selected->known[i][suit] <= 0)
				Selected->known[i][suit] = number;

			if (suit == DIAMONDS)
			{
				if ((Selected->getNumOfPinochles() == 0) && (!Selected->checkForMarriage(SPADES)) && (!Selected->getXAround(QUEEN)))
					Selected->known[QUEEN][SPADES] = 0;
			}
			else if (suit == SPADES)
			{
				if ((Selected->getNumOfPinochles() == 0) && (!Selected->checkForMarriage(DIAMONDS)) && (!Selected->getXAround(QUEEN)))
					Selected->known[JACK][DIAMONDS] = 0;
			}
		}
		else
		{
			if (!(Selected->known[i][suit] < 2))
				Selected->known[i][suit] = 2;

			if (suit == DIAMONDS)
			{
				if ((Selected->getNumOfPinochles() == 0) && (!Selected->checkForMarriage(SPADES)) && (!Selected->getXAround(QUEEN)))
					Selected->known[QUEEN][SPADES] = 0;
			}
			else if (suit == SPADES)
			{
				if ((Selected->getNumOfPinochles() == 0) && (!Selected->checkForMarriage(DIAMONDS)) && (!Selected->getXAround(QUEEN)))
					Selected->known[JACK][DIAMONDS] = 0;
			}
		}
	}
}
