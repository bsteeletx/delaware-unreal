#pragma once

#include "PinochleGame.h"
#include "BidGame.h"
#include "TrickGame.h"
#include "Player.h"
#include "Defines.h"
#include "ParentImage.h"

class Card
{
public:
	Card(short unsigned int frontImageNumber, short unsigned int backImageNumber);
	Card();
	~Card(void);

    bool compareBackImageTo(unsigned int image);

	void display(float x, float y);

    unsigned int getBackImage(void);
	bool getFaceDirection(void);
	unsigned int getFrontImage(void);
	bool getIsRotated();
	bool getOutline(void);
	unsigned int getOutlineImage(void);
	short int getRank(void);
	void getRank(char buffer[], short int rankNum);
	float getRotationAmount(void);
	short int getStyle(void);
	short int getSuit(void);
	void getSuit(char buffer[], short int suitNum);
	float getX(void);
	float getY(void);

	void hide(void);

	bool isRotatedLeft(void);
	bool isRotatedRight(void);
	
	void offset(float x, float y);
	void outlineCard(void);
	
	void raiseHeight(void);
	void resetHeight(void);
	void revealCard(Player);
	void rotate(float amount);
	void rotateLeft();
	void rotateRight();
	
    void setBackImage(unsigned int backImage);
	void setBackImage(unsigned int backImage, short place);
    void setData(char filename[], short int rankNum, short int suitNum);
	void setDepth(unsigned short int value);
	void setFrontImage(unsigned int frontImage);
    void setOutlineImage(unsigned int outlineImage);
	void setSize(float value);
	void setStyle(int newStyle);
	void show(void);
	void sortVisualCards(Player, Card *, short theme);
	void sortVisualCards(Player West, Player North, Player East, Player South, Card *vDeck, short int round, short theme);
	
	void turnFaceDown();
	void turnFaceUp();
	
	void unOutlineCard(void);
	void unRotate();
	bool updateCardLoc(short int turn, PinochleGame PGame, BidGame BGame, TrickGame TGame, short int count, bool flag);
	bool updateCardLocs(short int turn, Card *vDec, short int card1, short int card2, short int card3, short int card4, short int round, bool flag=false);
	short int updateCardPos(Card *vdeck, Player South, short int topSuit, short int topRank, short int leadSuit, short int trump, short int firstPlay);
	bool updateCardThrowIn(Player Selected, Card *Deck);
	bool updateShrinkCloseTrick(short int location, Card *Card2, Card *Card3, Card *Card4);

private:
	Sprite Front;
	Sprite Back;
	Sprite Outlined;
    static ParentImage Parent;

	bool isRotated;
	bool rotatedLeft;
	bool rotatedRight;
	bool isTrump;
	bool isFaceDown;
	bool multipleBack;
	float rotateAmount;
	short int suit;
	short int rank;
    static unsigned int cardBackImageNumber[3];
};
