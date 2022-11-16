#include "Card.h"
#include "template.h"

Card::Card()
{
	isRotated = false;
	rotateAmount = 0.0f;
    cardBackImageNumber[0] = 0;
    cardBackImageNumber[1] = 0;
    cardBackImageNumber[2] = 0;
}

Card::Card(short unsigned int frontImageNumber, short unsigned int backImageNumber)
{
	isRotated = false;
	rotateAmount = 0.0f;
}

Card::~Card(void)
{
}

unsigned int Card::getFrontImage()
{
    return Front.getImageNumber();
}

unsigned int Card::getBackImage()
{
    return Back.getImageNumber();
}

unsigned int Card::getOutlineImage()
{
    return Outlined.getImageNumber();
}

void Card::setFrontImage(unsigned int frontImage)
{
	Front.setImage(frontImage);
}

void Card::setBackImage(unsigned int backImage)
{
    Back.setImage(backImage);
}

void Card::setBackImage(unsigned int backImage, short place)
{
    Back.setImage(backImage);
    cardBackImageNumber[place] = backImage;
}

void Card::setOutlineImage(unsigned int outlineImage)
{
    Outlined.setImage(outlineImage);
}

bool Card::compareBackImageTo(unsigned int image)
{
    for (int i = 0; i < 3; i++)
    {
        if (image == cardBackImageNumber[i])
            return true;
    }

    return false;
}

void Card::setData(char dir[], short int rankNum, short int suitNum)
{
	char sRank[8];
	char  sSuit[16];
	char temp[32];
	char outline[32];
	char back[32] = {NULL};
    int imgNum;
    short place = 0;

    if (!rankNum && !suitNum)
        Parent.setImage(dir);

    getRank(sRank, rankNum);
	getSuit(sSuit, suitNum);

	strcpy(temp, sRank);
	strcat(temp, "_");
	strcat(temp, sSuit);
	strcpy(outline, temp);
	strcat(temp, ".png");
	strcat(outline, "m.png");

    imgNum = Parent.getImageNumber(temp);
	Front.setImage(imgNum);

	strcpy(temp, dir);
	strcat(temp, "card_back2.png");
	if (agk::GetFileExists(temp))
	{
#if (OS == IOS)
		short int tempNum = arc4random() % 3;
#else
		short tempNum = agk::Random(0, 2);
#endif
		char buf[2];

        multipleBack = true;

		strcpy(back, "card_back");

		if (tempNum != 0)
		{
			sprintf(buf, "%d", tempNum+1);
			strcat(back, buf);
		}

        place = atoi(buf);
	}
	else
    {
		strcpy(back, "card_back");
        multipleBack = false;
    }

	strcat(back, ".png");

    imgNum = Parent.getImageNumber(back);

    if (!compareBackImageTo(imgNum))
        setBackImage(imgNum, place);

    imgNum = Parent.getImageNumber(outline);
	Outlined.setImage(imgNum);

	rank = rankNum;
	suit = suitNum;
}

bool Card::getOutline(void)
{
	return isTrump;
}

bool Card::getFaceDirection(void)
{
	return isFaceDown;
}

void Card::getSuit(char buffer[], short int suitNum)
{
	switch(suitNum)
	{
	case 0:
		strcpy(buffer, "hearts");
		break;
	case 1:
		strcpy(buffer, "clubs");
		break;
	case 2:
		strcpy(buffer, "diamonds");
		break;
	case 3:
		strcpy(buffer, "spades");
		break;
	}

}

void Card::getRank(char buffer[], short int rankNum)
{
	switch(rankNum)
	{
	case 0:
		strcpy(buffer, "ace");
		break;
	case 1:
		strcpy(buffer, "10");
		break;
	case 2:
		strcpy(buffer, "king");
		break;
	case 3:
		strcpy(buffer, "queen");
		break;
	case 4:
		strcpy(buffer, "jack");
		break;
	}
}

void Card::turnFaceDown(void)
{
	Front.setVisible(false);
	Outlined.setVisible(false);
	Back.setVisible(true);

	isFaceDown = true;
}

void Card::turnFaceUp(void)
{
	Back.setVisible(false);

	Outlined.setVisible(isTrump);
	Front.setVisible(!isTrump);

	isFaceDown = false;
}

void Card::rotateLeft()
{
	agk::SetSpriteAngle(Front.getSpriteNumber(), 270.0f);
	agk::SetSpriteAngle(Back.getSpriteNumber(), 270.0f);
	agk::SetSpriteAngle(Outlined.getSpriteNumber(), 270.0f);
	rotateAmount = 270.0f;
	isRotated = true;
	rotatedLeft = true;
}

void Card::rotateRight()
{
	agk::SetSpriteAngle(Front.getSpriteNumber(), 90.0f);
	agk::SetSpriteAngle(Back.getSpriteNumber(), 90.0f);
	agk::SetSpriteAngle(Outlined.getSpriteNumber(), 90.0f);
	rotateAmount = 90.0f;
	isRotated = true;
	rotatedRight = true;
}

