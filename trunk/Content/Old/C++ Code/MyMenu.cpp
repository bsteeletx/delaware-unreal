#include "MyMenu.h"
#include "AGK.h"

MyMenu::MyMenu(void)
{

}


MyMenu::~MyMenu(void)
{
}

void MyMenu::setup(AnimatedSprite *Symbol)
{
    MenuParent = Sprite(Text("default/MainMenu.png"));

	///step 1: setup or setData/////////////
	NewGame.setup(Symbol, "new_game", MenuParent);
	Options.setup(Symbol, "options", MenuParent);
	ResumeGame.setup(Symbol, "resume", MenuParent);

	Symbol->incrementFrame();
	agk::Sync();

	///step 2: setSize
	NewGame.setSize(20.0f);
	Options.setSize(20.0f);
	ResumeGame.setSize(20.0f);

	Symbol->incrementFrame();
	agk::Sync();

	///step 3: set priority
	setDepth(1);

	Symbol->incrementFrame();
	agk::Sync();

	///step 4: set x, y with display
	Options.display(65.0, 80.0);
	ResumeGame.display(40.0, 80.0);
	NewGame.display(15.0, 80.0);

	Symbol->incrementFrame();
	agk::Sync();

	///step 5: hide

	///step 6 (optional): set imprint
}

void MyMenu::update()
{
	short int choice = 0;

	float mouseX = agk::GetPointerX();
	float mouseY = agk::GetPointerY();

	if ((mouseX >= 15) && (mouseX <= 35))
	{
		if((mouseY >= 81) && (mouseY <= 89))
			choice = 1;
	}

	if (ResumeGame.ButtonUp.getVisible())
	{
		if ((mouseX >= 40) && (mouseX <= 60))
		{
			if ((mouseY >= 81) && (mouseY <= 89))
				choice = 3;
		}
	}

	if ((mouseX >= 65) && (mouseX <= 85))
	{
		if ((mouseY >= 81) && (mouseY <= 89))
			choice = 4;
	}

	if (choice == 0)
	{
		NewGame.ButtonOver.setVisible(false);
		Options.ButtonOver.setVisible(false);
		ResumeGame.ButtonOver.setVisible(false);
		MainMenu.ButtonOver.setVisible(false);
		return;
	}

	if (choice == 1)
		NewGame.ButtonOver.setVisible(true);
	/*else
	{
		Options.ButtonOver.hide();
		ResumeGame.ButtonOver.hide();
	}*/

	if (choice == 4)
		Options.ButtonOver.setVisible(true);
	/*else
	{
		ResumeGame.ButtonOver.hide();
		NewGame.ButtonOver.hide();
	}*/

	if (choice == 3)
		ResumeGame.ButtonOver.setVisible(true);
	/*else
	{
		NewGame.ButtonOver.hide();
		Options.ButtonOver.hide();
	}*/
}

void MyMenu::muteSound(void)
{
	NewGame.setSound(false);
}

void MyMenu::unMuteSound(void)
{
	NewGame.setSound(true);
}

bool MyMenu::ButtonAnimation(short int stage, short int button)
{
	bool val = false;

	switch(button)
	{
	case 1:
		val = NewGame.animation(stage);
		break;
	case 2:
		val = ResumeGame.animation(stage);
		break;
	case 3:
		val = ResumeGame.animation(stage);
		break;
	case 4:
		val = Options.animation(stage);
		break;
	}

	return val;
}

void MyMenu::hide(void)
{
	NewGame.hide();
	ResumeGame.hide();
	Options.hide();

	//Background.hide();
}

void MyMenu::setDepth(short unsigned int value)
{
	//Background.setDepth(value);

	NewGame.setDepth(value - 1);
	ResumeGame.setDepth(value - 1);
	Options.setDepth(value - 1);
}
