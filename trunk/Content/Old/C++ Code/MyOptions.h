#pragma once

#include "MyMenu.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "Checkbox.h"
#include "Defines.h"
#include "Buttons.h"

const short int MAX_THEMES = 8;

class MyOptions :
	public MyMenu
{
public:
	MyOptions(void);
	~MyOptions(void);

	bool arrowAnimation(short stage, short button);
	void show(short int curStyle);
	void mute(void);
	void unMute(void);
	void setPriority(short unsigned int value);
	void setup(AnimatedSprite *Symbol);
	void update(void);
	//void hide(void);

	Sprite Background;
	Sprite ThemeOption[MAX_THEMES];
	Checkbox SoundCheck;
	Buttons Left;
	Buttons Right;
	//Buttons NewGame; //1
	//Buttons ResumeGame; //2

private:
	bool isMuted;

    Sprite Parent;
};