float Card::getRotationAmount(void)
{
	return rotateAmount;
}

void Card::unRotate()
{
	agk::SetSpriteAngle(Front.getSpriteNumber(), 0.0f);
	agk::SetSpriteAngle(Back.getSpriteNumber(), 0.0f);
	agk::SetSpriteAngle(Outlined.getSpriteNumber(), 0.0f);
	isRotated = false;
	rotatedLeft = false;
	rotatedRight = false;
}

bool Card::getIsRotated()
{
	return isRotated;
}

float Card::getX(void)
{
	return Front.getX();
}

float Card::getY(void)
{
	return Front.getY();
}

bool Card::updateShrinkCloseTrick(short int location, Card *Card2, Card *Card3, Card *Card4)
{
	float x[4] = {getX(), Card2->getX(), Card3->getX(), Card4->getX()};
	float y[4] = {getY(), Card2->getY(), Card3->getY(), Card4->getY()};
	float newX;
	float newY;
	float moveXperTurn[4];
	float moveYperTurn[4];
	bool offScreen[4] = {false, false, false, false};
	short int speed = 4;

	switch (location)
	{
	case WEST:
		newX = -110.0f;
		newY = 50.0f;
		break;
	case NORTH:
		newX = 50.0f;
		newY = -110.0f;
		break;
	case EAST:
		newX = 110.0f;
		newY = 50.0f;
		break;
	case SOUTH:
		newX = 50.0f;
		newY = 110.0f;
		break;
	default:
		newX = 50.0f;
		newY = 50.0f;
		break;
	}

	for (short int i = 0; i < 4; i++)
	{
		if (newX < x[i])
		{//have to go negative to reach destination
			if ((x[i] - newX) / 4 > speed)
				moveXperTurn[i] = (newX - x[i]) / 4; //normal movement rate, negative
			else if (x[i] - speed >= newX)
				moveXperTurn[i] = (float)(-speed); //slower movement rate
			else
				moveXperTurn[i] = newX - x[i]; //even out to destination, will be negative
		}
		else
		{ //have to go positive to reach destination
			if ((newX - x[i]) / 4 > speed)
				moveXperTurn[i] = (newX - x[i]) / 4; //nomal movement rate
			else if (x[i] + speed <= newX)
				moveXperTurn[i] = (float)(speed);
			else
				moveXperTurn[i] = newX - x[i];
		}

		if (newY < y[i])
		{
			if ((y[i] - newY) / 4 > speed)
				moveYperTurn[i] = (newY - y[i]) / 4;
			else if (y[i] - speed >= newY)
				moveYperTurn[i] = (float)(-speed);
			else
				moveYperTurn[i] = newY - y[i];
		}
		else
		{
			if ((newY - y[i]) / 4 > speed)
				moveYperTurn[i] = (newY - y[i]) / 4;
			else if (y[i] + speed <= newY)
				moveYperTurn[i] = (float)(speed);
			else if (y[i] + speed > newY)  //don't need to check if it's less than or equal to because it's default at 10
				moveYperTurn[i] = newY - y[i];
		}
	}

	if ((x[0] > 100) || (x[0] < 0) || (y[0] > 100) || (y[0] < 0))
	{
		offScreen[0] = true;
		hide();
	}
	else
		display(x[0] + moveXperTurn[0], y[0] + moveYperTurn[0]);

	if ((x[1] > 100) || (x[1] < 0) || (y[1] > 100) || (y[1] < 0))
	{
		offScreen[1] = true;
		Card2->hide();
	}
	else
		Card2->display(x[1] + moveXperTurn[1], y[1] + moveYperTurn[1]);

	if ((x[2] > 100) || (x[2] < 0) || (y[2] > 100) || (y[2] < 0))
	{
		offScreen[2] = true;
		Card3->hide();
	}
	else
		Card3->display(x[2] + moveXperTurn[2], y[2] + moveYperTurn[2]);

	if ((x[3] > 100) || (x[3] < 0) || (y[3] > 100) || (y[3] < 0))
	{
		offScreen[3] = true;
		Card4->hide();
	}
	else
		Card4->display(x[3] + moveXperTurn[3], y[3] + moveYperTurn[3]);


	if (offScreen[0] && offScreen[1] && offScreen[2] && offScreen[3])
		return true;
	else
		return false;
}

bool Card::isRotatedLeft(void)
{
	return rotatedLeft;
}

bool Card::isRotatedRight(void)
{
	return rotatedRight;
}

void Card::rotate(float amount)
{
	isRotated = true;

	rotateAmount += amount;

	agk::SetSpriteAngle(Front.getSpriteNumber(), rotateAmount);
	agk::SetSpriteAngle(Back.getSpriteNumber(), rotateAmount);
	agk::SetSpriteAngle(Outlined.getSpriteNumber(), rotateAmount);
}

