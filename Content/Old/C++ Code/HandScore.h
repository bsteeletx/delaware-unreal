#pragma once

#include "Sprite.h"
#include "Numbers.h"
#include "AnimatedSprite.h"
#include "Defines.h"
#include "ParentImage.h"

class HandScore
{
public:
	HandScore(void);
	~HandScore(void);

	void hide(void);
	void setDepth(short unsigned int value);
	void setup(AnimatedSprite *Symbol);
	void setNumberData(void);
	void reset(void);

	Sprite Background;
	Sprite HCheck;
	Sprite HCheckOn;
	Sprite VCheck;
	Sprite VCheckOn;
	Numbers HandScoreData[2][4];
    ParentImage Parent;


private:
};
