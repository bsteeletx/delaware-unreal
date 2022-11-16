#pragma once
#include "sprite.h"
#include "Sound.h"
#include "AnimatedSprite.h"
#include "Defines.h"
//#include "ParentImage.h"

class Buttons
{
public:
	Buttons(void);
	~Buttons(void);

	void addFileExtension(char filebase[], short int buttonType);
	bool animation(short int stage);
	void display(float x, float y);
	void hide(void);
	void show(void);
	void setData(const char newFilename[], const char newDir[]);
	void setDepth(short unsigned int value);
	void setup(AnimatedSprite *Symbol, const char filename[]);
    void setup(AnimatedSprite *Symbol, const char filename[], Sprite ParentTemp);
	void setSize(float x, float y = -1.0f);
	void setSound(bool playSound);

	Sprite ButtonUp;
	Sprite ButtonDown;
	Sprite ButtonOver;
	Sprite ButtonGray;



private:

	static Sound ButtonClick;
    bool soundPlaying;

    Sprite Parent;

};
