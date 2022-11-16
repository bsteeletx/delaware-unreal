#include "Buttons.h"
#include "agk.h"

//Sound Buttons::ButtonClick;
//int Sound::volume = 100;

Buttons::Buttons(void)
{
	soundPlaying = false;
}


Buttons::~Buttons(void)
{
}

void Buttons::setSound(bool playSound)
{
	if (playSound)
		ButtonClick.setSystemVolume(100);
	else
		ButtonClick.setSystemVolume(0);
}

bool Buttons::animation(short int stage)
{
	switch (stage)
	{
	case 1:
		ButtonDown.setVisible(true);
		ButtonUp.setVisible(false);
		ButtonOver.setVisible(false);
		if (!soundPlaying)
		{
			ButtonClick.play();
			soundPlaying = true;
		}
		return false;
		break;
	case 2:
		ButtonDown.setVisible(false);
		ButtonUp.setVisible(true);
		soundPlaying = false;
		return false;
		break;
	}

	return true;
}

void Buttons::display(float x, float y)
{
	if (agk::GetSpriteExists(ButtonUp.getSpriteNumber()))
		ButtonUp.setPosition(x, y);
	if (agk::GetSpriteExists(ButtonDown.getSpriteNumber()))
		ButtonDown.setPosition(x, y);
	if (agk::GetSpriteExists(ButtonOver.getSpriteNumber()))
		ButtonOver.setPosition(x, y);
	if (agk::GetSpriteExists(ButtonGray.getSpriteNumber()))
		ButtonGray.setPosition(x, y);
}

void Buttons::hide(void)
{
	if (agk::GetSpriteExists(ButtonUp.getSpriteNumber()))
		ButtonUp.setVisible(false);
	if (agk::GetSpriteExists(ButtonDown.getSpriteNumber()))
		ButtonDown.setVisible(false);
	if (agk::GetSpriteExists(ButtonOver.getSpriteNumber()))
		ButtonOver.setVisible(false);
	if (agk::GetSpriteExists(ButtonGray.getSpriteNumber()))
		ButtonGray.setVisible(false);
}

void Buttons::show(void)
{
	if (agk::GetSpriteExists(ButtonUp.getSpriteNumber()))
		ButtonUp.setVisible(true);
}

void Buttons::setSize(float x, float y)
{
	if (agk::GetSpriteExists(ButtonUp.getSpriteNumber()))
		ButtonUp.setSize(x, y);
	if (agk::GetSpriteExists(ButtonDown.getSpriteNumber()))
		ButtonDown.setSize(x, y);
	if (agk::GetSpriteExists(ButtonOver.getSpriteNumber()))
		ButtonOver.setSize(x, y);
	if (agk::GetSpriteExists(ButtonGray.getSpriteNumber()))
		ButtonGray.setSize(x, y);
}

void Buttons::setDepth(short unsigned int value)
{
	if (agk::GetSpriteExists(ButtonUp.getSpriteNumber()))
		ButtonUp.setDepth(value);
	if (agk::GetSpriteExists(ButtonDown.getSpriteNumber()))
		ButtonDown.setDepth(value);
	if (agk::GetSpriteExists(ButtonOver.getSpriteNumber()))
		ButtonOver.setDepth(value);
	if (agk::GetSpriteExists(ButtonGray.getSpriteNumber()))
		ButtonGray.setDepth(value);
}

void Buttons::addFileExtension(char filebase[], short int buttonType)
{
	switch(buttonType)
	{
	case 1:
		strcat(filebase, "_off.png");
		break;
	case 2:
		strcat(filebase, "_down.png");
		break;
	case 3:
		strcat(filebase, "_on.png");
		break;
	case 4:
		strcat(filebase, "_negative.png");
		break;
	}
}

void Buttons::setData(const char newFilename[], const char newDir[])
{
	char temp[64];

	if (strlen(newFilename) >= 60)
		return;

	for (short int i = 0; i < 4; i++)
	{
		strcpy(temp, newFilename);

		addFileExtension(temp, i+1);

		switch (i)
		{
		case 0:
			ButtonUp = Sprite(Text(temp));
			break;
		case 1:
			ButtonDown = Sprite(Text(temp));
			break;
		case 2:
			ButtonOver = Sprite(Text(temp));
			break;
		case 3:
			ButtonGray = Sprite(Text(temp));
		}
	}
}

void Buttons::setup(AnimatedSprite *Symbol, const char filename[])
{
	char temp[32];
 
	ButtonClick = Sound(Text("sounds/woodclick1.wav"));

	if (std::strlen(filename) > 32)
		return;

	strcpy(temp, filename);
	addFileExtension(temp, 1);
	ButtonUp = Sprite(Parent.getImageNumber(), Text(temp));

	Symbol->incrementFrame();
	agk::Sync();

	strcpy(temp, filename);
	addFileExtension(temp, 2);
    ButtonDown = Sprite(Parent.getImageNumber(), Text(temp));
	
	Symbol->incrementFrame();
	agk::Sync();

	strcpy(temp, filename);
	addFileExtension(temp, 3);
    ButtonOver = Sprite(Parent.getImageNumber(), Text(temp));

	Symbol->incrementFrame();
	agk::Sync();

	strcpy(temp, filename);
	addFileExtension(temp, 4);
    ButtonGray = Sprite(Parent.getImageNumber(), Text(temp));

	Symbol->incrementFrame();
	agk::Sync();
}

void Buttons::setup(AnimatedSprite *Symbol, const char filename[], Sprite ParentTemp)
{
    Parent = ParentTemp;

    setup(Symbol, filename);
}
