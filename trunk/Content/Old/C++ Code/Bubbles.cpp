#include "Bubbles.h"
#include "agk.h"

#define X 0
#define Y 1

Bubbles::Bubbles(void)
{


}


Bubbles::~Bubbles(void)
{
}

void Bubbles::setup(AnimatedSprite *Symbol, const char fullname[], const char dir[])
{
	float offset = 5.0f;

	Parent = Sprite(Text(fullname));
	
	BidBubble[EAST] = Sprite(Parent.getImageNumber(), Text("east_bid.png"));
	MeldBubble[EAST] = Sprite(Parent.getImageNumber(), Text("east_meld.png"));
	BidBubble[NORTH] = Sprite(Parent.getImageNumber(), Text("north_bid.png"));
	MeldBubble[NORTH] = Sprite(Parent.getImageNumber(), Text("north_meld.png"));
    BidBubble[SOUTH] = Sprite(Parent.getImageNumber(), Text("south_bid.png"));
	MeldBubble[SOUTH] = Sprite(Parent.getImageNumber(), Text("south_meld.png"));
	BidBubble[WEST] = Sprite(Parent.getImageNumber(), Text("west_bid.png"));
    MeldBubble[WEST] = Sprite(Parent.getImageNumber(), Text("west_meld.png"));

	Bid.setData(dir);

	for (short int i = 0; i < 4; i++)
		BidPass[i] = Sprite(Parent.getImageNumber(), Text("pass_bid.png"));

	///display--Needs to be offset!!
	if(! (strcmp (dir, "green/") || strcmp(dir, "red/") || strcmp(dir, "xmas/")))
		offset = 5.0f;

	BidBubble[WEST].setPosition(14.5f + offset, 12.0f);
	BidBubble[NORTH].setPosition(30.0f, 13.0f); //green felt doesn't need offset, do others?
	BidBubble[EAST].setPosition(46.0f - offset, 12.0f);

#if (PLATFORM != MOBILE)
        BidBubble[SOUTH].setPosition(30.0f, 35.0f); //green felt doesn't need offset, do others?
#else
        BidBubble[SOUTH].setPosition(30.0f, 35.0f - offset); //green felt doesn't need offset, do others?
#endif

	Symbol->incrementFrame();
	agk::Sync();

	hide();

#if (PLATFORM != MOBILE)
	firstCardPos[GREEN][WEST][X] = 32.5f;
	firstCardPos[GREEN][WEST][Y] = 36.75f;
	firstCardPos[GREEN][NORTH][X] = 30.0f;
	firstCardPos[GREEN][NORTH][Y] = 40.5f;
	firstCardPos[GREEN][EAST][X] = 29.5f;
	firstCardPos[GREEN][EAST][Y] = 37.0f;
	firstCardPos[GREEN][SOUTH][X] = 32.4f;
	firstCardPos[GREEN][SOUTH][Y] = 39.5f;

	firstCardPos[RED][WEST][X] = 36.5f;
	firstCardPos[RED][WEST][Y] = 32.5f;
	firstCardPos[RED][NORTH][X] = 28.0f;
	firstCardPos[RED][NORTH][Y] = 40.5f;
	firstCardPos[RED][EAST][X] = 28.25f;
	firstCardPos[RED][EAST][Y] = 40.5f;
	firstCardPos[RED][SOUTH][X] = 28.25f;
	firstCardPos[RED][SOUTH][Y] = 40.25f;

	firstCardPos[ANCIENT][WEST][X] = 27.5f;
	firstCardPos[ANCIENT][WEST][Y] = 33.75f;
	firstCardPos[ANCIENT][NORTH][X] = 24.75f;
	firstCardPos[ANCIENT][NORTH][Y] = 37.5f; //
	firstCardPos[ANCIENT][EAST][X] = 25.0f;
	firstCardPos[ANCIENT][EAST][Y] = 32.0f;
	firstCardPos[ANCIENT][SOUTH][X] = 28.25f;
	firstCardPos[ANCIENT][SOUTH][Y] = 38.0f; //

	firstCardPos[NOEL][WEST][X] = 31.5f; //bubble needs moving west
	firstCardPos[NOEL][WEST][Y] = 36.75f;
	firstCardPos[NOEL][NORTH][X] = 31.0f;
	firstCardPos[NOEL][NORTH][Y] = 36.5f;
	firstCardPos[NOEL][EAST][X] = 34.0;
	firstCardPos[NOEL][EAST][Y] = 37.0f;
	firstCardPos[NOEL][SOUTH][X] = 35.4f;
	firstCardPos[NOEL][SOUTH][Y] = 44.5f;

	/*firstCardPos[ELECTRIC][WEST][X] = 27.5f; //bubble needs moving west
	firstCardPos[ELECTRIC][WEST][Y] = 33.5f;
	firstCardPos[ELECTRIC][NORTH][X] = 28.0f;
	firstCardPos[ELECTRIC][NORTH][Y] = 41.0f;
	firstCardPos[ELECTRIC][EAST][X] = 32.0f;
	firstCardPos[ELECTRIC][EAST][Y] = 40.5f;
	firstCardPos[ELECTRIC][SOUTH][X] = 26.0f;
	firstCardPos[ELECTRIC][SOUTH][Y] = 40.0f;*/
#else
    firstCardPos[GREEN][WEST][X] = 32.5f;
	firstCardPos[GREEN][WEST][Y] = 40.5f; //
	firstCardPos[GREEN][NORTH][X] = 30.0f;
	firstCardPos[GREEN][NORTH][Y] = 45.0f; //
	firstCardPos[GREEN][EAST][X] = 29.5f;
	firstCardPos[GREEN][EAST][Y] = 40.5f; //
	firstCardPos[GREEN][SOUTH][X] = 32.4f;
	firstCardPos[GREEN][SOUTH][Y] = 38.5f;

	firstCardPos[RED][WEST][X] = 35.5f; //
	firstCardPos[RED][WEST][Y] = 36.75f; //
	firstCardPos[RED][NORTH][X] = 27.0f; //
	firstCardPos[RED][NORTH][Y] = 45.0f; //
	firstCardPos[RED][EAST][X] = 28.25f;
	firstCardPos[RED][EAST][Y] = 45.75f; //
	firstCardPos[RED][SOUTH][X] = 28.25f;
	firstCardPos[RED][SOUTH][Y] = 39.0f; //

	firstCardPos[ANCIENT][WEST][X] = 27.5f;
	firstCardPos[ANCIENT][WEST][Y] = 36.25f; //
	firstCardPos[ANCIENT][NORTH][X] = 24.75f;
	firstCardPos[ANCIENT][NORTH][Y] = 41.5f; //
	firstCardPos[ANCIENT][EAST][X] = 25.0f;
	firstCardPos[ANCIENT][EAST][Y] = 36.6f; //
	firstCardPos[ANCIENT][SOUTH][X] = 28.25f;
	firstCardPos[ANCIENT][SOUTH][Y] = 37.0f; //

	firstCardPos[NOEL][WEST][X] = 32.5f;
	firstCardPos[NOEL][WEST][Y] = 40.5f; //
	firstCardPos[NOEL][NORTH][X] = 30.0f;
	firstCardPos[NOEL][NORTH][Y] = 45.0f; //
	firstCardPos[NOEL][EAST][X] = 29.5f;
	firstCardPos[NOEL][EAST][Y] = 40.5f; //
	firstCardPos[NOEL][SOUTH][X] = 32.4f;
	firstCardPos[NOEL][SOUTH][Y] = 38.5f;

	/*firstCardPos[ELECTRIC][WEST][X] = 27.5f; //bubble needs moving west
	firstCardPos[ELECTRIC][WEST][Y] = 41.0f;
	firstCardPos[ELECTRIC][NORTH][X] = 28.0f;
	firstCardPos[ELECTRIC][NORTH][Y] = 40.0f;
	firstCardPos[ELECTRIC][EAST][X] = 39.0f;
	firstCardPos[ELECTRIC][EAST][Y] = 47.5f;
	firstCardPos[ELECTRIC][SOUTH][X] = 32.0f;
	firstCardPos[ELECTRIC][SOUTH][Y] = 47.5f;*/
#endif

	/*firstCardPos[FUTURE][WEST][X] = 27.5f; //bid bubble needs moving south and west
	firstCardPos[FUTURE][WEST][Y] = 41.0f;
	firstCardPos[FUTURE][NORTH][X] = 28.0f;
	firstCardPos[FUTURE][NORTH][Y] = 40.0f;
	firstCardPos[FUTURE][EAST][X] = 36.0f;
	firstCardPos[FUTURE][EAST][Y] = 47.5f;
	firstCardPos[FUTURE][SOUTH][X] = 32.0f;
	firstCardPos[FUTURE][SOUTH][Y] = 47.5f;

	*/
}