void Card::raiseHeight()
{
	agk::SetSpriteY(Front.getSpriteNumber(), (float)(SOUTH_LOC_Y - (100.0f*(50.0f/1024.0f))));
	agk::SetSpriteY(Outlined.getSpriteNumber(), (float)(SOUTH_LOC_Y - (100.0f*(50.0f/1024.0f))));
	agk::SetSpriteY(Back.getSpriteNumber(), (float)(SOUTH_LOC_Y - (100.0f*(50.0f/1024.0f))));
}

void Card::resetHeight(void)
{
	agk::SetSpriteY(Front.getSpriteNumber(), (float) SOUTH_LOC_Y);
	agk::SetSpriteY(Back.getSpriteNumber(), (float) SOUTH_LOC_Y);
	agk::SetSpriteY(Outlined.getSpriteNumber(), (float) SOUTH_LOC_Y);
}

void Card::unOutlineCard(void)
{
	isTrump = false;
	Outlined.setVisible(false);

	Front.setVisible(!isFaceDown);
}

void Card::outlineCard(void)
{
	Front.setVisible(false);

	Outlined.setVisible(!isFaceDown);
	
	isTrump = true;
}

void Card::revealCard(Player Selected)
{
	short int card = 0;

	for (short int i = 0; i < 20; i++)
	{
		card = Selected.getCard(i);

		if (card == 99)
			break;
		else
			turnFaceUp();
	}
}

void Card::sortVisualCards(Player West, Player North, Player East, Player South, Card *vDeck, short int round, short theme)
{
	short int i = 0;
    float offset;

    if (theme == 0)
        offset = 0.5f;
    else
        offset = 0.0f;

	while (i != 20-(round+1))
	{
		if (!vDeck[West.getCard(i)].getIsRotated())
		{
			vDeck[West.getCard(i)].rotateRight();
			vDeck[West.getCard(i)].turnFaceDown();
		}

		if (!vDeck[East.getCard(i)].getIsRotated())
		{
			vDeck[East.getCard(i)].rotateLeft();
			vDeck[East.getCard(i)].turnFaceDown();
		}

		if (vDeck[South.getCard(i)].getIsRotated())
		{
			vDeck[South.getCard(i)].unRotate();
			vDeck[South.getCard(i)].turnFaceUp();
		}

		vDeck[North.getCard(i)].turnFaceDown();

		if (i == 0)
		{
			vDeck[West.getCard(i)].display(WEST_LOC_X, WEST_LOC_Y + (-0.5f * CARDWIDTHH));
			vDeck[North.getCard(i)].display(NORTH_LOC_X - CARDWIDTHN, NORTH_LOC_Y);
			vDeck[East.getCard(i)].display(EAST_LOC_X, EAST_LOC_Y - (-0.5f * CARDWIDTHH));
			vDeck[South.getCard(i)].display(SOUTH_LOC_X, SOUTH_LOC_Y);
		}
		else
		{
			vDeck[West.getCard(i)].display(WEST_LOC_X, WEST_LOC_Y + (((1.5f*i)-0.5f) * CARDWIDTHH));
			vDeck[North.getCard(i)].display(NORTH_LOC_X - ((i+1) * CARDWIDTHN), NORTH_LOC_Y);
			vDeck[East.getCard(i)].display(EAST_LOC_X, EAST_LOC_Y - (((1.5f*i)-0.5f) * CARDWIDTHH));
			vDeck[South.getCard(i)].display(SOUTH_LOC_X + (i * CARDWIDTHS), SOUTH_LOC_Y);
		}

		vDeck[West.getCard(i)].setDepth(i - 3);
		vDeck[North.getCard(i)].setDepth(i - 3);
		vDeck[East.getCard(i)].setDepth(i - 3);
		vDeck[South.getCard(i)].setDepth(i - 3);

		i++;
	}

    if (theme == 0)
    {
        for (short j = 0; j < 20; j++)
        {
            float newX = vDeck[North.getCard(j)].getX();
            float curY = vDeck[North.getCard(j)].getY();

            vDeck[North.getCard(j)].display(newX + offset, curY);
        }
    }
}

void Card::sortVisualCards(Player Selected, Card *vDeck, short theme)
{
	short int playerID = Selected.getID();
	short int maxCards = Selected.getCardsInHand();
    float offset = 0.0f;

    if (theme == 0)
        offset = 0.5f;

	for (short int i = 0; i < maxCards; i++)
	{
		if (playerID == WEST)
			vDeck[Selected.getCard(i)].display(WEST_LOC_X, WEST_LOC_Y + ((10 - (maxCards / 2)) * CARDWIDTHH) + (((1.5f*i)-0.5f) * CARDWIDTHH));
		else if (playerID == NORTH)
			vDeck[Selected.getCard(i)].display(NORTH_LOC_X - ((10 - (maxCards / 2)) * CARDWIDTHN) - ((i+1) * CARDWIDTHN) + offset, NORTH_LOC_Y);
		else if (playerID == EAST)
			vDeck[Selected.getCard(i)].display(EAST_LOC_X, EAST_LOC_Y - ((10 - (maxCards / 2)) * CARDWIDTHH) - (((1.5f*i)-0.5f) * CARDWIDTHH));
		else if (playerID == SOUTH)
			vDeck[Selected.getCard(i)].display((SOUTH_LOC_X + ((10 - (maxCards / 2)) * CARDWIDTHS)) + (i * CARDWIDTHS), SOUTH_LOC_Y);

		vDeck[Selected.getCard(i)].setDepth(i - 3);
	}

}

