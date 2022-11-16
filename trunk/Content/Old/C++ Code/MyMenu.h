#pragma once

#include "Buttons.h"
#include "AnimatedSprite.h"
#include "Defines.h"
#include "ParentImage.h"

class MyMenu
{
public:
	MyMenu(void);
	~MyMenu(void);

	bool ButtonAnimation(short int stage, short int button);
	void hide(void);
	void update(void);
	void setDepth(short unsigned int value);
	void setup(AnimatedSprite *Symbol);
	void muteSound(void);
	void unMuteSound(void);

	Buttons NewGame; //1
	Buttons MainMenu; //2
	Buttons ResumeGame; //3
	Buttons Options; //4
    Sprite MenuParent;

private:

	//Sprite Background;
};