float Bubbles::getFirstCardPos(int theme, int player, bool getX)
{
	if (getX)
		return firstCardPos[theme][player][X];
	else
		return firstCardPos[theme][player][Y];
}

void Bubbles::setBidSize(float x, float y)
{
	for (int i = 0; i < 4; i++)
	{
		BidBubble[i].setSize(x, y);
		BidPass[i].setSize(x/2.0f, y/2.0f);
	}
}

void Bubbles::hide(void)
{
	for (int i = 0; i < 4; i++)
	{
		BidBubble[i].setVisible(false);
		MeldBubble[i].setVisible(false);
		BidPass[i].setVisible(false);
	}

	Bid.hideAll();
}

void Bubbles::setDepth(short unsigned int value)
{
	for (int i = 0; i < 4; i++)
	{
		BidBubble[i].setDepth(value);
		MeldBubble[i].setDepth(value);
		BidPass[i].setDepth(value - 1);
	}

	Bid.setDepth(value - 1);
}

void Bubbles::show(short int meldBubble)
{
	MeldBubble[meldBubble].setVisible(true);
}

bool Bubbles::updateBid(short int turn, short int dealer, BidGame NewBidGame, short int theme)  //returns true if bid, false if not
{
	float x = 0.0f;
	float y = 0.0f;
	//bool passed = false;
	short int round = (turn - dealer - 1) / 4;
	unsigned short int bid = NewBidGame.getBid();
	float offset = 0.0f;

	if (theme != 4)
		offset = 5.0f;

	if (NewBidGame.getBidder() == -1)
	{
		BidBubble[turn%4].setVisible(true);
		agk::Sync();

		return true;
	}

	if ((!NewBidGame.getPass(turn%4)) && (!NewBidGame.getBidDecided()))
		BidBubble[turn%4].setVisible(true);

	if (NewBidGame.getPass(turn%4))
	{
		if (NewBidGame.passRound[turn%4] == round)
		{
			bid = 0;
			BidBubble[turn%4].setVisible(true);
		}
		else
		{
			BidBubble[turn%4].setVisible(false);
			return false;
		}
	}

	switch (turn % 4)
	{
	case WEST:
		if (bid)
		{//bid
#if (PLATFORM != MOBILE)
			x = 38.0f + offset;
			y = 32.0f;
#else
            x = 38.0f + offset;
            y = 35.0f;
#endif
			if (theme == ELECTRIC)
				x += 5.0f;
			else if (theme == NOEL)
			{
				x -= 1.0f;
				y += 1.0f;
			}
		}
		else
		{//pass
			x = 26.0f + offset;
			y = 25.0f;

			if (theme == ELECTRIC)
				x += 5.0f;
			else if (theme == NOEL)
				x -= 1.0f;
		}
		break;
	case NORTH:
		if (bid)
		{//did not pass
#if (PLATFORM != MOBILE)
			x = 52.5f;
			y = 31.5f + offset;
#else
            x = 52.5f;
            y = 34.5f + offset;
#endif
			if (theme == NOEL)
				y -= 2.5f;
		}
		else
		{
#if (PLATFORM != MOBILE)
			x = 35.25f + offset;
			y = 29.0f;
#else
            x = 35.25f + offset;
            y = 30.0f;
#endif
			if (theme == NOEL)
			{
				x += 0.0f;
				y -= 3.0f;
			}
		}
		break;
	case EAST:
		if (bid)
		{
#if (PLATFORM != MOBILE)
			x = 66.5f - offset;
			y = 32.0f;
#else
            x = 67.0f - offset;
            y = 35.0f;
#endif
			if (theme == NOEL)
			{
				y += 0.75f;
				x += 2.0f;
			}
		}
		else
		{
			x = 54.0f - offset;
			y = 25.0f;

			if (theme == NOEL)
				x += 2.5f;
		}

		break;
	case SOUTH:
		/*if (!NewBidGame.getPass(SOUTH) && (!NewBidGame.getBidDecided()))
		{
			SouthBid.setVisible(true)
		}

		if (NewBidGame.getPass(SOUTH))
		{
			if (NewBidGame.sPassRound == round)
			{
				bid = 0;
				SouthBid.setVisible(true)
			}
			else
			{
				SouthBid.hide();
				//passed = true;
				return false;
			}
		}*/

		if (bid)
		{
#if (PLATFORM != MOBILE)
			x = 52.5f;
			y = 58.0f - offset;
#else
            x = 52.5f;
            y = 55.0f - offset;
#endif
      		if (theme == ELECTRIC)
				x += 1.0f;
			else if (theme == NOEL)
				y += 3.0f;
		}
		else
		{
#if (PLATFORM != MOBILE)
			x = 40.0f;
			y = 51.0f - offset;
#else
            x = 40.0f;
            y = 45.0f - offset;
#endif
			if (theme == NOEL)
			{
				x += 1.0f;
				y += 2.5f;
			}
		}
		break;
	}

	if (bid)
	{//CurrentTheme.Bid.setSize(0.30f);

		if (bid >= 100)
			Bid.display(2, 1, x-12.0f, y);

        if (theme != 1)// && (theme != 3))
        {
            Bid.display(1, bid%100/10, x-6.0f, y);
            Bid.display(0, bid%10, x, y);
        }
        else// if (theme == 1)
        {
            Bid.display(1, bid%100/10, x-6.5f, y);
            Bid.display(0, bid%10, x, y);
        }

		return true;
	}
	else
	{
		BidPass[turn%4].setPosition(x, y);
		BidPass[turn%4].setVisible(true);

		return true;
	}
}

