#pragma once

#include "Sprite.h"
#include "Buttons.h"
#include "Numbers.h"
#include "AnimatedSprite.h"
#include "Defines.h"
//#include "ParentImage.h"

class BidControl
{
public:
	BidControl();
	~BidControl(void);

	void show(void);
	void hide(void);
	void setPriority(short unsigned int value);
	void display(float x, float y);
	bool isVisible(void);
	bool animation(short int menuChoice, short int stage);
	void setup(AnimatedSprite *Symbol, const char image[], const char dir[]);
	void setSize(float x, float y = -1.0f);

	Sprite Frame;

	Buttons Bid; //1
	Buttons Minus; //2
	Buttons Pass; //3
	Buttons Plus; //4

	Numbers BidValue;

private:

    Sprite Parent;

};
