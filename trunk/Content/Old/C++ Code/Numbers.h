#pragma once
#include "Sprite.h"
#include "Text.h"
#include "Defines.h"
#include "ParentImage.h"

class Numbers
{
public:
	//Sound(char filename[]);
	Numbers();
	~Numbers(void);

	//void changeImage(short int theme);
    void decrementValue(void);
	void display(short int digit, short int number);
	void display(short int digit, short int number, float x, float y);
	short int getDigit(short int digit);
	bool getPositive(void);
	//int getSeparatedValue(void);
	short int getValue(void);
	bool getVisible(void);
	float getX(short int digit);
	float getY(short int digit);
	void hide(short int digit, short int number);
	void hideAll(void);
	void incrementTo(short int targetValue, bool alwaysDisplay = false);
    void incrementValue(void);
    void initDefaultNumbers(void);
    void initThematicNumbers(const char dir[]);
    void makeNegative(void);
    void makePositive(void);
	bool isVisible(void);
	//bool scroll(char value, char digit);
	void setColor(unsigned short int red, unsigned short int green, unsigned short int blue, unsigned short int alpha);
	void setData(const char dir[], bool useParentheses=false);
	//void setDigit(short int digit, float x, float y);
	void setDigit(short int digit, short int number);
	//void setDigitData(short int digit, char desc[]);
	void setLocation(short int digit, float x, float y, float negOffsetX=0, float negOffsetY=0);
	void setDepth(short unsigned int prioValue);
	void setSize(float x, float y = -1.0f);
	void setValue(short int newValue);
	void show(short int digit, short int number);
	void showAll(bool always=false);
	void useParentheses(void);

    static ParentImage Parent;
    static ParentImage ScoreParent;
    static unsigned short defaultNumbers[10];
    static unsigned short thematicNumbers[10];

private:

	void updateValue(int newValue);

	Sprite digitOne[10]; //ones
	Sprite digitTwo[10]; //tens
	Sprite digitThree[10]; //hundreds
	Sprite Parentheses[2];

	Text MinusSign;

	short int visDigit1;
	short int visDigit2;
	short int visDigit3;
	short int value;
	unsigned short int colorValue[4];
	bool isPositive;
	bool useMinus;
};
