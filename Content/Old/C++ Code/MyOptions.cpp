#include "MyOptions.h"
#include "MyMenu.h"
#include "agk.h"

MyOptions::MyOptions(void)
{

}


MyOptions::~MyOptions(void)
{
}

void MyOptions::setup(AnimatedSprite *Symbol)
{
    Parent = Sprite(Text("default/Options.png"));

	unsigned int imgNum = Parent.getImageNumber();

	///step 1: setup or setData/////////////
	NewGame.setup(Symbol, "new_game", Parent);
	ResumeGame.setup(Symbol, "resume", Parent);

    //imgNum = Parent.getImageNumber("options_background.png");
	Background = Sprite(Parent.getImageNumber(), Text("options_background.png"));

    ThemeOption[GREEN] = Sprite(imgNum, Text("background_g"));
	ThemeOption[RED] = Sprite(imgNum, Text("background_r"));
	ThemeOption[ANCIENT] = Sprite(imgNum, Text("background_a"));
	ThemeOption[NOEL] = Sprite(imgNum, Text("background_x"));
	
	//imgNum = Parent.getImageNumber("background_a.jpg");
	//ThemeOption[4].setImageNumber(imgNum);

	//ThemeOption[3].setData(pImage, "background_f.jpg");
	SoundCheck.setData("sound_btn", Parent);
	Left.setup(Symbol, "left_arrow", Parent);
	Right.setup(Symbol, "right_arrow", Parent);

	Symbol->incrementFrame();
	agk::Sync();

	///step 2: setSize
	Background.setSize(100.0f);

#if (PLATFORM != MOBILE)
    {
		SoundCheck.Checked.setSize(37.5f);
        SoundCheck.NotChecked.setSize(37.5f);

    }
#else
    {
        SoundCheck.Checked.setSize(45.0f);
        SoundCheck.NotChecked.setSize(45.0f);
    }
#endif

	for (short int i = 0; i < NUM_TABLE_TOP; i++)
		ThemeOption[i].setSize(45.0f);

	NewGame.setSize(20.0f);
	ResumeGame.setSize(20.0f);

	Left.setSize(10.0f);
	Right.setSize(10.0f);

	Symbol->incrementFrame();
	agk::Sync();

	///step 3: set priority
	setPriority(100);

	Symbol->incrementFrame();
	agk::Sync();

	///step 4: set x, y location with display
#if (PLATFORM != MOBILE)
    {
        SoundCheck.Checked.setPosition(27.0f, 70.0f);
        SoundCheck.NotChecked.setPosition(27.0f, 70.0f);
        ResumeGame.display(65.0, 82.0);
        NewGame.display(40.0f, 82.0f);
    }
#else
    {
        SoundCheck.Checked.display(30.0f, 72.5f);
        SoundCheck.NotChecked.display(30.0f, 72.5f);
        ResumeGame.display(65.0, 83.0);
        NewGame.display(40.0f, 83.0f);
    }
#endif
    Background.setPosition(0.0f, 0.0f);
	Left.display(WEST_LOC_X, 39.0f);
	Right.display(EAST_LOC_X, 39.0f);

	for (short int i = 0; i < NUM_TABLE_TOP; i++)
		ThemeOption[i].setPosition(27.5f, 23.0f);

	Symbol->incrementFrame();
	agk::Sync();

	///step 5: hide
	hide();

	Symbol->incrementFrame();
	agk::Sync();

	///step 6 (optional): set imprint


}

void MyOptions::show(short int curStyle)
{
	Background.setVisible(true);

	ThemeOption[curStyle].setVisible(true);

	if (isMuted)
		SoundCheck.NotChecked.setVisible(true);
	else
		SoundCheck.Checked.setVisible(true);

	NewGame.ButtonUp.setVisible(true);
	ResumeGame.ButtonUp.setVisible(true);

	Left.show();
	Right.show();
	//MainMenu.ButtonUp.setVisible(true)
}

bool MyOptions::arrowAnimation(short stage, short button)
{
	bool val = false;

	switch(button)
	{
	case 5:
		val = Right.animation(stage);
		break;
	case 6:
		val = Left.animation(stage);
		break;
	}

	return val;
}

void MyOptions::mute(void)
{
	if (isMuted)
		return;

	isMuted = true;
	SoundCheck.NotChecked.setVisible(true);
	SoundCheck.Checked.setVisible(false);
	muteSound();
}

void MyOptions::unMute(void)
{
	if (!isMuted)
		return;

	isMuted = false;
	SoundCheck.Checked.setVisible(true);
	SoundCheck.NotChecked.setVisible(false);
	unMuteSound();
}

void MyOptions::setPriority(short unsigned int value)
{
	for (short int i = 0; i < 6; i++)
		ThemeOption[i].setDepth(value - 1);

	Background.setDepth(1);

	SoundCheck.Checked.setDepth(value - 1);
	SoundCheck.NotChecked.setDepth(value - 1);

	NewGame.setDepth(value - 2);
	ResumeGame.setDepth(value - 2);

	Left.setDepth(value - 1);
	Right.setDepth(value - 1);
}

void MyOptions::update()
{
	short int choice = 0;

	float mouseX = agk::GetPointerX();
	float mouseY = agk::GetPointerY();

	if (isMuted && SoundCheck.Checked.getVisible())
	{
		SoundCheck.Checked.setVisible(false);
		SoundCheck.NotChecked.setVisible(true);
	}
	else if (!isMuted && SoundCheck.NotChecked.getVisible())
	{
		SoundCheck.Checked.setVisible(true);
		SoundCheck.NotChecked.setVisible(false);
	}

	if ((mouseX >= 40.0f) && (mouseX <= 60.0f))
	{
		if((mouseY >= 83.0f) && (mouseY <= 94.0f))
			choice = 1;
	}

	if (ResumeGame.ButtonUp.getVisible())
	{
		if ((mouseX >= 65.0f) && (mouseX <= 85.0f))
		{
			if ((mouseY >= 80.0f) && (mouseY <= 92.0f))
				choice = 2;
		}
	}

	if (choice == 0)
	{
		NewGame.ButtonOver.setVisible(false);
		ResumeGame.ButtonOver.setVisible(false);
		return;
	}
	else if (choice == 1)
	{
		NewGame.ButtonOver.setVisible(true);
		ResumeGame.ButtonOver.setVisible(false);
	}
	else if (choice == 2)
	{
		ResumeGame.ButtonOver.setVisible(true);
		NewGame.ButtonOver.setVisible(false);
	}
}

/*void MyOptions::hide()
{
	NewGame.hide();
	ResumeGame.hide();
	//Background.hide();
}*/
