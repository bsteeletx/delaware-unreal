#include "HandScore.h"
#include "agk.h"

HandScore::HandScore(void)
{

}


HandScore::~HandScore(void)
{
}

void HandScore::setup(AnimatedSprite *Symbol)
{
    int imgNum;
    float hOffset;
    float vOffset;
	///step 1: setup or setData
	Parent.setImage("default/Scoreboards.png");

    imgNum = Parent.getImageNumber("board table.png");
	Background.setImage(imgNum);
	setNumberData();

    imgNum = Parent.getImageNumber("check_box_off.png");
	HCheck.setImage(imgNum);
    VCheck.setImage(imgNum);

    imgNum = Parent.getImageNumber("check_box_on.png");
	HCheckOn.setImage(imgNum);
    VCheckOn.setImage(imgNum);

	Symbol->incrementFrame();
	agk::Sync();

	///step 2: setSize
	HCheck.setSize(5.0f);
	HCheckOn.setSize(5.0f);
	VCheck.setSize(5.0f);
	VCheckOn.setSize(5.0f);
	Background.setSize(80.0f);

	Symbol->incrementFrame();
	agk::Sync();

	///step 3: setPriority
	setDepth(50);

	Symbol->incrementFrame();
	agk::Sync();

	///step 4: set x, y with display
#if (PLATFORM == MOBILE)
    hOffset = 4.5f;
    vOffset = 3.5f;
#else
    hOffset = 0.0f;
    vOffset = 0.0f;
#endif

	Background.setPosition(10.0f, 25.0f);
	HCheck.setPosition(66.0f, 60.0f + hOffset);
	HCheckOn.setPosition(66.0f, 60.0f + hOffset);
	VCheck.setPosition(66.0f, 51.0f + vOffset);
	VCheckOn.setPosition(66.0f, 51.0f + vOffset);

	Symbol->incrementFrame();
	agk::Sync();

	///step 5: hide
	HCheck.setVisible(false);
	HCheckOn.setVisible(false);
	VCheck.setVisible(false);
	VCheckOn.setVisible(false);

	///step 6 (optional): setimprint

	/////TEXT


	/*MinusSign[1] = Text("-");
	MinusSign[1].setColor(RGBA(0, 0, 0);
	MinusSign[1].setSize(5.5f);
	MinusSign[1].setDepth(55);
	MinusSign[1].hide();

	MinusSign[0].setPosition(Point((76.5f, 51.5f);
	MinusSign[1].setPosition(Point((76.5f, 60.5f);*/
}

void HandScore::setNumberData(void)
{
    float hOffset = 0.0f;
    float vOffset = 0.0f;
    float mOffset = 0.0f;

	for (short int i = 0; i < 2; i++)
	{
		for (short int j = 0; j < 4; j++)
			HandScoreData[i][j].setData("default/");
	}

#if (PLATFORM == MOBILE)
    hOffset = 4.5f;
    vOffset = 3.5f;
    mOffset = -1.0f;
#endif

	for (short int i = 0; i < 3; i++)
	{
		for (short int k = 0; k < 5; k++)
		{
			if (k == 1)
				continue;
			if (k > 1)
			{
				HandScoreData[0][k-1].setLocation(i, 83.0f - ((2.5f * i) + (k * 12.6f)), 52.0f + vOffset, -4.5f, -0.5f + mOffset);
				HandScoreData[1][k-1].setLocation(i, 83.0f - ((2.5f * i) + (k * 12.6f)), 61.0f + hOffset, -4.5f, -0.5f + mOffset);
			}
			else
			{
				HandScoreData[0][k].setLocation(i, 83.0f - ((2.5f * i) + (k * 12.6f)), 52.0f + vOffset, -4.5f, -0.5f + mOffset);
				HandScoreData[1][k].setLocation(i, 83.0f - ((2.5f * i) + (k * 12.6f)), 61.0f + hOffset, -4.5f, -0.5f + mOffset);
			}
		}
	}

	for (short int k = 0; k < 4; k++)
	{
		HandScoreData[0][k].setDepth(52);
		HandScoreData[1][k].setDepth(52);
		HandScoreData[0][k].setSize(2.5f);
		HandScoreData[1][k].setSize(2.5f);
		HandScoreData[0][k].setValue(0);
		HandScoreData[1][k].setValue(0);
		HandScoreData[0][k].hideAll();
		HandScoreData[1][k].hideAll();
	}
}

void HandScore::hide(void)
{
	Background.setVisible(false);
	for (short int i = 0; i < 2; i++)
	{
		for (short int j = 0; j < 4; j++)
			HandScoreData[i][j].hideAll();
	}

	HCheck.setVisible(false);
	HCheckOn.setVisible(false);
	VCheck.setVisible(false);
	VCheckOn.setVisible(false);
}

void HandScore::setDepth(short unsigned int value)
{
	Background.setDepth(value);

	HCheck.setDepth(value - 1);
	HCheckOn.setDepth(value - 1);
	VCheck.setDepth(value - 1);
	VCheckOn.setDepth(value - 1);

	for (short int i = 0; i < 2; i++)
	{
		for (short int j = 0; j < 4; j++)
			HandScoreData[i][j].setDepth(value - 1);
	}
}

void HandScore::reset(void)
{
	for (short int i = 0; i < 2; i++)
	{
		for (short int j = 0; j < 4; j++)
			HandScoreData[i][j].setValue(0);
	}
}
