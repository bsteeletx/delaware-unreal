#pragma once

#include "Sprite.h"
#include "Numbers.h"
#include "AnimatedSprite.h"
#include "Defines.h"
#include "ParentImage.h"

class GameScore
{
public:
	GameScore(void);
	~GameScore(void);

	void hide(void);
	void setPriority(short unsigned int value);
	void setup(AnimatedSprite *Symbol, ParentImage *HandParent);
	void setNumberData(void);
	void reset(void);

	Sprite Background;
	Sprite Header;
	Sprite MiddleSection[11];
	Sprite Total;

	Numbers GameScoreData[12][3];

private:

    ParentImage Parent;

};