bool Card::updateCardLoc(short int turn, PinochleGame PGame, BidGame BGame, TrickGame TGame, short int count, bool flag)
{//sending one at a time to the center
	bool rotateRight = false;
	bool rotateLeft = false;
	float destinationX = 50.0f;
	float destinationY = 50.0f;
	float moveX;
	float moveY;
	float curXLocation = getX();
	float curYLocation = getY();
	float speed = 15.0f;
	float cardWidth = 1.76f;

	//offset(62.5f, 87.5f);

	if (turn != SOUTH)
		turnFaceUp();

	switch (turn)
	{
	case WEST:
		destinationX -= 20.0f;
		destinationY -= 10.0f;
		rotateRight = true;
		break;
	case NORTH:
		destinationX -= 7.8f;
		destinationY -= 20.0f;
		break;
	case EAST:
		destinationX += 5.0f;
		destinationY -= 10.0f;
		rotateLeft = true;
		break;
	case SOUTH:
		destinationX -= 7.8f;
		//destinationY += 10.0f;
		break;
	default:
		break;
	}

	if (destinationX < curXLocation)
	{//have to go negative to reach destination
		if ((curXLocation - destinationX) / speed > cardWidth)
			moveX = (destinationX - curXLocation) / speed; //normal movement rate, negative
		else if (curXLocation - cardWidth >= destinationX)
			moveX = -cardWidth; //slower movement rate
		else
			moveX = destinationX - curXLocation; //even out to destination, will be negative
	}
	else
	{ //have to go positive to reach destination
		if ((destinationX - curXLocation) / speed > cardWidth)
			moveX = (destinationX - curXLocation) / speed; //nomal movement rate
		else if (curXLocation + cardWidth <= destinationX)
			moveX = cardWidth;
		else
			moveX = destinationX - curXLocation;
	}

	if (destinationY < curYLocation)
	{
		if ((curYLocation - destinationY) / speed > cardWidth)
			moveY = (destinationY - curYLocation) / speed;
		else if (curYLocation - cardWidth >= destinationY)
			moveY = -cardWidth;
		else
			moveY = destinationY - curYLocation;
	}
	else
	{
		if ((destinationY - curYLocation) / speed > cardWidth)
			moveY = (destinationY - curYLocation) / speed;
		else if (curYLocation + cardWidth <= destinationY)
			moveY = cardWidth;
		else if (curYLocation + cardWidth > destinationY)  //don't need to check if it's less than or equal to because it's default at 10
			moveY = destinationY - curYLocation;
	}

#if (OS != IOS)
	rotate((float)(agk::Random(0, 45)+agk::Random(0, 45)));
#else
    rotate((float) ((arc4random() % 46) + (arc4random() % 46)));
#endif

	if (curXLocation != 0.0f)
		display(curXLocation + moveX, curYLocation + moveY);
	else
	{
		display(destinationX, destinationY);
		return true;
	}

	if ((fabsf(moveX) <= 0.000001f) && (fabsf(moveY) <= 0.00001f))
		return true;
	else
		return false;
}

