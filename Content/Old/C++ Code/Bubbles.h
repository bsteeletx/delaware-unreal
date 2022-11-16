#pragma once

#include "sprite.h"
#include "Numbers.h"
#include "AnimatedSprite.h"
#include "BidGame.h"
#include "Defines.h"
#include "ParentImage.h"

class Bubbles
{
public:
	Bubbles(void);
	~Bubbles(void);

	float getFirstCardPos(int theme, int player, bool getX);
	void show(short int meldBubble);
	void setBidSize(float x, float y = -1.0f);
	void hide(void);
	void setDepth(short unsigned int value);
	void setup(AnimatedSprite *Symbol, const char fullname[], const char dir[]);
	bool updateBid(short int turn, short int dealer, BidGame NewBidGame, short int theme);

	Numbers Bid;

	Sprite BidPass[4];
	Sprite BidBubble[4];
	Sprite MeldBubble[4];

private:
	char currentDir[64];
	float firstCardPos[NUM_TABLE_TOP][4][2];

    Sprite Parent;
};
