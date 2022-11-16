#include "TrumpSelect.h"
#include "agk.h"

TrumpSelect::TrumpSelect(void)
{

}

TrumpSelect::~TrumpSelect(void)
{
}

void TrumpSelect::setup(AnimatedSprite *Symbol, const char filename[])
{
    //int imgNum;
    Parent = Sprite(Text(filename));

	Hearts.setup(Symbol, "hearts_selection", Parent);
	Clubs.setup(Symbol, "clubs_selection", Parent);
	Diamonds.setup(Symbol, "diamonds_selection", Parent);
	Spades.setup(Symbol, "spades_selection", Parent);

	Frame = Sprite(Parent.getImageNumber(), Text("trump_selection_bubble.png"));
    
	Symbol->incrementFrame();
	agk::Sync();

	display(28.0f, 30.0f);
	hide();
}

void TrumpSelect::show(void)
{
	Hearts.show();
	Clubs.show();
	Diamonds.show();
	Spades.show();
	Frame.setVisible(true);
}

void TrumpSelect::hide(void)
{
	Hearts.hide();
	Clubs.hide();
	Diamonds.hide();
	Spades.hide();
	Frame.setVisible(false);
}

void TrumpSelect::setSize(float x, float y)
{
	Frame.setSize(x, y);

	Hearts.setSize(x, y);
	Clubs.setSize(x, y);
	Diamonds.setSize(x, y);
	Spades.setSize(x, y);
}

void TrumpSelect::setPriority(unsigned short int value)
{
	Hearts.setDepth(value + 1);
	Clubs.setDepth(value + 1);
	Diamonds.setDepth(value + 1);
	Spades.setDepth(value + 1);

	Frame.setDepth(value);
}

void TrumpSelect::display(float x, float y)
{
	Hearts.display(x, y);
	Clubs.display(x, y);
	Diamonds.display(x, y);
	Spades.display(x, y);

	Frame.setPosition(x, y);
}

bool TrumpSelect::animation(short int button, short int stage)
{
	switch (button)
	{
	case 1:
		return Hearts.animation(stage);
		break;
	case 2:
		return Clubs.animation(stage);
		break;
	case 3:
		return Diamonds.animation(stage);
		break;
	case 4:
		return Spades.animation(stage);
		break;
	default:
		return false;
		break;
	}
}