bool Card::updateCardThrowIn(Player Selected, Card *Deck)
{//sending four to the center
	float destinationX[8] = {45.0f, 45.0f, 45.0f, 45.0f, 45.0f, 45.0f, 45.0f, 45.0f};
	float destinationY[8] = {40.0f, 40.0f, 40.0f, 40.0f, 40.0f, 40.0f, 40.0f, 40.0f};
	short int cardSprites[8] = {Selected.getCard(0), Selected.getCard(1), Selected.getCard(2), Selected.getCard(3), Selected.getCard(4), Selected.getCard(5), Selected.getCard(6), Selected.getCard(7)};
	float moveXperTurn[8] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
	float moveYperTurn[8] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
	float curXLocation[8] = {5.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f};
	float curYLocation[8] = {5.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f};
	bool atLocation[8] = {false, false, false, false, false, false, false, false};
	short turn = Selected.getID();
	short totalCards = Selected.getCardsInHand();
	float offset[8];
	float median = (float) (totalCards - 1) / 2; // 3: 1, 5: 2, 7: 3----------2:0.5, 4:1.5, 6:2.5
	short adj = 1;
	float speed = 16.0f;
	const float cardwidth = 1.76f;

	if (turn < 2)
		adj = -1;

	for (int i = 0; i < totalCards; i++)
	{
		if (totalCards == 1) //only happens when only one card left
		{
			offset[i] = 0.0f;
			break;
		}
		float divisor = median; // 3:1  5:2  7:3------------2:0.5, 4:1.5, 6:2.5
		float numerator = (float) (totalCards - 1); // 3:2, 5:4, 7:6--------------2:1, 4:3, 6:5
		offset[i] = 2.0f * (numerator * (i - median)) / divisor; // 3:(0:-2, 1:0, 2:2) 5:(0:-4, 1:-2, 2:0, 3:2, 4:4) 7:(0:-6, 1:-4...-----------2:(0:-1, 1:1), 4:(0:-3, 1:-1, 2:1, 3:3), 6:(0:-5, 1:...
		//above is spread between individual cards
	}

	if (turn % 2 == BLUE)
	{
		for (int i = 0; i < totalCards; i++)
		{
			if (turn == WEST)
				destinationX[i] = 25.0f;
			else
				destinationX[i] = 65.0f;

			destinationY[i] += offset[i] - 5.0f;
		}
	}
	else
	{
		for (int i = 0; i < totalCards; i++)
		{
			destinationX[i] += offset[i];

			if (turn == NORTH)
				destinationY[i] = 20.0f;
			else
				destinationY[i] = 60.0f;
		}
	}

	for (short int i = 0; i < totalCards; i++)
	{
		int cardSprite = Selected.getCard(i);

		curXLocation[i] = Deck[cardSprite].getX();
		curYLocation[i] = Deck[cardSprite].getY();

		if (destinationX[i] < curXLocation[i])
		{//have to go negative to reach destination
			if ((curXLocation[i] - destinationX[i]) / speed > cardwidth)
				moveXperTurn[i] = (destinationX[i] - curXLocation[i]) / speed; //normal movement rate, negative
			else if (curXLocation[i] - cardwidth >= destinationX[i])
				moveXperTurn[i] *= -2; //slower movement rate
			else
				moveXperTurn[i] = destinationX[i] - curXLocation[i]; //even out to destination, will be negative
		}
		else
		{ //have to go positive to reach destination
			if ((destinationX[i] - curXLocation[i]) / speed > cardwidth)
				moveXperTurn[i] = (destinationX[i] - curXLocation[i]) / speed; //nomal movement rate
			else if (curXLocation[i] + cardwidth <= destinationX[i])
				moveXperTurn[i] *= 2;
			else
				moveXperTurn[i] = destinationX[i] - curXLocation[i];
		}

		if (destinationY[i] < curYLocation[i])
		{
			if ((curYLocation[i] - destinationY[i]) / speed > cardwidth)
				moveYperTurn[i] = (destinationY[i] - curYLocation[i]) / speed;
			else if (curYLocation[i] - cardwidth >= destinationY[i])
				moveYperTurn[i] *= -2;
			else
				moveYperTurn[i] = destinationY[i] - curYLocation[i];
		}
		else
		{
			if ((destinationY[i] - curYLocation[i]) / speed > cardwidth)
				moveYperTurn[i] = (destinationY[i] - curYLocation[i]) / speed;
			else if (curYLocation[i] + cardwidth <= destinationY[i])
				moveYperTurn[i] *= 2;
			else if (curYLocation[i] + cardwidth > destinationY[i])  //don't need to check if it's less than or equal to because it's default at 10
				moveYperTurn[i] = destinationY[i] - curYLocation[i];
		}

		Deck[cardSprite].display(curXLocation[i] + moveXperTurn[i], curYLocation[i] + moveYperTurn[i]);
		Deck[cardSprite].turnFaceUp();

#if (OS != IOS)
		Deck[cardSprite].rotate((float)(agk::Random(0, 45)+agk::Random(0, 45)));
#else
		Deck[cardSprinte].rotate((float) ((arc4random() % 46) + (arc4random() % 46)));
#endif
		if ((fabsf(moveXperTurn[i]) <= 0.001f) && (fabsf(moveYperTurn[i]) <= 0.001f))
			atLocation[i] = true;
	}

	for (int i = totalCards; i < 8; i++)
		atLocation[i] = true;

	if (atLocation[0] && atLocation[1] && atLocation[2] && atLocation[3] && atLocation[4] && atLocation[5] && atLocation[6] && atLocation[7])
		return true;
	else
		return false;

	/*bool rotateRight = false;
	bool rotateLeft = false;
	float destinationX[5] = {50.0f, 50.0f, 50.0f, 50.0f, 50.0f};
	float destinationY[5] = {50.0f, 50.0f, 50.0f, 50.0f, 50.0f};
	float offset[5] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
	float moveX;
	float moveY;
	float curXLocation = getX();
	float curYLocation = getY();
	float speed = 15.0f;
	float cardWidth = 1.76f;
	int turn = Selected.getID();
	int totalCards = Selected.getCardsInHand();

	//offset(62.5f, 87.5f);

	if (turn != SOUTH)
		turnFaceUp();

	switch (totalCards)
	{
	case 2:
		offset[0] = -CARDWIDTHS;
		offset[1] = CARDWIDTHS;
		break;
	case 3:
		offset[0] = -2 * CARDWIDTHS;
		offset[2] = 2 * CARDWIDTHS;
		break;
	case 4:
		offset[0] = -3 * CARDWIDTHS;
		offset[1] = -1.5f * CARDWIDTHS;
		offset[2] = 1.5f * CARDWIDTHS;
		offset[3] = 3 * CARDWIDTHS;
		break;
	case 5:
		offset[0] = -4 * CARDWIDTHS;
		offset[1] = -2 * CARDWIDTHS;
		offset[3] = 2 * CARDWIDTHS;
		offset[4] = 4 * CARDWIDTHS;
		break;
	}

	switch (turn)
	{
	case WEST:
		for (int i = 0; i < totalCards; i++)
		{
			destinationX[i] -= 20.0f;
			destinationY[i] -= 10.0f + offset[i];
		}
		rotateRight = true;
		break;
	case NORTH:
		for (int i = 0; i < totalCards; i++)
		{
			destinationX[i] -= 7.8f + offset[i];
			destinationY[i] -= 20.0f;
		}
		break;
	case EAST:
		for (int i = 0; i < totalCards; i++)
		{
			destinationX[i] += 5.0f + offset[i];
			destinationY[i] -= 10.0f;
		}
		rotateLeft = true;
		break;
	case SOUTH:
		for (int i = 0; i < totalCards; i++)
		{
			destinationX[i] -= 7.8f + offset[i];
			//destinationY[i] = ;
		}
		break;
	default:
		break;
	}

	if (destinationX[0] < curXLocation)
	{//have to go negative to reach destination
		if ((curXLocation - destinationX[0]) / speed > cardWidth)
			moveX = (destinationX[0] - curXLocation) / speed; //normal movement rate, negative
		else if (curXLocation - cardWidth >= destinationX[0])
			moveX = -cardWidth; //slower movement rate
		else
			moveX = destinationX[0] - curXLocation; //even out to destination, will be negative
	}
	else
	{ //have to go positive to reach destination
		if ((destinationX[0] - curXLocation) / speed > cardWidth)
			moveX = (destinationX[0] - curXLocation) / speed; //nomal movement rate
		else if (curXLocation + cardWidth <= destinationX[0])
			moveX = cardWidth;
		else
			moveX = destinationX[0] - curXLocation;
	}

	if (destinationY[0] < curYLocation)
	{
		if ((curYLocation - destinationY[0]) / speed > cardWidth)
			moveY = (destinationY[0] - curYLocation) / speed;
		else if (curYLocation - cardWidth >= destinationY[0])
			moveY = -cardWidth;
		else
			moveY = destinationY[0] - curYLocation;
	}
	else
	{
		if ((destinationY[0] - curYLocation) / speed > cardWidth)
			moveY = (destinationY[0] - curYLocation) / speed;
		else if (curYLocation + cardWidth <= destinationY[0])
			moveY = cardWidth;
		else if (curYLocation + cardWidth > destinationY[0])  //don't need to check if it's less than or equal to because it's default at 10
			moveY = destinationY[0] - curYLocation;
	}

#if (OS != IOS)
	rotate((float)(agk::Random(0, 45)+agk::Random(0, 45)));
#else
    rotate((float) ((arc4random() % 46) + (arc4random() % 46)));
#endif

	if (curXLocation != 0.0f)
		display(curXLocation + moveX, curYLocation + moveY);
	else
	{
		display(destinationX[0], destinationY[0]);
		return true;
	}

	if ((fabsf(moveX) <= 0.000001f) && (fabsf(moveY) <= 0.00001f))
		return true;
	else
		return false;*/
}

