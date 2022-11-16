#pragma once

#include "Buttons.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "Defines.h"
#include "ParentImage.h"

class TrumpSelect
{
public:
	TrumpSelect(void);
	~TrumpSelect(void);

	void show(void);
	void hide(void);
	void setSize(float x, float y = -1.0f);
	void setPriority(unsigned short int value);
	void display(float x, float y);
	bool animation(short int button, short int stage);
	void setup(AnimatedSprite *Symbol, const char filename[]);

	Buttons Hearts; //1
	Buttons Clubs; //2
	Buttons Diamonds; //3
	Buttons Spades; //4

	Sprite Frame;

private:
	char currentDir[32];

    Sprite Parent;
};
