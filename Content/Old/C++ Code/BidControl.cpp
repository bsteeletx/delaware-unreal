#include "BidControl.h"
#include "agk.h"


BidControl::BidControl()
{

}

BidControl::~BidControl(void)
{
}

void BidControl::setup(AnimatedSprite *Symbol, const char image[], const char dir[])
{
    //int frameImageNum;

	Parent = Sprite(Text(image), false);
    //Parent.setImage(image);
	Frame = Sprite(Parent.getImageNumber(), Text("control_bid_bubble.png"));
    //frameImageNum = Parent.getImageNumber("control_bid_bubble.png");

	Bid.setup(Symbol, "bid", Parent);
	Minus.setup(Symbol, "minus", Parent);
	Pass.setup(Symbol, "pass", Parent);
	Plus.setup(Symbol, "plus", Parent);

	Symbol->incrementFrame();
	agk::Sync();

   // Frame.setImageNumber(frameImageNum);

	Symbol->incrementFrame();
	agk::Sync();

	BidValue.setData(dir);

	Symbol->incrementFrame();
	agk::Sync();
}

void BidControl::show(void)
{
	Frame.setVisible(true);

	Bid.ButtonUp.setVisible(true);
	Minus.ButtonUp.setVisible(true);
	Pass.ButtonUp.setVisible(true);
	Plus.ButtonUp.setVisible(true);

	BidValue.showAll();
}

void BidControl::hide(void)
{
	Frame.setVisible(false);

	Bid.ButtonUp.setVisible(false);
	Minus.ButtonUp.setVisible(false);
	Pass.ButtonUp.setVisible(false);
	Plus.ButtonUp.setVisible(false);

	BidValue.hideAll();
}

void BidControl::setSize(float x, float y)
{
	Frame.setSize(x, y);
	Bid.setSize(x, y);
	Minus.setSize(x, y);
	Pass.setSize(x, y);
	Plus.setSize(x, y);
}

void BidControl::setPriority(short unsigned int value)
{
	Frame.setDepth(value);

	Bid.setDepth(value - 1);
	Minus.setDepth(value - 1);
	Pass.setDepth(value - 1);
	Plus.setDepth(value - 1);
}

void BidControl::display(float x, float y)
{
	Frame.setPosition(x, y);

	Bid.display(x, y);
	Minus.display(x, y);
	Pass.display(x, y);
	Plus.display(x, y);
}

bool BidControl::isVisible(void)
{
	if (Frame.getVisible())
		return true;

	return false;
}

bool BidControl::animation(short int menuChoice, short int stage)
{
	switch (menuChoice)
	{
	case 1:
		return Bid.animation(stage);
		break;
	case 2:
		return Minus.animation(stage);
		break;
	case 3:
		return Pass.animation(stage);
		break;
	case 4:
		return Plus.animation(stage);
		break;
	default:
		return false;
		break;
	}
}