bool Card::updateCardLocs(short int turn, Card *vDec, short int card1, short int card2, short int card3, short int card4, short int round, bool flag)
{//sending 4 at a time to hand
	float destinationX[4] = {-1.0f, -1.0f, -1.0f, -1.0f};
	float destinationY[4] = {-1.0f, -1.0f, -1.0f, -1.0f};
	short int cardSprites[4] = {card1, card2, card3, card4};
	float moveXperTurn[4] = {10.0f, 10.0f, 10.0f, 10.0f};
	float moveYperTurn[4] = {10.0f, 10.0f, 10.0f, 10.0f};
	float curXLocation[4] = {10.0f, 10.0f, 10.0f, 10.0f};
	float curYLocation[4];
	float offsetN = (round * 4.0f * CARDWIDTHN) + CARDWIDTHN;
	float offsetH = (round * 4.0f * CARDWIDTHH) + CARDWIDTHH;
	float offsetS = (round * 4.0f * CARDWIDTHS) + CARDWIDTHS;
	bool atLocation[4] = {false, false, false, false};

	if (turn % 2 == 0)
		offsetH = (round * 6.0f * CARDWIDTHH) - (1.5f * CARDWIDTHH);

	switch (turn)
	{
	case WEST:
		destinationX[0] = destinationX[1] = destinationX[2] = destinationX[3] = WEST_LOC_X;
		destinationY[0] = WEST_LOC_Y + offsetH + CARDWIDTHH;
		destinationY[1] = destinationY[0] + (CARDWIDTHH * 1.5f);
		destinationY[2] = destinationY[1] + (CARDWIDTHH * 1.5f);
		destinationY[3] = destinationY[2] + (CARDWIDTHH * 1.5f);
		for(short int i = 0; i < 4; i++)
		{
			vDec[cardSprites[i]].rotateRight();
			vDec[cardSprites[i]].turnFaceDown();
		}
		break;
	case NORTH:
        if (flag)
            offsetN -= 0.5f;
		destinationX[0] = NORTH_LOC_X - offsetN;
		destinationX[1] = destinationX[0] - CARDWIDTHN;
		destinationX[2] = destinationX[1] - CARDWIDTHN;
		destinationX[3] = destinationX[2] - CARDWIDTHN;
		destinationY[0] = destinationY[1] = destinationY[2] = destinationY[3] = NORTH_LOC_Y;
		//y[1] = CENTER_Y -200; //debug
		for (short int i = 0; i < 4; i++)
			vDec[cardSprites[i]].turnFaceDown();
		break;
	case EAST:
		destinationX[0] = destinationX[1] = destinationX[2] = destinationX[3] = EAST_LOC_X;
		destinationY[0] = EAST_LOC_Y - offsetH - CARDWIDTHH;
		destinationY[1] = destinationY[0] - (CARDWIDTHH * 1.5f);
		destinationY[2] = destinationY[1] - (CARDWIDTHH * 1.5f);
		destinationY[3] = destinationY[2] - (CARDWIDTHH * 1.5f);
		for(short int i = 0; i < 4; i++)
		{
			vDec[cardSprites[i]].rotateLeft();
			vDec[cardSprites[i]].turnFaceDown();
		}
		break;
	case SOUTH:
		destinationX[0] = SOUTH_LOC_X - CARDWIDTHS + offsetS;
		destinationX[1] = destinationX[0] + CARDWIDTHS;
		destinationX[2] = destinationX[1] + CARDWIDTHS;
		destinationX[3] = destinationX[2] + CARDWIDTHS;
		destinationY[0] = destinationY[1] = destinationY[2] = destinationY[3] = SOUTH_LOC_Y;
		for(short int i = 0; i < 4; i++)
			vDec[cardSprites[i]].turnFaceUp();
		//y[1] = CENTER_Y +200;
		break;
	default:
		break;
	}

	for (short int i = 0; i < 4; i++)
	{
		curXLocation[i] = vDec[cardSprites[i]].getX();
		curYLocation[i] = vDec[cardSprites[i]].getY();

		if (destinationX[i] < curXLocation[i])
		{//have to go negative to reach destination
			if ((curXLocation[i] - destinationX[i]) / 8.0 > 20.0)
				moveXperTurn[i] = (destinationX[i] - curXLocation[i]) / 8; //normal movement rate, negative
			else if (curXLocation[i] - 20.0 >= destinationX[i])
				moveXperTurn[i] *= -2; //slower movement rate
			else
				moveXperTurn[i] = destinationX[i] - curXLocation[i]; //even out to destination, will be negative
		}
		else
		{ //have to go positive to reach destination
			if ((destinationX[i] - curXLocation[i]) / 8 > 20)
				moveXperTurn[i] = (destinationX[i] - curXLocation[i]) / 8; //nomal movement rate
			else if (curXLocation[i] + 20 <= destinationX[i])
				moveXperTurn[i] *= 2;
			else
				moveXperTurn[i] = destinationX[i] - curXLocation[i];
		}

		if (destinationY[i] < curYLocation[i])
		{
			if ((curYLocation[i] - destinationY[i]) / 8 > 20)
				moveYperTurn[i] = (destinationY[i] - curYLocation[i]) / 8;
			else if (curYLocation[i] - 20 >= destinationY[i])
				moveYperTurn[i] *= -2;
			else
				moveYperTurn[i] = destinationY[i] - curYLocation[i];
		}
		else
		{
			if ((destinationY[i] - curYLocation[i]) / 8 > 20)
				moveYperTurn[i] = (destinationY[i] - curYLocation[i]) / 8;
			else if (curYLocation[i] + 20 <= destinationY[i])
				moveYperTurn[i] *= 2;
			else if (curYLocation[i] + 20 > destinationY[i])  //don't need to check if it's less than or equal to because it's default at 10
				moveYperTurn[i] = destinationY[i] - curYLocation[i];
		}

		vDec[cardSprites[i]].display(curXLocation[i] + moveXperTurn[i], curYLocation[i] + moveYperTurn[i]);
		//if (turn == EAST)
			//vDec[cardSprites[i]].setDepth((round * 4) - i + 4);
		//else
		vDec[cardSprites[i]].setDepth((round * 4) - i - 2);
		if ((abs(moveXperTurn[i]) <= 0.01f) && (abs(moveYperTurn[i]) <= 0.01f))
			atLocation[i] = true;
		vDec[cardSprites[i]].show();
	}

	if (atLocation[0] && atLocation[1] && atLocation[2] & atLocation[3])
		return true;
	else
		return false;
}

