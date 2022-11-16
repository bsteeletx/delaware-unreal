#pragma once

#include "AnimatedSprite.h"
#include "BidControl.h"
#include "Bubbles.h"
#include "Buttons.h"
#include "Card.h"
#include "Numbers.h"
#include "Sprite.h"
#include "TrumpSelect.h"
#include "Defines.h"
#include "ParentImage.h"

class Theme
{
public:
	Theme(void);
	~Theme(void);

	void show(short int state);
	void hide(void);
	void setup(AnimatedSprite *Symbol, char dir[], short int state, float _aspect);
	void setNumberData(void);
	void setSizeNumbers(void);
	void setNumberLoc(void);
	void setNumberPrio(void);
	void initNumbers(void);
    bool isSetup(void);
	void reset(void);

	char currentDir[64];

	BidControl BCBubble;
	Bubbles BidMeldBubbles;
	Buttons InGameMenuButton;

	Card Deck[80];

	Numbers Bid;
	Numbers BidScore;
	Numbers H2Get;
	Numbers HMeld;
	Numbers HScore;
	Numbers PastEastBid;
	Numbers PastNorthBid;
	Numbers PastSouthBid;
	Numbers PastWestBid;
	Numbers V2Get;
	Numbers VMeld;
	Numbers VScore;

	Sprite Background;
	Sprite TextBackground;

	TrumpSelect TSBubble;

private:

    bool themeReady;
};