short int Card::updateCardPos(Card *vdeck, Player South, short int topSuit, short int topRank, short int leadSuit, short int trump, short int firstPlay)
{
	float mouseX = agk::GetPointerX();
	//float mouseY = agk::GetPointerY();
	short int cardSel;
	short int maxCards = South.getCardsInHand();
	short int cardValue;
	Deck test;
	bool raiseCard = true;
	bool trumped;

	if (leadSuit != topSuit)
		trumped = true;
	else
		trumped = false;

	//if (mouseX - SOUTH_LOC_X < 0)
		//cardSel = 0;
	cardSel = (short int) ((mouseX - (SOUTH_LOC_X + ((10 - (maxCards / 2)) * CARDWIDTHS))) / CARDWIDTHS);

	if (cardSel > maxCards - 1)
        cardSel = maxCards - 1;
	else if (cardSel < 0)
		cardSel = 0;

	cardValue = South.getCard(cardSel);
	//make sure selection is valid
	if (firstPlay % 4 != 0)
	{//not first to play
		//check if player has to beat top play
		//check that player has suit
		if (South.getNumOfCardsInRange(-1, leadSuit))
		{//player has suit
			if (test.getSuit(cardValue) != leadSuit)
				raiseCard = false;
			else
			{//has suit, check to see if player has a higher ranked card
				if ((topRank != ACE) && !trumped)
				{//has suit, !trumped, and lead was not an ACE
					short int count = 0;

					for (short int i = 1; i < topRank + 1; i++)
						count += South.getNumOfCardsInRange(topRank - i, leadSuit);
					if (count)
					{//has suit, !trumped, lead was not an ACE, and Player has cards that can beat topRank
						if (test.getRank(cardValue) >= topRank)
							raiseCard = false;
					}//no else as if player cannot beat top rank, all cards in suit are valid7
				}//has suit, !trumped, lead WAS an ACE, anything in suit is okay
				//has suit, trumped, anything in suit is okay
			}
		}
		else
		{//player does not have suit, must trump
			if (South.getNumOfCardsInRange(-1, trump))
			{//player has trump
				if (test.getSuit(cardValue) != trump)
					raiseCard = false;
				if (trumped)
				{//already trumped, must beat it if possible
					if (topRank != ACE)
					{//can you beat it?
						short int count = 0;

						for (short int i = 1; i < topRank + 1; i++)
							count += South.getNumOfCardsInRange(topRank - i, trump);
						if (count)
						{//can beat topRank
							if (test.getRank(cardValue) >= topRank)
								raiseCard = false;
						}//can't beat, in trump section
					}//topRank IS an Ace, cannot beat, already in trump section
				}//not already trumped, already in trump section, can play anything
			}//if player doesn't have trump, can play anything
		}
	}//first to play, can play anything

	if (raiseCard)
		vdeck[cardValue].raiseHeight();

	for (short int i = 0; i < maxCards; i++)
	{
		if (vdeck[South.getCard(i)].getY() != SOUTH_LOC_Y)
			if (i !=  cardSel)
				vdeck[South.getCard(i)].resetHeight();
	}

	if (raiseCard)
		return South.getCard(cardSel);
	else
		return -1;
}

short int Card::getRank()
{
	return rank;
}

short int Card::getSuit()
{
	return suit;
}

void Card::setSize(float value)
{
	Front.setSize(value);
	Back.setSize(value);
	Outlined.setSize(value);
}

void Card::offset(float x, float y)
{
	Front.setOffset(x, y);
	Back.setOffset(x, y);
	Outlined.setOffset(x, y);
}

void Card::setDepth(unsigned short int value)
{
	Front.setDepth(value);
	Back.setDepth(value);
	Outlined.setDepth(value);
}

void Card::display(float x, float y)
{
	Front.setPosition(x, y);
	Back.setPosition(x, y);
	Outlined.setPosition(x, y);

	show();
}

void Card::show(void)
{
	Front.setVisible(!isFaceDown && !isTrump);
	Outlined.setVisible(!isFaceDown && isTrump);
	Back.setVisible(isFaceDown);
}

void Card::hide(void)
{
	Back.setVisible(false);
	Outlined.setVisible(false);
	Front.setVisible(false);
}
