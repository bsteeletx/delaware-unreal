// Includes, namespace and prototypes
#include "template.h"
#include <ctime>

using namespace AGK;
app App;

#if (DEMOBUILD)
const float demoTime = 3600.0f;
float timeUsedInSession;
float timeUsedPrev;
#endif

bool debug = false; 
unsigned short themeQueue[128];
unsigned short themeQueuePointer;
unsigned short menuQueue[128];
unsigned short menuQueuePointer;

//for other things
//unsigned short int Sound::v
Sound Buttons::ButtonClick;
ParentImage Numbers::Parent;
ParentImage Card::Parent;
ParentImage Numbers::ScoreParent;
unsigned int Card::cardBackImageNumber[3];
unsigned short Numbers::thematicNumbers[10];
unsigned short Numbers::defaultNumbers[10];

// the main entry point for the application is this function
void app::Begin ( void )
{
//    float tempWidth = (float) agk::GetDeviceWidth();
//   float tempHeight = (float) agk::GetDeviceHeight();
//    int currentOrientation;
    
//    findValueForDebugging(tempWidth);
//    findValueForDebugging(tempHeight);
    
//	if (tempWidth > tempHeight)
//        aspect = tempWidth / tempHeight;
//    else
//        aspect = tempHeight / tempWidth;
    
	agk::SetDisplayAspect(ASPECT);

#if PLATFORM != PC
    agk::SetOrientationAllowed(0, 0, 1, 1);
	agk::SetTransitionMode(1);
#endif
	agk::SetBorderColor(0, 0, 0);
	agk::SetSyncRate(30, 0);
    
	agk::SetTextDefaultFontImage(agk::LoadImage("default/Arvo.png"));
    agk::SetResolutionMode(1);
    
    /*if (PLATFORM == MOBILE)
        agk::SetResolutionMode(0);
    else
        agk::SetResolutionMode(1);*/
    
    
	//Setting Error Mode, 0 means no reports, 1, mean console reports, 2 means stop everything
	agk::SetErrorMode(1);

#if (OS == WINDOWS)
	agk::SetWindowTitle("Double Pinochle");
#endif
	//strcpy(themeDir, "green/");

	//initialize variables
	resetGame(true);
    
    loadSteeleLogo();
    initSteeleLogo();
    
	NewGame.toggleXState(MENU);
	NewGame.toggleXState(LOADING);

#if DEMOBUILD
	timeUsedInSession = 0;
	debug = false;
#endif
}

#if (DEMOBUILD)
void app::Loop ( bool *exit )
#else
void app::Loop(void)
#endif
{	
	if (gameState != NewGame.getState())
		update = 60;
	else if (update > 10)
		update -= 0.05f;

	gameState = NewGame.getState();
	trump = NewTrickGame.getTrumpSuit();
#if(OS==DROID)
	if((agk::GetRawKeyPressed(27)||agk::GetRawKeyPressed(87))&&menuPressed)
	{
		menuChoice=-1;
		ingamebutton=true;
		inGameMenuButton();
		menuPressed=false;
	}
#endif


	debug = false;

	if (debug)
		testScale();

#if DEMOBUILD      
	if (!debug && !demoOver)
#else
	if (!debug)
#endif
	{
		//put in controls
		switch (gameState)
		{
		case LOADING:
			handleLoading();
			return;
			break;
		case MENU:
			handleMenu();
			break;
		case OPTIONS:
			handleOptions(!CurrentTheme.isSetup());
			break;
		case START:
			handleStart();
			break;
		case INGAMEMENU:
			handleInGameMenu();
			break;
		case INGAMEOPTIONS:
			handleInGameOptions();
			break;
		case DEALING:
			handleDealing();
			break;
		case ANIMATION:
			handleAnimation();
			break;
		case SORTING:
			handleSorting();
			break;
		case BIDDING:
			handleBidding();
			break;
		case MELDING:
			handleMelding();
			break;
		case PLAYING:
			handlePlaying();
			break;
		case SCOREBOARD:
			handleScoreboard();
			break;
		default:
			if (gameState < 0)
				NewGame.toggleXState(LOADING);
			return;
			break;
		}
	}

    gameState = NewGame.getState();
	trump = NewTrickGame.getTrumpSuit();

	if (gameState == BIDDING)
	{
		NBack.setVisible(true);
		EBack.setVisible(true);
		WBack.setVisible(true);
		SBack.setVisible(true);

		if (NewBidGame.playerBid(NORTH) || NewBidGame.getPass(NORTH))
			CurrentTheme.PastNorthBid.showAll(true);
		else
			CurrentTheme.PastNorthBid.hideAll();

		if (NewBidGame.playerBid(EAST) || NewBidGame.getPass(EAST))
			CurrentTheme.PastEastBid.showAll(true);
		else
			CurrentTheme.PastEastBid.hideAll();

		if (NewBidGame.playerBid(WEST) || NewBidGame.getPass(WEST))
			CurrentTheme.PastWestBid.showAll(true);
		else
			CurrentTheme.PastWestBid.hideAll();

		if (NewBidGame.playerBid(SOUTH) || NewBidGame.getPass(SOUTH))
			CurrentTheme.PastSouthBid.showAll(true);
		else
			CurrentTheme.PastSouthBid.hideAll();
	}

	if ((gameState == ANIMATION) && !passInitialAnim)
	{
		agk::Sync();
		return;
	}
	/*if (agk::KeyDown(AGK_KEY_CONTROL))

		agk::Key
		dbControlKey())
		toggleDebug();
		*/
	gameTimer = agk::Timer();

#if (DEMOBUILD)
	if (gameState >= START)
	{
		timeUsedInSession = gameTimer;

		if (timeUsedInSession + timeUsedPrev > demoTime)
		{
			if (!DemoEndBack.hasGrown())
				DemoEndBack.updateGrowOpen(DemoEndBack.getScaleSize(), DemoEndBack.getScaleSizeY());
			else if (!mouseClick)
				DemoEndText.setVisible(true)
			else
				*exit = true;

			demoOver = true;
		}

	}
#endif

	if (delayStep)
	{
		//agk::Print(delayStep);
		if (gameTimer - mouseDelay > 0.125f)
		{
			delayStep++;
			mouseDelay = gameTimer;
		}

		mouseClick = false;
	}

	if (agk::GetPointerReleased())
	{
		if (!incrementing)
		{
			mouseClick = true;
			mouseX = agk::GetPointerX();
			mouseY = agk::GetPointerY();
			delayStep = 1;
			mouseDelay = gameTimer;
			swipe = false;
			hold = false;
		}
	}
    

    
#if (PLATFORM != PC)
    if (!hold)
    {
        if(agk::GetPointerState())
        {
            hold = true;
            xstartpos = agk::GetPointerX();
            ystartpos = agk::GetPointerY();
        }
    }
#endif

#if (PLATFORM == PC)
	if (agk::GetButtonPressed(5))
		showAllCards();
#endif
    
	if (delayStep == 4)
	{
		delayStep = 0;
		mouseClick = false;
	}

	/*else if (delayStep)
	{
		mouseDelay = agk::Timer() - mouseDelay;

		if (mouseDelay > 1.0f)
			delayStep++;
	}*/


	//draw on screen
	//updateScore(NewGame, NewTrickGame, NewBidGame, count, passInitialAnim);
	if (gameState > START)
	{
		if (CurrentTheme.Background.getWidth() == 100.0f)
		{
			bid = NewBidGame.getBid();
			updateBottomLeft();
			updateTopLeft();
			updateTopRight();
		}
	}
	else
	{
		if (TrumpSymbol.getExists())
		{
			if (TrumpSymbol.getVisible())
                TrumpSymbol.setVisible(false);

			for (int i = 0; i < 3; i++)
			{
				CurrentTheme.HScore.hide(i, CurrentTheme.HScore.getDigit(i));
				CurrentTheme.VScore.hide(i, CurrentTheme.VScore.getDigit(i));
			}
		}
	}

	if (debug)
	{
		displayAllKnown();

		if (!cardsRevealed)
		{
			for (short int i = 0; i < 20; i++)
			{
				CurrentTheme.Deck[i].revealCard(North);
				CurrentTheme.Deck[i].revealCard(West);
				CurrentTheme.Deck[i].revealCard(East);
			}
		}
	}

	if ((gameState >= BIDDING) && (gameState != SCOREBOARD))
	{
		ingamebutton=true;
		inGameMenuButton();
	}

	//agk::Print(agk::GetPointerX());
	//agk::Print(agk::GetPointerY());
	/*agk::Print(West.getPMeld());
	agk::Print(North.getPMeld());
	agk::Print(East.getPMeld());
	agk::Print(South.getPMeld());*/

	if (gameState <= INGAMEOPTIONS)
	{
		LoadingScreen.setVisible(true);
		LoadingScreen.setDepth(0);
	}

	else
		LoadingScreen.setVisible(false);


	if (delayStep || mouseClick || (gameState == ANIMATION) || (gameState == DEALING) || incrementing)
		update = 60;
	
	agk::SetSyncRate(update, 0);

	//agk::Print("Sync Rate: ");
	//agk::Print(update);

	agk::Sync();

	if (gameState == SORTING)
		agk::Sleep(500);
	else if (gameState == PLAYING)
	{
		//if (foundLocation)
			//dbWaitKey();
		if ((count % 4 == 0) && foundLocation && (mouseDelay == 2))
		{
			for (short int i = JACK; i >= ACE; i--)
			{
				for (short int j = HEARTS; j <= SPADES; j++)
				{
					if (NewTrickGame.getNumOfCardsLeft(i, j) == 0)
					{
						West.known[i][j] = 0;
						North.known[i][j] = 0;
						East.known[i][j] = 0;
						South.known[i][j] = 0;
					}
				}
			}
		}
		//else if (foundLocation)
			//dbWait(500); //should be at 1000
	}
	else if (gameState == BIDDING)
	{
		if (pauseGame && !incrementing)
		{
			agk::Sleep(1000);
			pauseGame = false;
		}
	}
}

// Called when the app ends
void app::End ( void )
{
	unsigned short int fileID = agk::OpenToWrite("SSDPS.DAT", 0);
	agk::WriteString(fileID, "_Theme_");
	agk::WriteInteger(fileID, tableTopStyle);
	agk::WriteString(fileID, "_Sound_");
	if (soundOn)
		agk::WriteInteger(fileID, 1);
	else
		agk::WriteInteger(fileID, 0);
#if(DEMOBUILD)
	{
		agk::WriteString(fileID, "_DemoTime_");
		agk::WriteFloat(fileID, timeUsedInSession + timeUsedPrev);
		agk::WriteString(fileID, "_DemoSeed_");
		agk::WriteInteger(fileID, ShuffledDeck.getDeckCounter());
	}
#endif   
	//doEndGameSwitch(fileID, NewGame.getState());

	agk::CloseFile(fileID);

	return;
}

void app::resetGame(bool startup)
{
	West.reset();
	North.reset();
	East.reset();
	South.reset();
	NewBidGame.reset();
	ShuffledDeck.reset(DEMOBUILD);
	NewTrickGame.reset();
	NewGame.resetGame();
	hand = 0;
	//dealer = agk::Random(0, 3);
#if (OS == IOS)
    dealer = arc4random() % 4;
#else
	dealer = agk::Random(0, 3);
#endif
    if (!startup)
	{
		hideAllText();
        CurrentTheme.HScore.setValue(0);
        CurrentTheme.VScore.setValue(0);
		GameResults.reset();
		GameResults.hide();
		HandResults.reset();
		HandResults.hide();
		CurrentTheme.hide();

		for (float i = CurrentTheme.Background.getWidth(); i > 0.0f; CurrentTheme.Background.setSize(--i))
			agk::Sync();
    }
    else
    {
        //unsigned int temp = (unsigned) time(NULL);
        //std::srand(temp);
        //agk::SetRandomSeed(temp);   
    }
}

void app::setSoundData(void)
{
	Cards1 = Sound("sounds/cardshuffle1.wav");
	Cards2 = Sound("sounds/cardshuffle2withbridge.wav");
	Cards3 = Sound("sounds/cardshuffle3.wav");
	Cards4 = Sound("sounds/cardslide2.wav");
	Cards5 = Sound("sounds/cardslide1.wav");
	Cards6 = Sound("sounds/cardslide3.wav");
	Cards7 = Sound("sounds/cardslide4.wav");
	Cards8 = Sound("sounds/cardslide5.wav");
	Cards9 = Sound("sounds/cardbridge1.wav");
	Cards10 = Sound("sounds/gettingcardsstraightend.wav");

	Loading.incrementFrame();
	agk::Sync();

	Trump[4] = Sound("sounds/glassbreak1.wav");
	Trump[3] = Sound("sounds/glassbreak2.wav");
	Trump[2] = Sound("sounds/glassbreak3.wav");
	Trump[1] = Sound("sounds/carcrash1.wav");
	Trump[0] = Sound("sounds/dynamite1.wav");

	Loading.incrementFrame();
	agk::Sync();

	//Ambient.setFileName("sounds/CasinoBG.wav");

	PointClick = Sound("sounds/mouseclick1.wav");

	Loading.incrementFrame();
	agk::Sync();

}

void app::loadSteeleLogo(void)
{
	char temp[64];
	
	loadingImage[0] = myLoadImage(fileLoc("Loading.png", temp));

#if (PLATFORM != MOBILE)
    SteeleLogo = Sprite(Text("default/steele_logo_plate.png"));
#else
    SteeleLogo = Sprite(Text("default/steele_logo_480x320_plate.png"));
#endif
	//SteeleLogo.setData(loadingImage[0], "steele_logo_transparent.png");
}

void app::initSteeleLogo(void)
{
	SteeleLogo.setDepth(1);
	SteeleLogo.setSize(100.0f);
	SteeleLogo.setVisible(true);
	SteeleLogo.setPosition(0.0f,  0.0f);
    agk::Sync();

#if (OS != IOS)
	for(unsigned short int i = 0; i < 254; i+=2)
	{
		SteeleLogo.setAlpha(i);
		agk::Sync();
	}
    
	agk::Sleep(1000);
#endif
}

void app::fadeSteeleLogo(void)
{
	for(unsigned short int i = 254; i > 0; i-=2)
	{
		SteeleLogo.setAlpha(i);
		agk::Sync();
	}
    
    agk::SetOrientationAllowed(0, 0, 1, 1);
}

void app::loadLoadScreen(void)
{
#if (OS != IOS)
	SteeleLogo.setVisible(false);
#endif
	LoadingScreen = Sprite(loadingImage[0], Text("load_landscape.png"));
	LoadScreen1 = Sprite(loadingImage[0], Text("loading.jpg"));
#if (PLATFORM == MOBILE)
	Loading = AnimatedSprite(25, 25, 32, loadingImage[0], Text("Load Suit Rotation.png"));
#else
    Loading = AnimatedSprite(75, 75, 32, loadingImage[0], Text("Load Suit Rotation.png"));
#endif
	Loading.setVisible(false);
    
    SteeleLogo.~Sprite();
}


void app::handleMenu()
{
	buttonDelay = 1.0f;
#if (OS == IOS)
    #if (PLATFORM == TABLET) // New Game-Options Button 
    const float yMinValue = 81.0f;
    const float yMaxValue = 89.0f;
    #elif (PLATFORM == MOBILE)
    const float yMinValue = 80.0f;
    const float yMaxValue = 95.0f;
    #endif
#elif (OS == DROID)
	const float yMinValue = 80.0f;
    const float yMaxValue = 95.0f;
#elif (OS == WINDOWS)
	const float yMinValue = 81.0f;
    const float yMaxValue = 89.0f;
#endif
    
	LoadScreen1.setVisible(false);
    Legal.setVisible(false);
    Loading.setVisible(false);
	
	for (float i = CurrentTheme.Background.getWidth(); i > 0.0f; CurrentTheme.Background.setSize(--i))
		agk::Sync();

	MainMenu.NewGame.ButtonUp.setVisible(true);
	MainMenu.Options.ButtonUp.setVisible(true);

	//MainMenu.update();

	if (mouseClick)
	{
		if ((mouseX >= (15.0f )) && (mouseX <= (35.0f )))
		{
			if((mouseY >= (yMinValue)) && (mouseY <= (yMaxValue)))
			{
				menuChoice = 1;
			}
		}

		if ((mouseX >= (55.0f )) && (mouseX <= 84.8f ))
		{
			if ((mouseY >= (yMinValue)) && (mouseY <= (yMaxValue)))
				menuChoice = 4;
		}
	}

	if (delayStep)
	{
		if(MainMenu.ButtonAnimation(delayStep, menuChoice))
		{
			if (menuChoice == 1)
			{
                MainMenu.hide();
                Loading.setVisible(true);
                LoadScreen1.setVisible(true);
                clearQueue(&Loading, 1);
                CurrentTheme.setup(&Loading, themeDir, NewGame.getState(), aspect);
				NewGame.toggleXState(MENU);
				NewGame.toggleXState(START);
                Loading.setVisible(false);
                LoadScreen1.setVisible(false);
			}
			else if (menuChoice == 4)
			{
				NewGame.toggleXState(MENU);

				if (!NewGame.isStateActive(OPTIONS))
					NewGame.toggleXState(OPTIONS);
			}

			delayStep = 0;
			menuChoice = 0;
			MainMenu.hide();
		}
	}
}

void app::handleInGameMenu()
{
	for (float i = CurrentTheme.Background.getWidth(); i > 0.0f; CurrentTheme.Background.setSize(--i))
		agk::Sync();
	
	MainMenu.NewGame.ButtonUp.setVisible(true);
	MainMenu.Options.ButtonUp.setVisible(true);
	MainMenu.ResumeGame.ButtonUp.setVisible(true);
	MeldAmount.setVisible(false);
	TapScreen.setVisible(false);
	TextBackground.setVisible(false);
	//MainMenu.update();
	CurrentTheme.BidMeldBubbles.hide();

#if(OS==DROID)
	if(agk::GetRawKeyPressed(27))
	{
		menuChoice=3;
	}
#endif
	if (mouseClick)
	{
		if ((mouseX >= (16.17f )) && (mouseX <= (35.0f ))) //NewGame
		{
			if((mouseY >= (81.0f )) && (mouseY <= (88.8f )))
			{
				menuChoice = 1;
			}
		}

		if ((mouseX >= (66.2f )) && (mouseX <= (85.0f ))) //Options
		{
			if ((mouseY >= (81.0f )) && (mouseY <= (88.8f )))
				menuChoice = 4;
		}

		if ((mouseX >= (41.0f )) && (mouseX <= (59.9f ))) //ResumeGame
		{
			if ((mouseY >= (81.0f )) && (mouseY <= (88.8f )))
				menuChoice = 3;
			menuPressed=true;
		}

		if (menuChoice == 0)
			mouseClick = false;
	}

	if(MainMenu.ButtonAnimation(delayStep, menuChoice))
	{
		MainMenu.hide();

		if (menuChoice == 1)
		{
			NewGame.toggleXState(INGAMEMENU);
			resetGame();
			NewGame.toggleXState(START);
		}
		else if (menuChoice == 4)
		{
			NewGame.toggleXState(INGAMEMENU);
			NewGame.toggleXState(INGAMEOPTIONS);
		}
		else if (menuChoice == 3)
		{
			NewGame.toggleXState(INGAMEMENU);
			NewGame.toggleXState(GROWBACKGROUND);
		}
		menuChoice = 0;
		delayStep = 0;
    }
}

void app::setThemeOptionImage(char buffer[])
{
	strcpy(buffer, "background_");

	switch(tableTopStyle)
	{
	case 0:
		strcat(buffer, "g.jpg");
		break;
	case 1:
		strcat(buffer, "r.jpg");
		break;
	case 2:
		strcat(buffer, "a.jpg");
		break;
	case 3:
		strcat(buffer, "x.jpg");
		break;
	case 4:
		strcat(buffer, "e.jpg");
		break;
	case 5:
		strcat(buffer, "f.jpg");
		break;
	default:
		strcat(buffer, "g.jpg");
	}
}

void app::handleOptions(bool init)
{
    float width = agk::GetSpriteWidth(Options.Background.getSpriteNumber());
    float height = agk::GetSpriteHeight(Options.Background.getSpriteNumber());
    
#if (OS == IOS)
    #if (PLATFORM == TABLET) //    left-right arrows / sound button / new game-resume buttons
    const float yMinValues[3] = {35.0f, 70.0f, 81.0f};
    const float yMaxValues[3] = {51.5f, 80.0f, 94.0f};
    #elif (PLATFORM == MOBILE)
        const float yMinValues[3] = {40.0f, 75.0f, 85.0f};
        const float yMaxValues[3] = {55.0f, 85.0f, 100.0f};
    #endif
#elif (OS == DROID)
	const float yMinValues[3] = {40.0f, 75.0f, 85.0f};
    const float yMaxValues[3] = {55.0f, 85.0f, 100.0f};
#elif (OS == WINDOWS)
	const float yMinValues[3] = {35.0f, 70.0f, 81.0f};
    const float yMaxValues[3] = {51.5f, 80.0f, 94.0f};
#endif
	
	for (float i = Options.Background.getWidth(); i < 100.0f; Options.Background.setSize(++i))
		agk::Sync();

	Options.show(tableTopStyle);

	if (NewGame.getPrevState() <= MENU)
		Options.ResumeGame.hide();

	//Options.update();

	if (mouseClick)
	{
        if ((xstartpos >= 40.0f) && (xstartpos <= 60.0f))
        {
            if (mouseX <= xstartpos - 5.0f)
            {
                if (mouseX <= 40.0f)
                {
                    menuChoice = 5;
                    swipe = true;
                }
            }
            else if (mouseX >= xstartpos + 5.0f)
            {
                if (mouseX >= 60.0f)
                {
                    menuChoice = 6;
                    swipe = true;
                }
            }
        }
        
        if (mouseClick && !swipe)
        {
    
            if (mouseX >= (7.0f ))
            {
                if (mouseX <= (93.5f ))
                {
                    if ((mouseY >= (yMinValues[0])) && (mouseY <= (yMaxValues[0])))
                    {//somewhere valid
                        if (mouseX <= (16.5f ))
                        {
                            menuChoice = 6;
                            delayStep = 1;
                        }
                        else if (mouseX >= (84.0f ))
                        {
                            menuChoice = 5;
                            delayStep = 1;
                        }
                    }
                    else if ((mouseY >= yMinValues[1]) && (mouseY <= yMaxValues[1]))
                    {
                        if (soundOn == true)
                        {
                            soundOn = false;
                            Options.mute();
                        }
                        else
                        {
                            soundOn = true;
                            Options.unMute();
                        }
                    }
                    else if ((mouseX >= (41.0f )) && (mouseX <= (60.0f )))
                    {
                        if ((mouseY >= (yMinValues[2])) && (mouseY <= (yMaxValues[2])))
                        {
                            menuChoice = 1;
                            delayStep = 1;
                        }
                    }
                    else if ((mouseX >= (65.0f )) && (mouseX <= (85.0f )))
                    {
                        if ((mouseY >= (yMinValues[2])) && (mouseY <= (yMaxValues[2])))
                        {
                            if (NewGame.getPrevState() != MENU)
                            {
                                menuChoice = 2;
                                delayStep = 1;
                            }
                        }
                    }
                }
            }
        }
    }

	if (swipe || (delayStep && (menuChoice >= 5)))
	{
		short newStyle  = -1;
		short offDirection = -1;
		short onDirection = -1;

		if(menuChoice == 5)
		{//table top
			if (tableTopStyle + 1 < NUM_TABLE_TOP)
				newStyle = tableTopStyle + 1;
			else
				newStyle = 0;

			offDirection = WEST;
			onDirection = EAST;
            
            mouseDelay = agk::Timer();
		}
		else
		{
			if (tableTopStyle - 1 < 0)
				newStyle = NUM_TABLE_TOP - 1;
			else
				newStyle = tableTopStyle - 1;

			offDirection = EAST;
			onDirection = WEST;
            
            mouseDelay = agk::Timer();
		}

		if (offDirection == EAST)
		{
			//set new image off screen at -width
			Options.ThemeOption[newStyle].setX(-Options.ThemeOption[newStyle].getWidth());

			for (float i = Options.ThemeOption[tableTopStyle].getX(); i < 100.0f; Options.ThemeOption[tableTopStyle].setX(++i))
			{
				for (float j = Options.ThemeOption[newStyle].getX(); ; Options.ThemeOption[newStyle].setX(++j))
					agk::Sync();
			}
		}
		else if (offDirection == WEST)
		{
			Options.ThemeOption[newStyle].setX(100.0f);

			for (float i = Options.ThemeOption[tableTopStyle].getX(); i < -Options.ThemeOption[tableTopStyle].getWidth(); Options.ThemeOption[tableTopStyle].setX(--i))
			{
				for (float j = Options.ThemeOption[newStyle].getX(); ; Options.ThemeOption[newStyle].setX(--j))
					agk::Sync();
			}
		}
		else
		{
            menuChoice = 0;
            tableTopStyle = newStyle;
            swipe = false;
		}

        if (swipe)
            return;
	}

	if (delayStep)
	{
		if (menuChoice < 5)
		{
			if(Options.ButtonAnimation(delayStep, menuChoice))
			{
				bool change = false;

				Options.hide();
				Options.SoundCheck.hide();
				Options.Left.hide();
				Options.Right.hide();

                change = changeTheme(tableTopStyle, init);
                
				if (change)
				{
					CurrentTheme.hide();
					setAnimatedSpriteData();
				}
                				           
				Options.ThemeOption[tableTopStyle].setVisible(false);

				for (float i = Options.Background.getWidth(); i <= 0.0f; Options.Background.setSize(--i))
					agk::Sync();

				NewGame.toggleXState(OPTIONS);
                
                if (menuChoice == 2)
                    NewGame.toggleXState(GROWBACKGROUND);
                else if (menuChoice == 1)
                {
					resetGame();
					NewGame.toggleXState(START);
				}
                
                //clearQueue(&Loading, 1);
				menuChoice = 0;
				delayStep = 0;

				if (change)
				{
					unsigned short int fileID = agk::OpenToWrite("SSDPS.DAT", 0);
					agk::WriteString(fileID, "_Theme_");
					agk::WriteInteger(fileID, tableTopStyle);
					agk::WriteString(fileID, "_Sound_");
					if (soundOn)
						agk::WriteInteger(fileID, 1);
					else
						agk::WriteInteger(fileID, 0);

					agk::WriteString(fileID, "_Language_");
					agk::WriteInteger(fileID, language);

					agk::CloseFile(fileID);
				}
			}
		}
		else
			Options.arrowAnimation(delayStep, menuChoice);
	}

	debug = false;
}

void app::handleInGameOptions(void)
{
	for (float i = Options.Background.getWidth(); i <= 100.0f; Options.Background.setSize(++i))
		agk::Sync();

	Options.show(tableTopStyle);
	
	//Options.update();
#if PLATFORM == PC

	//if((!((strcmp(agk::GetDeviceName(), "windows") == 0) || (strcmp(agk::GetDeviceName(), "osx"))==0)) || debug)
	//{
        if(agk::GetPointerState())
        {
			if ((mouseY >= (23.0f )) && (mouseY <= (68.0f )))
			{
				if (!hold)
				{
					xstartpos = agk::GetPointerX();
					hold = true;
				}
				else
				{
					if (agk::GetPointerX() <= xstartpos - (5.0f ))
					{
						menuChoice = 5;
						hold = false;
					}
					else if (agk::GetPointerX() >= xstartpos + (5.0f ))
					{
						menuChoice = 6;
						hold = false;
					}
				}
			}
		}
		else
			hold = false;
    //}
#endif

	if (mouseClick && !debug)
	{

		if (mouseX >= (7.0f ))
		{
			if (mouseX <= (93.5f ))
			{
				if ((mouseY >= (12.0f )) && (mouseY <= (51.5f )))
				{//somewhere valid
					if (mouseX <= (16.5f ))
					{
						menuChoice = 5;
						delayStep = 1;
					}
					else if (mouseX >= (84.0f ))
					{
						menuChoice = 6;
						delayStep = 1;
					}
				}
				else if ((mouseY >= (70.0f )) && (mouseY <= (80.0f )))
				{
					if (soundOn == true)
					{
						soundOn = false;
						Options.mute();
					}
					else
					{
						soundOn = true;
						Options.unMute();
					}
				}
				else if ((mouseX >= (41.0f )) && (mouseX <= (60.0f )))
				{
					if ((mouseY >= (83.0f )) && (mouseY <= (94.0f )))
					{
						menuChoice = 1;
						delayStep = 1;
					}
				}
				else if ((mouseX >= (65.0f )) && (mouseX <= (85.0f )))
				{
					if ((mouseY >= (81.0f )) && (mouseY <= (91.4f )))
					{
						menuChoice = 2;
						delayStep = 1;
					}

				}
			}
		}
	}

	if (!delayStep && ((menuChoice == 5) || (menuChoice == 6)))
	{
		short newStyle;
		short offDirection;
		short onDirection;

		if(menuChoice == 5)
		{//table top
			if (tableTopStyle + 1 < NUM_TABLE_TOP)
				newStyle = tableTopStyle + 1;
			else
				newStyle = 0;

			offDirection = WEST;
			onDirection = EAST;
		}
		else
		{
			if (tableTopStyle - 1 < 0)
				newStyle = NUM_TABLE_TOP - 1;
			else
				newStyle = tableTopStyle - 1;

			offDirection = EAST;
			onDirection = WEST;
		}

		if (offDirection == EAST)
		{
			//set new image off screen at -width
			Options.ThemeOption[newStyle].setX(-Options.ThemeOption[newStyle].getWidth());

			for (float i = Options.ThemeOption[tableTopStyle].getX(); i < 100.0f; Options.ThemeOption[tableTopStyle].setX(++i))
			{
				for (float j = Options.ThemeOption[newStyle].getX(); ; Options.ThemeOption[newStyle].setX(++j))
					agk::Sync();
			}
		}
		else if (offDirection == WEST)
		{
			Options.ThemeOption[newStyle].setX(100.0f);

			for (float i = Options.ThemeOption[tableTopStyle].getX(); i < -Options.ThemeOption[tableTopStyle].getWidth(); Options.ThemeOption[tableTopStyle].setX(--i))
			{
				for (float j = Options.ThemeOption[newStyle].getX(); ; Options.ThemeOption[newStyle].setX(--j))
					agk::Sync();
			}
		}
		else
		{
			menuChoice = 0;
			tableTopStyle = newStyle;
		}

		debug = false;
		return;
	}

	if (delayStep)
	{
		if (menuChoice < 5)
		{
			if(Options.ButtonAnimation(delayStep, menuChoice))
			{
				changeTheme(tableTopStyle);
				Options.hide();
				Options.ThemeOption[tableTopStyle].setVisible(false);
				Options.Background.setVisible(false);
				Options.SoundCheck.hide();
				NewGame.toggleXState(INGAMEOPTIONS);
				if (menuChoice == 2)
					NewGame.toggleXState(GROWBACKGROUND);
				else
				{
					resetGame();
					NewGame.toggleXState(START);
				}
				menuChoice = 0;
				delayStep = 0;

				unsigned short int fileID = agk::OpenToWrite("SSDPS.DAT", 0);
				agk::WriteString(fileID, "_Theme_");
				agk::WriteInteger(fileID, tableTopStyle);
				agk::WriteString(fileID, "_Sound_");
				if (soundOn)
					agk::WriteInteger(fileID, 1);
				else
					agk::WriteInteger(fileID, 0);

				agk::WriteString(fileID, "_Language_");
				agk::WriteInteger(fileID, language);

				agk::CloseFile(fileID);
			}
		}
		else
			Options.arrowAnimation(delayStep, menuChoice);
	}
}

void app::getThemeDir(short int style, char buffer[])
{
	switch(style)
	{
	case 0:
		strcpy(buffer, "green/");
		break;
	case 1:
		strcpy(buffer, "red/");
		break;
	case 2:
		strcpy(buffer, "ancient/");
		break;
	case 3:
		strcpy(buffer, "xmas/");
		break;
	case 4:
		strcpy(buffer, "electric/");
		break;
	case 5:
		strcpy(buffer, "future/");
		break;
	default:
		strcpy(buffer, "default/");
		break;
	}
}

bool app::changeTheme(short int style, bool init)
{
	//int controlBubbleStart = CONTROL_BUBBLE_START + (9 * style);
	//int trumpSelectionStart = TRUMP_SELECTION_START + (8 * style);
	//int trumpSelectionNegativeStart = TRUMP_SELECTION_NEGATIVE_START + (4 * style);
	bool returnValue = false;

	Loading.setVisible(true);
	LoadScreen1.setVisible(true);
	LoadScreen1.setDepth(3);
	agk::Sync();

	char tempDir[32];

	getThemeDir(style, tempDir);

	if (strcmp(themeDir, tempDir) || init)
	{
        clearQueue(&Loading, 0);
        
		strcpy(themeDir, tempDir);
		CurrentTheme.setup(&Loading, themeDir, NewGame.getState(), aspect);
		returnValue = true;
        CurrentTheme.Deck[0].sortVisualCards(West, North, East, South, CurrentTheme.Deck, round, tableTopStyle);
	}

	//Loading.incrementFrame();
	//agk::Sync();

	Loading.setVisible(false);
	LoadScreen1.setVisible(false);

	return returnValue;
}

void app::handleStart(void)
{
	for (float i = Options.Background.getWidth(); i >= 0.0f; Options.Background.setSize(--i))
		agk::Sync();

	CurrentTheme.Background.setDepth(1);

	CurrentTheme.show(START);
	foundLocation = false;
	playingAnimation = false;
	passInitialAnim = false;
	round = 0;
	count = 0;
	bid = 49;

	CurrentTheme.BCBubble.BidValue.setValue(51);
	CurrentTheme.BCBubble.BidValue.hideAll();

	CurrentTheme.BidScore.setValue(50);
	CurrentTheme.BidScore.display(0, 0);
	CurrentTheme.BidScore.display(1, 5);

	CurrentTheme.PastEastBid.hideAll();
	CurrentTheme.PastNorthBid.hideAll();
	CurrentTheme.PastWestBid.hideAll();
	CurrentTheme.PastSouthBid.hideAll();

	bidWinner = -1;
	hMeld = 0;
	vMeld = 0;
	hTricks = -1;
	vTricks = -1;
	VisualBid = 255;

	NewBidGame.setDealer(dealer);

	dealerOffset[0] = 0;
	dealerOffset[1] = 0;

	CurrentTheme.BidMeldBubbles.setBidSize(39.0f, -1.0f);

	if (dealer == SOUTH)
		dealerOffset[1] = 200;
	else if (dealer == WEST)
		dealerOffset[0] = -200;
	else if (dealer == NORTH)
		dealerOffset[1] = -200;
	else
		dealerOffset[0] = 200;

	for(short int i = 0; i < 80; i++)
	{
		//if (CurrentTheme.Deck[i].hasShrunk())
		//CurrentTheme.Deck[i].setSize(15.0f);
		//CurrentTheme.Deck[i].offset(3.125f, 3.25f);
		CurrentTheme.Deck[i].unRotate();
		CurrentTheme.Deck[i].display(50 + dealerOffset[0], 50 + dealerOffset[1]);
		CurrentTheme.Deck[i].turnFaceDown();
		CurrentTheme.Deck[i].show();
		CurrentTheme.Deck[i].unOutlineCard();
	}

	NewGame.toggleXState(START);
	NewGame.toggleXState(DEALING);
}

void app::handleDealing(void)
{

	//short int random = agk::Random(1, 3);
#if (OS == IOS)
    short random = (arc4random() % 3) + 1;
#else
	short random = (agk::Random(1, 3));
#endif

	switch (random)
	{
	case 1:
		Cards1.play();
		break;
	case 2:
		Cards2.play();
		break;
	case 3:
		Cards3.play();
		break;
	}


	//Background.setPosition(0, 0);
	//Background.setDepth(1);
	for (short int i = 0; i < 80; i += 4)
	{
		ShuffledDeck.deal(cards);  //four at a time
		if (turn == WEST)
			West.dealToHand(cards);

		else if (turn == NORTH)
			North.dealToHand(cards);
		else if (turn == EAST)
			East.dealToHand(cards);
		else
			South.dealToHand(cards);
		turn = (turn + 1) % 4;
	}
	NewGame.toggleXState(DEALING);
	NewGame.toggleXState(ANIMATION);
}

void app::handleAnimation(void)
{

	if (!playingAnimation)
	{
		if (round < 5)
		{
            bool themeCheck;
			foundLocation = false;
            
            if (tableTopStyle == 0)
                themeCheck = true;
            
			if (turn == WEST)
				foundLocation = CurrentTheme.Deck[0].updateCardLocs(turn, CurrentTheme.Deck, West.getCard(round * 4), West.getCard((round * 4) + 1), West.getCard((round * 4) + 2), West.getCard((round * 4) + 3), round, themeCheck);
			else if (turn == NORTH)
				foundLocation = CurrentTheme.Deck[0].updateCardLocs(turn, CurrentTheme.Deck, North.getCard(round * 4), North.getCard((round * 4) + 1), North.getCard((round * 4) + 2), North.getCard((round * 4) + 3), round, themeCheck);
			else if (turn == EAST)
				foundLocation = CurrentTheme.Deck[0].updateCardLocs(turn, CurrentTheme.Deck, East.getCard(round * 4), East.getCard((round * 4) + 1), East.getCard((round * 4) + 2), East.getCard((round * 4) + 3), round, themeCheck);
			else if (turn == SOUTH)
				foundLocation = CurrentTheme.Deck[0].updateCardLocs(turn, CurrentTheme.Deck, South.getCard(round * 4), South.getCard((round * 4) + 1), South.getCard((round * 4) + 2), South.getCard((round * 4) + 3), round, themeCheck);

			if (foundLocation) //at location!
			{
				if (soundOn)
				{
#if (OS != IOS)
					short int randomSound = agk::Random(4, 8);
#else
                    short randomSound = (arc4random() % 5) + 4;
#endif

					switch (randomSound)
					{
					case 4:
						Cards4.play();
					case 5:
						Cards5.play();
						break;
					case 6:
						Cards6.play();
						break;
					case 7:
						Cards7.play();
						break;
					case 8:
						Cards8.play();
						break;
					}
				}

				foundLocation = false;
				turn = (turn + 1) % 4;
				count += 4;

				if (count % 16 == 0)
					round++;

				if (count == 80)
				{
					count = 0;
					turn = 1;
					NewGame.toggleXState(ANIMATION);
					NewGame.toggleXState(SORTING);
				}
			}
		}
	}
	else
	{
		if (allIn)
		{
			Player Selected;

			if (turn == WEST)
				Selected = West;
			else if (turn == NORTH)
				Selected = North;
			else if (turn == EAST)
				Selected = East;
			else if (turn == SOUTH)
				Selected = South;
				
			foundLocation = CurrentTheme.Deck[0].updateCardThrowIn(Selected, CurrentTheme.Deck);

			if (foundLocation)
				count += Selected.getCardsInHand();
		}
		else
			foundLocation = CurrentTheme.Deck[cardPlayed[(count-1)%4]].updateCardLoc(turn, NewGame, NewBidGame, NewTrickGame, count, passInitialAnim);

		if (foundLocation)
		{
			if (soundOn)
			{
#if (OS != IOS)
				short int randomSound = agk::Random(4, 8);
#else
                short randomSound = (arc4random() % 5) + 4;
#endif

				switch (randomSound)
				{
				case 4:
					Cards4.play();
					break;
				case 5:
					Cards5.play();
					break;
				case 6:
					Cards6.play();
					break;
				case 7:
					Cards7.play();
					break;
				case 8:
					Cards8.play();
					break;
				}
			}

			NewGame.toggleXState(ANIMATION);
			playingAnimation = false;
			turn++;
			if (turn > 3)
				turn = 0;
		}
	}

	if (count == 80)
		allIn = false;
}

void app::handleSorting(void)
{
	passInitialAnim = true;
	if (turn)
	{
		//NewGame.toggleXState(SORTING);
		West.sortHand();
		North.sortHand();
		East.sortHand();
		South.sortHand();
		Cards10.play();
		CurrentTheme.Deck[0].sortVisualCards(West, North, East, South, CurrentTheme.Deck, -1, tableTopStyle);
		turn = 0;
	}
	else
	{
		West.tabulateMeld();
		North.tabulateMeld();
		East.tabulateMeld();
		South.tabulateMeld();
		turn = -1;
		count = -1;
		NewGame.toggleXState(SORTING);
		NewGame.toggleXState(BIDDING);
		NBack.setSize(13.5);
		SBack.setSize(13.5);
		EBack.setSize(13.5);
		WBack.setSize(13.5);
		NBack.setVisible(true);
		SBack.setVisible(true);
		WBack.setVisible(true);
		EBack.setVisible(true);
	}
}

void app::handleBidding(void)
{
	pauseGame = false;
	bool playerBid = false;
	char num[4]= {NULL};

	NBack.setSize(13.5f);
	EBack.setSize(13.5f);
	SBack.setSize(13.5f);
	WBack.setSize(13.5f);
	NBack.setVisible(true);
	EBack.setVisible(true);
	SBack.setVisible(true);
	WBack.setVisible(true);

	if (CurrentTheme.BidScore.getValue() < bid)
	{
		incrementing = true;
		return;
	}
	if ((CurrentTheme.BidScore.getValue() == bid) && incrementing)
	{
		pauseGame = true;
		incrementing = false;
		if ((CurrentTheme.PastWestBid.getValue() != West.getlastbid()) && (NewBidGame.playerBid(WEST)))
		{
			if (!NewBidGame.getPass(WEST))
			{
				CurrentTheme.PastWestBid.incrementTo(West.getlastbid());
				incrementing = true;
			}
		}
		if ((CurrentTheme.PastNorthBid.getValue() != North.getlastbid()) && (NewBidGame.playerBid(NORTH)))
		{
			if (!NewBidGame.getPass(NORTH))
			{
				CurrentTheme.PastNorthBid.incrementTo(North.getlastbid());
				incrementing = true;
			}
		}
		if ((CurrentTheme.PastEastBid.getValue() != East.getlastbid()) && (NewBidGame.playerBid(EAST)))
		{
			if (!NewBidGame.getPass(EAST))
			{
				CurrentTheme.PastEastBid.incrementTo(East.getlastbid());
				incrementing = true;
			}
		}
		if ((CurrentTheme.PastSouthBid.getValue() != South.getlastbid()) && (NewBidGame.playerBid(SOUTH)))
		{
			if (!NewBidGame.getPass(SOUTH))
			{
				CurrentTheme.PastSouthBid.incrementTo(South.getlastbid());
				incrementing = true;
			}
		}
		return;
	}

	if (!NewBidGame.playerBid(WEST) && !NewBidGame.getPass(WEST))
	{
		CurrentTheme.PastWestBid.setValue(bid);
		CurrentTheme.PastWestBid.hideAll();
	}
	if (!NewBidGame.playerBid(EAST) && !NewBidGame.getPass(EAST))
	{
		CurrentTheme.PastEastBid.setValue(bid);
		CurrentTheme.PastEastBid.hideAll();
	}
	if (!NewBidGame.playerBid(NORTH) && !NewBidGame.getPass(NORTH))
	{
		CurrentTheme.PastNorthBid.setValue(bid);
		CurrentTheme.PastNorthBid.hideAll();
	}
	if (!NewBidGame.playerBid(SOUTH) && !NewBidGame.getPass(SOUTH))
	{
		CurrentTheme.PastSouthBid.setValue(bid);
		CurrentTheme.PastSouthBid.hideAll();
	}

	incrementing = false;

	if (turn == -1)
	{
		turn = dealer;
		pauseGame = true;
		turn++;
		NewBidGame.setBidder(dealer + 1);
		count++;
	}

	if (!NewBidGame.getBidDecided())  //winning bidder hasn't been determined
	{
		if (count < 4)
		{
			if (turn % 4 == WEST)
				NewBidGame.meldBid(&West, &East);
			else if (turn % 4 == NORTH)
				NewBidGame.meldBid(&North, &South);
			else if (turn % 4 == EAST)
				NewBidGame.meldBid(&East, &West);
			else
				playerBid = southBid();
		}
		else
		{
			if ((turn % 4 == West.getID()) && (!NewBidGame.getPass(West.getID())))
				NewBidGame.takeBid(&West, &East, count/4);
			else if ((turn % 4 == North.getID()) && (!NewBidGame.getPass(North.getID())))
				NewBidGame.takeBid(&North, &South, count/4);
			else if ((turn % 4 == East.getID()) && (!NewBidGame.getPass(East.getID())))
				NewBidGame.takeBid(&East, &West, count/4);
			else if ((turn % 4 == South.getID()) && (!NewBidGame.getPass(South.getID())))
				playerBid = southBid();
		}

		CurrentTheme.BidMeldBubbles.hide();

		if ((turn % 4 != SOUTH) || (playerBid && !menuChoice) || ((NewBidGame.getPass(SOUTH) && (turn % 4 == SOUTH))))
		{
			pauseGame = CurrentTheme.BidMeldBubbles.updateBid(turn, dealer, NewBidGame, tableTopStyle);
			bid = NewBidGame.getBid();
			if (CurrentTheme.BidScore.getValue() < bid)
				incrementing = true;
			else if (pauseGame)
			{
				switch (turn%4)
				{
				case WEST:
					if (CurrentTheme.PastWestBid.getValue() < bid)
						incrementing = true;
					break;
				case NORTH:
					if (CurrentTheme.PastNorthBid.getValue() < bid)
						incrementing = true;
					break;
				case EAST:
					if (CurrentTheme.PastEastBid.getValue() < bid)
						incrementing = true;
					break;
				case SOUTH:
					if (CurrentTheme.PastSouthBid.getValue() < bid)
						incrementing = true;
					break;
				}
			}
			menuChoice = 0;

			switch (turn % 4)
			{
			case WEST:
				if (!NewBidGame.getPass(West.getID()))
				{
					West.setlastbid(bid);
					NewBidGame.setPlayerBid(WEST);
				}
				else if(NewBidGame.getPass(West.getID()))
				{
					CurrentTheme.PastWestBid.setValue(West.getMeldBid() * 10);
				}
				break;
			case NORTH:
				if(!NewBidGame.getPass(North.getID()))
				{
					North.setlastbid(bid);
					NewBidGame.setPlayerBid(NORTH);
				}
				else if (NewBidGame.getPass(North.getID()))
				{
					CurrentTheme.PastNorthBid.setValue(North.getMeldBid() * 10);
				}
				break;
			case EAST:
				if(!NewBidGame.getPass(East.getID()))
				{
					East.setlastbid(bid);
					NewBidGame.setPlayerBid(EAST);
				}
				else if (NewBidGame.getPass(East.getID()))
				{
					CurrentTheme.PastEastBid.setValue(East.getMeldBid() * 10);
				}
				break;
			case SOUTH:
				if (!NewBidGame.getPass(South.getID()))
				{
					South.setlastbid(bid);
					NewBidGame.setPlayerBid(SOUTH);
				}
				else
				{
					CurrentTheme.PastSouthBid.setValue(South.getMeldBid() * 10);
				}

				CurrentTheme.BCBubble.hide();
				break;
			}

			turn++;
			count++;

		}

	}
	else
	{
		char temp[64];

		CurrentTheme.BidMeldBubbles.hide();

		switch (language)
		{
		case ENGLISH:
			switch(NewBidGame.getBidWinner())
			{
			case 0:
				strcpy(temp, "Westley");
				break;
			case 1:
				strcpy(temp, "Norman");
				break;
			case 2:
				strcpy(temp, "Easton");
				break;
			case 3:
				strcpy(temp, "You");
				break;
			}

			strcat(temp, " took the Bid at ");
			break;
		case FRENCH:
			switch(NewBidGame.getBidWinner())
			{
			case 0:
				strcpy(temp, "L'Ouest a");
				break;
			case 1:
				strcpy(temp, "Nord ont");
				break;
			case 2:
				strcpy(temp, "Est");
				break;
			case 3:
				strcpy(temp, "Vous avez");
				break;
			}
			
			strcat(temp, " pris l'enchère à ");
			break;
		case ITALIAN:
			switch(NewBidGame.getBidWinner())
			{
			case 0:
				strcpy(temp, "Ovest ha");
				break;
			case 1:
				strcpy(temp, "Nord ha");
				break;
			case 2:
				strcpy(temp, "Est ha");
				break;
			case 3:
				strcpy(temp, "Hai");
				break;
			}
			
			strcat(temp, " preso l'offert a ");
			break;
		case GERMAN:
			switch(NewBidGame.getBidWinner())
			{
			case 0:
				strcpy(temp, "Westen");
				break;
			case 1:
				strcpy(temp, "Norden");
				break;
			case 2:
				strcpy(temp, "Ost");
				break;
			case 3:
				strcpy(temp, "Sie");
				break;
			}
			
			strcat(temp, " nahm das Gebot bei ");
			break;
		case SPANISH:
			switch(NewBidGame.getBidWinner())
			{
			case 0:
				strcpy(temp, "Occidente");
				break;
			case 1:
				strcpy(temp, "Norte");
				break;
			case 2:
				strcpy(temp, "Este");
				break;
			case 3:
				strcpy(temp, "Que");
				break;
			}
			
			strcat(temp, " tomó la oferta a ");
			break;
		}

		if (NewBidGame.getBid() == 49)
		{
			NewBidGame.setBid(50);
			bid = 50;
		}
		else
			bid = NewBidGame.getBid();

		sprintf(num,"%d",bid);

		strcat(temp, num);

		gameResult.setString(temp);
		gameResult.setAlignment(1); // 1 = center
		
		bidWinner = NewBidGame.getBidWinner();
		turn = bidWinner;

		if (turn == WEST)
			NewTrickGame.setTrumpSuit(West.getStrongSuit());
		else if (turn == NORTH)
			NewTrickGame.setTrumpSuit(North.getStrongSuit());
		else if (turn == EAST)
			NewTrickGame.setTrumpSuit(East.getStrongSuit());
		else
		{
			CurrentTheme.BidMeldBubbles.hide();

			if (South.getNumOfMarriages(HEARTS) + South.getNumOfMarriages(CLUBS) + South.getNumOfMarriages(DIAMONDS) + South.getNumOfMarriages(SPADES) != 0)
				southTrumpSelection();
			else
				NewTrickGame.setTrumpSuit(4);
		}

		count = 0;

		short int trumpSuit = NewTrickGame.getTrumpSuit();

		if (trumpSuit != -1)
		{
			NewGame.toggleXState(BIDDING);
			NewGame.toggleXState(MELDING);

			if (trumpSuit != 4)
			{
				for (short int i = 0; i < 20; i++)
					CurrentTheme.Deck[i+(trumpSuit*20)].outlineCard();

				NewGame.determineAmountToGet(&West, &North, &East, &South, NewTrickGame.getTrumpSuit(), NewBidGame.getBidder(), NewBidGame.getBid());

				CurrentTheme.TSBubble.hide();
			}

			switch (language)
			{
			case ENGLISH:
				switch (trumpSuit)
				{
				case 0: gameResult4.setString("and Named Hearts as Trump"); break;
				case 1: gameResult4.setString("and Named Clubs as Trump"); break;
				case 2: gameResult4.setString("and Named Diamonds as Trump"); break;
				case 3: gameResult4.setString("and Named Spades as Trump"); break;
				case 4:
					char buffer[64] = {NULL};
					sprintf(buffer, "and could not name trump.\nTeam loses %d points", bid);
					gameResult4.setString(buffer);
					break;
				}
				break;
			case FRENCH:
				switch (trumpSuit)
				{
				case 0: gameResult4.setString("coeurs et nommé commete atout"); break;
				case 1: gameResult4.setString("clubs et nommé commete atout"); break;
				case 2: gameResult4.setString("et a nommé les diamants commete atout"); break;
				case 3: gameResult4.setString("et a nommé pique commete atout"); break;
				case 4:
					char buffer[64] = {NULL};
					sprintf(buffer, "et ne pouvais pas le nom trump.\nÉquipe perd %d points.", bid);
					gameResult4.setString(buffer);
					break;
				}
				break;
			case ITALIAN:
				switch (trumpSuit)
				{
				case 0: gameResult4.setString("e nominator cuori come vincente"); break;
				case 1: gameResult4.setString("e nominator club come vincente"); break;
				case 2: gameResult4.setString("e nominato diamanti come vincente"); break;
				case 3: gameResult4.setString("e nominato come picche vincente"); break;
				case 4:
					char buffer[64] = {NULL};
					sprintf(buffer, "e non il nome trump.\nSquadra perde %d punti.", bid);
					gameResult4.setString(buffer);
					break;
				}
				break;
			case GERMAN:
				switch (trumpSuit)
				{
				case 0: gameResult4.setString("und herzen als trumpf namens"); break;
				case 1: gameResult4.setString("und clubs als trumpf namens"); break;
				case 2: gameResult4.setString("und nannte diamanten als trumpf"); break;
				case 3: gameResult4.setString("und pik als trumpf namens"); break;
				case 4:
					char buffer[64] = {NULL};
					sprintf(buffer, "und nicht namen trumpf.\nTeam verliert %d punkte.", bid);
					gameResult4.setString(buffer);
					break;
				}
				break;
			case SPANISH:
				switch (trumpSuit)
				{
				case 0: gameResult4.setString("y el nombre corazones como triunfo"); break;
				case 1: gameResult4.setString("y el nombre de los clubes de triunfo"); break;
				case 2: gameResult4.setString("y el nombre de los diamantes de triunfo"); break;
				case 3: gameResult4.setString("y el nombre espadas como triunfo"); break;
				case 4:
					char buffer[64] = {NULL};
					sprintf(buffer, "y noel nombre de triunfo.\nEquipo pierde %d puntos.", bid);
					gameResult4.setString(buffer);
					break;
				}
				break;
			}
			gameResult.setAlignment(1);
			gameResult.setVisible(true);
			gameResult4.setAlignment(1);
			gameResult4.setVisible(true);
			TapScreen.setPosition(Point(gameResult4.getX(), gameResult4.getY() + 9.0f));
			TapScreen.setVisible(true);
			TextBackground.setDepth(50);
			TextBackground.setPosition(22.0f, 28.0f);
			TextBackground.setSize(60, 25);
			TextBackground.setAlpha(192);
			TextBackground.setVisible(true);
			//gameResult4.setVisible(true)
		}
	}
}

void app::handleMelding(void)
{
	short int bidWinner = NewBidGame.getBidWinner();
	short int skipTo = 0;
	short int trumpSuit = NewTrickGame.getTrumpSuit();
	char cMeldAmount[32] = {NULL};
	short int cardsDisplayed = 0;

	CurrentTheme.BidMeldBubbles.hide();

	CurrentTheme.PastNorthBid.hideAll();
	CurrentTheme.PastEastBid.hideAll();
	CurrentTheme.PastWestBid.hideAll();
	CurrentTheme.PastSouthBid.hideAll();

	NBack.setVisible(false);
	EBack.setVisible(false);
	SBack.setVisible(false);
	WBack.setVisible(false);

	if (count > 8)
	{
		if (( ( trumpSuit == 4 ) || ( NewGame.horizontalMeld < 20 ) ) && ( NewBidGame.getBidWinner() % 2 == BLUE) )
		{
			if (!mouseClick)
			{
				char buffer[256];
				char buffer2[64];

				//DemoEndBack.setVisible(true)
                
                hideAllText();

				switch(language)
				{
				case ENGLISH:
					sprintf(buffer, "Blue Team took the bid\nand did not have\nthe minimum 20 meld points.\nBlue Team goes back %d points\n", bid);

					if (vMeld >= 20)
					{
						sprintf(buffer2, "\nRed Team has more than 20 points\nRed team scores %d points", vMeld);
						strcat(buffer, buffer2);
					}
					break;
				case FRENCH:
					sprintf(buffer, "L'Équipe bleue a pris l'offre\net n'ont pas\nle minimum de %d points de fusioner", bid);

					if (vMeld >= 20)
					{
						sprintf(buffer2, "\nÉquipe rouge a plus de 20 points\nÉquipe marque %d points rouges", vMeld);
						strcat(buffer, buffer2);
					}
					break;
				case ITALIAN:
					sprintf(buffer, "Squadra blu ha preso l'offerta\ne non hanno\nil minimo di 20 punti si fondono");//need re-translation

					if (vMeld >= 20)
					{
						sprintf(buffer2, "\nSquadra rossa ha più di 20 puti\nPunteggi squadra rosso %d punti", vMeld);
						strcat(buffer, buffer2);
					}
					break;
				case GERMAN:
					sprintf(buffer, "Blaue team nahm das angebot\nund nicht die\nmindestens %d punkte verschmelzen", bid);

					if (vMeld >= 20)
					{
						sprintf(buffer2, "\nRote team hat mehr als 20 punkte\nRote team %d punkte", hMeld);
						strcat(buffer, buffer2);
					}
					break;
				case SPANISH:
					sprintf(buffer, "Equipo azul tomó la oferta\ny no tenia\nel minimo de %d puntos se fusionana", bid);

					if (vMeld >= 20)
					{
						sprintf(buffer2, "\nEquipo rojo tiene más de 20 puntos\nResultados del equipo rojo de %d puntos", vMeld);
						strcat(buffer, buffer2);
					}
					break;
				}

				MeldFail.setString(buffer);
				MeldFail.setVisible(true);
                
                TapScreen.setPosition(Point(MeldFail.getX(), MeldFail.getY() + 18.0f));
				TapScreen.setVisible(true);
				TextBackground.setPosition(19.0f, 33.0f);
				TextBackground.setSize(65, 42);
				TextBackground.setVisible(true);
			}
			else
			{

				DemoEndBack.setVisible(false);
				MeldFail.setVisible(false);
				NewGame.toggleXState(MELDING);
				NewGame.toggleXState(SCOREBOARD);

				CurrentTheme.BidMeldBubbles.hide();
			}

			return;
		}
		else if ( ( ( trumpSuit == 4 ) || ( NewGame.verticalMeld < 20 ) ) && ( NewBidGame.getBidWinner() % 2 == RED ) )
		{
			if (!mouseClick)
			{
				char buffer[256] = {NULL};
				char buffer2[64] = {NULL};

				//DemoEndBack.setVisible(true)
				DemoEndBack.setVisible(false);
                
                hideAllText();

				switch(language)
				{
				case ENGLISH:
					sprintf(buffer, "Red Team took the bid\nand did not have\nthe minimum 20 meld points.\nRed Team goes back %d points\n", bid);

					if (hMeld >= 20)
					{
						sprintf(buffer2, "\nBlue Team has more than 20 points\nBlue team scores %d points", hMeld);
						strcat(buffer, buffer2);
					}
					break;
				case FRENCH:
					sprintf(buffer, "Rouges équipe a pris l'offre\net n'ont pas\nle minimum de %d points de fusioner", bid);

					if (hMeld >= 20)
					{
						sprintf(buffer2, "\nÉquipe bleue a plus de 20 points\nÉquipe marque %d points bleus", hMeld);
						strcat(buffer, buffer2);
					}
					break;
				case ITALIAN:
					sprintf(buffer, "Rosso team ha preso l'offerta\ne non hanno\nil minimo di %d punti si fondono", bid);

					if (hMeld >= 20)
					{
						sprintf(buffer2, "\nSquadra blu ha più di 20 puti\nPunteggi squadra blu %d punti", hMeld);
						strcat(buffer, buffer2);
					}
					break;
				case GERMAN:
					sprintf(buffer, "Rot-team nahm das angebot\nund nicht die\nmindestens %d punkte verschmelzen", bid);

					if (hMeld >= 20)
					{
						sprintf(buffer2, "\nBlaue team hat mehr als 20 punkte\nBlaue team %d punkte", hMeld);
						strcat(buffer, buffer2);
					}
					break;
				case SPANISH:
					sprintf(buffer, "Equipo rojo tomó la oferta\ny no tenia\nel minimo de %d puntos se fusionana", bid);

					if (hMeld >= 20)
					{
						sprintf(buffer2, "\nEquipo azul tiene más de 20 puntos\nPuntuaciones de %d puntos del equipo azul", hMeld);
						strcat(buffer, buffer2);
					}
					break;
				}

				MeldFail.setString(buffer);
				MeldFail.setVisible(true);

				TapScreen.setPosition(Point(MeldFail.getX(), MeldFail.getY() + 18.0f));
				TapScreen.setVisible(true);
				TextBackground.setPosition(19.0f, 33.0f);
				TextBackground.setSize(65, 42);
				TextBackground.setVisible(true);
			}
			else
			{
				DemoEndBack.setVisible(false);
				MeldFail.setVisible(false);
				NewGame.toggleXState(MELDING);
				NewGame.toggleXState(SCOREBOARD);

				CurrentTheme.BidMeldBubbles.hide();
			}

			return;
		}
		else
		{
			CurrentTheme.BidMeldBubbles.hide();

			NewTrickGame.buildDecisionTree();
			count = 0;
			NewGame.toggleXState(MELDING);
			NewGame.toggleXState(PLAYING);
			passInitialAnim = true;
			return;
		}
	}

	CurrentTheme.BidMeldBubbles.hide();

	if (count > 8)
	{
		MeldAmount.setVisible(false);
		TapScreen.setVisible(false);
		TextBackground.setVisible(false);
	}

	switch (count)
	{
	case 0:
		if (mouseClick)
		{
			if (bidWinner % 2 == 0)
				skipTo = 3;
			else
				skipTo = 1;
			gameResult.setVisible(false);
			gameResult4.setVisible(false);
			TextBackground.setPosition(25.0f, 65.0f);
			TextBackground.setSize(52, 13);
		}

		break; //else start with west
	case 1:
		if (NewGame.horizontalMeld >= 20)
		{
			cardsDisplayed = displayKnown(WEST);
			switch (language)
			{
			case ENGLISH:
				sprintf(cMeldAmount, "Westley has %d Meld Points", West.getPMeld());
				break;
			case FRENCH:
				sprintf(cMeldAmount, "Ouest a %d points de fusionner", West.getPMeld());
				break;
			case ITALIAN:
				sprintf(cMeldAmount, "Ovest ha %d punti si fondono", West.getPMeld());
				break;
			case GERMAN:
				sprintf(cMeldAmount, "Westen hat %d punkte verschmelzen", West.getPMeld());
				break;
			case SPANISH:
				sprintf(cMeldAmount, "Al oeste con %d puntos se funden", West.getPMeld());
				break;
			}
			if (bidWinner == WEST)
				skipTo = 9; //All Done, exit
			else if (bidWinner == EAST)
				skipTo = 2;
			else
				skipTo = 2;
		}
		else if ((NewGame.verticalMeld >= 20) && (bidWinner % 2 != 0))
		{//Red team has meld, and took bid
			if (bidWinner == SOUTH)
				skipTo = 3; //Send to North
			else if (bidWinner == NORTH)
				skipTo = 4; //Send to South
		}
		else
			skipTo = 9;
		break;
	case 2:
		if (NewGame.horizontalMeld >= 20)
		{
			cardsDisplayed = displayKnown(EAST);
			switch (language)
			{
			case ENGLISH:
				sprintf(cMeldAmount, "Easton has %d Meld Points", East.getPMeld());
				break;
			case FRENCH:
				sprintf(cMeldAmount, "Est a %d points de fusionner", East.getPMeld());
				break;
			case ITALIAN:
				sprintf(cMeldAmount, "Est ha %d punti si fondono", East.getPMeld());
				break;
			case GERMAN:
				sprintf(cMeldAmount, "Osten hat %d punkte verschmelzen", East.getPMeld());
				break;
			case SPANISH:
				sprintf(cMeldAmount, "Esta cuenta con %d puntos se funden", East.getPMeld());
				break;
			}
			if (bidWinner == EAST)
				skipTo = 9; //All Done, exit
			else if (bidWinner == WEST)
				skipTo = 1;
			else if ((NewGame.verticalMeld >= 20) && (bidWinner % 2 != 0))
			{//Red team has meld, and took bid
				if (bidWinner == SOUTH)
					skipTo = 3; //Send to North
				else if (bidWinner == NORTH)
					skipTo = 4; //Send to South
			}
			else //Red team took bit, and has less than 20 meld
				skipTo = 9;
		}
		else if ((NewGame.verticalMeld >= 20) && (bidWinner % 2 != 0))
		{//Red team has meld, and took bid
			if (bidWinner == SOUTH)
				skipTo = 3; //Send to North
			else if (bidWinner == NORTH)
				skipTo = 4; //Send to South
		}
		else
			skipTo = 9;
		break;
	case 3:
		if (NewGame.verticalMeld >= 20)
		{
			cardsDisplayed = displayKnown(NORTH);
			switch (language)
			{
			case ENGLISH:
				sprintf(cMeldAmount, "Norman has %d Meld Points", North.getPMeld());
				break;
			case FRENCH:
				sprintf(cMeldAmount, "Nord a %d points de fusionner", North.getPMeld());
				break;
			case ITALIAN:
				sprintf(cMeldAmount, "Nord ha %d punti si fondono", North.getPMeld());
				break;
			case GERMAN:
				sprintf(cMeldAmount, "Norden hat %d punkte verschmelzen", North.getPMeld());
				break;
			case SPANISH:
				sprintf(cMeldAmount, "Norte cuenta con %d puntos se funden", North.getPMeld());
				break;
			}
			if (bidWinner == NORTH)
				skipTo = 9; //All Done, exit
			else if (bidWinner == SOUTH)
				skipTo = 4;
			else
				skipTo = 4;
		}
		else if ((NewGame.horizontalMeld >= 20) && (bidWinner % 2 != 1))
		{//Red team has meld, and took bid
			if (bidWinner == WEST)
				skipTo = 2; //Send to East
			else if (bidWinner == EAST)
				skipTo = 1; //Send to West
		}
		else
			skipTo = 9;
		break;
	case 4:
		if (NewGame.verticalMeld >= 20)
		{
			cardsDisplayed = displayKnown(SOUTH);
			switch (language)
			{
			case ENGLISH:
				sprintf(cMeldAmount, "You have %d Meld Points", South.getPMeld());
				break;
			case FRENCH:
				sprintf(cMeldAmount, "Vous avez %d points de fusionner", South.getPMeld());
				break;
			case ITALIAN:
				sprintf(cMeldAmount, "Avete %d punti si fondono", South.getPMeld());
				break;
			case GERMAN:
				sprintf(cMeldAmount, "Sie haben %d punkte verschmelzen", South.getPMeld());
				break;
			case SPANISH:
				sprintf(cMeldAmount, "Usted tiene %d puntos se funden", South.getPMeld());
				break;
			}
			if (bidWinner == SOUTH)
				skipTo = 9; //All Done, exit
			else if (bidWinner == NORTH)
				skipTo = 3;
			else if ((NewGame.horizontalMeld >= 20) && (bidWinner % 2 != 1))
			{//Red team has meld, and took bid
				if (bidWinner == WEST)
					skipTo = 2; //Send to East
				else if (bidWinner == EAST)
					skipTo = 1; //Send to West
			}
			else
				skipTo = 9;
		}
		else if ((NewGame.horizontalMeld >= 20) && (bidWinner % 2 != 1))
		{//Red team has meld, and took bid
			if (bidWinner == WEST)
				skipTo = 2; //Send to East
			else if (bidWinner == EAST)
				skipTo = 1; //Send to West
		}
		else
			skipTo = 9;
		break;
	default:
		break;
	}

	if (count && (count <= 4))
	{
		MeldAmount.setPosition(Point(50.0f, 66.0f));
		MeldAmount.setString(cMeldAmount);
		MeldAmount.setVisible(true);

		TapScreen.setPosition(Point(MeldAmount.getX(), MeldAmount.getY() + 5.0f));
		TapScreen.setVisible(true);
	}

	if (mouseClick || !cardsDisplayed)
	{
		if ((mouseX >= (74.4f )) && (mouseX <= (98.0f )))
		{
				if ((mouseY >= 0.0f) && (mouseY <= (15.59f )))
					return;
		}

		CurrentTheme.Deck[0].sortVisualCards(West, North, East, South, CurrentTheme.Deck, -1, tableTopStyle);
		count = skipTo;
	}
}

void app::handlePlaying(void)
{
	trump = NewTrickGame.getTrumpSuit();

	MeldAmount.setVisible(false);
	TapScreen.setVisible(false);
	TextBackground.setVisible(false);

	if (count != 80)
	{

		if ((count % 4 == 0) && count && contFlag)
		{//trick is finished
			if (mouseClick)
			{
				TapScreen.setVisible(false);
				turn = NewTrickGame.getLead();
				foundLocation = CurrentTheme.Deck[cardPlayed[0]].updateShrinkCloseTrick(turn, &CurrentTheme.Deck[cardPlayed[1]], &CurrentTheme.Deck[cardPlayed[2]], &CurrentTheme.Deck[cardPlayed[3]]);
				wipingCards = true;
			}
			else if (wipingCards)
            {
				foundLocation = CurrentTheme.Deck[cardPlayed[0]].updateShrinkCloseTrick(turn, &CurrentTheme.Deck[cardPlayed[1]], &CurrentTheme.Deck[cardPlayed[2]], &CurrentTheme.Deck[cardPlayed[3]]);
                
                if (foundLocation)
                {
                    for (int i = 0; i < 4; i++)
                        cardPlayed[i] = -1;
                }
            }
			else if (!allIn)
			{
				TapScreen.setVisible(true);
				TextBackground.setSize(50, 10);
				TextBackground.setPosition(26.0f, 68.5f);
				TextBackground.setVisible(true);
				foundLocation = false;
			}
		}

		if (count)
		{
			if	(foundLocation)
				wipingCards = false;
			else
				return;
		}

		if ((foundLocation || !count) && !wipingCards)
		{
			if ((turn == NewTrickGame.getLead()) || allIn)
			{
				Player Selected;
				Player Partner;
				int totalCardsInHand;
				int trumpCardsLeft;
				int selectedTrumpCards;
				int partnerTrumpCards;

				switch(turn)
				{
				case WEST:
					Selected = West;
					Partner = East;
					break;
				case NORTH:
					Selected = North;
					Partner = South;
					break;
				case EAST:
					Selected = East;
					Partner = West;
					break;
				case SOUTH:
					Selected = South;
					Partner = North;
					break;
				}

				totalCardsInHand = Selected.getCardsInHand();
				selectedTrumpCards = Selected.getNumOfCardsInRange(-1, trump);
				trumpCardsLeft = NewTrickGame.getNumOfCardsLeft(-1, trump);
				partnerTrumpCards = Partner.getNumOfCardsInRange(-1, trump);

				if (((selectedTrumpCards + partnerTrumpCards == trumpCardsLeft) && (selectedTrumpCards == totalCardsInHand)) || allIn) 
				{
					if (totalCardsInHand < 9)
					{
						int points = Selected.getNumOfCardsInRange(KING, -1);
						points += Selected.getNumOfCardsInRange(TEN, -1);
						points += Selected.getNumOfCardsInRange(ACE, -1);

						CurrentTheme.Deck[0].updateCardThrowIn(Selected, CurrentTheme.Deck);

						
						/*for (int i = 0; i < totalCardsInHand; i++)
						{
							cardPlayed[count%4+i] = Selected.getCard(i);
							if (Selected.getID() == WEST)
								cardPlayed[count%4] = NewTrickGame.playCard(&West, North, East, South, count);
							else if (Selected.getID() == NORTH)
								cardPlayed[count%4] = NewTrickGame.playCard(&North, East, South, West, count);
							else if (Selected.getID() == EAST)
								cardPlayed[count%4] = NewTrickGame.playCard(&East, South, West, North, count);
							else
								cardPlayed[count%4] = NewTrickGame.playCard(&South, West, North, East, count);

							//count++;
						}*/

						NewGame.toggleXState(ANIMATION);
						playingAnimation = true;
						playOneTime = false;
						allIn = true;

						if (NewTrickGame.getLead() % 2 == RED)
							NewTrickGame.addBulkPoints(RED, points);
						else
							NewTrickGame.addBulkPoints(BLUE, points);
					}
				}
			}
			hTricks = NewTrickGame.getHTricks();
			vTricks = NewTrickGame.getVTricks();

			if (!allIn)
			{
				if (turn == WEST)
				{
					//sortVisualCards(South, CurrentTheme.Deck, count/4);
					if (!allIn)
					{
						cardPlayed[count%4] = NewTrickGame.playCard(&West, North, East, South, count);
						West.sortHand();
						CurrentTheme.Deck[0].sortVisualCards(West, CurrentTheme.Deck, tableTopStyle);
					}


				}
				else if (turn == NORTH)
				{
					//sortVisualCards(South, CurrentTheme.Deck, count/4);
					cardPlayed[count%4] = NewTrickGame.playCard(&North, East, South, West, count);
					North.sortHand();
					CurrentTheme.Deck[0].sortVisualCards(North, CurrentTheme.Deck, tableTopStyle);

				}
				else if (turn == EAST)
				{
					//sortVisualCards(South, CurrentTheme.Deck, count/4);
					cardPlayed[count%4] = NewTrickGame.playCard(&East, South, West, North, count);
					East.sortHand();
					CurrentTheme.Deck[0].sortVisualCards(East, CurrentTheme.Deck, tableTopStyle);
				}
				else if (turn == SOUTH)
				{
					if (!allIn)
					{
						SwipeHelp.setVisible(true);
						TextBackground.setPosition(22.5f, 64.0f);
						TextBackground.setSize(58, 13);
						TextBackground.setVisible(true);
						contFlag = false;
						//sortVisualCards(South, CurrentTheme.Deck);
						playOneTime = true;
#if PLATFORM == PC
	//					if((strcmp(agk::GetDeviceName(), "windows") == 0) || (strcmp(agk::GetDeviceName(), "osx"))==0)
		//				{
							cardPlayed[count%4] = CurrentTheme.Deck[0].updateCardPos(CurrentTheme.Deck, South, NewTrickGame.getTopSuit(), NewTrickGame.getTopRank(), NewTrickGame.getLeadSuit(), NewTrickGame.getTrumpSuit(), count);
							//mClick=mouseClick;
			//			}
#else
				//		else
					//	{
							if(agk::GetPointerState())
							{
								if (!hold)
								{
									ystartpos=agk::GetPointerY();
									hold = true;
								}
								else
								{
									if (agk::GetPointerY() <= ystartpos - 5.0f)
									{
										swipe = true;
										hold = false;
									}
								}
                            
								if (!swipe)
										cardPlayed[count%4] = CurrentTheme.Deck[0].updateCardPos(CurrentTheme.Deck, South, NewTrickGame.getTopSuit(), NewTrickGame.getTopRank(), NewTrickGame.getLeadSuit(), NewTrickGame.getTrumpSuit(), count);
                            
							}
					//
#endif
		#if (OS == WINDOWS)
						if (mouseClick && (cardPlayed[count%4] != -1))
		#else
						if (swipe && (cardPlayed[count%4] != -1))
		#endif
						{
							if ((!NewTrickGame.getTricksPlayed()) && (bidWinner == SOUTH))
								NewTrickGame.setLead(SOUTH);

							if (turn == SOUTH)
							{
								SwipeHelp.setVisible(false);
								TextBackground.setVisible(false);
							}

							NewTrickGame.playCard(&South, West, North, East, count, cardPlayed[count%4]);
							count++;
							South.sortHand();
							CurrentTheme.Deck[0].sortVisualCards(South, CurrentTheme.Deck, tableTopStyle);

							NewGame.toggleXState(ANIMATION);
							playingAnimation = true;
							contFlag = true;
							playOneTime = false;
							swipe = false;
						}
					}
				}
			}
			if (turn != SOUTH)
			{
				if (!allIn)
				{
					NewGame.toggleXState(ANIMATION);
					playingAnimation = true;
					count++;
					playOneTime = false;
				}
			}

			if (ShuffledDeck.getSuit(cardPlayed[(count-1)%4]) == NewTrickGame.getTrumpSuit())
			{
				if (NewTrickGame.getLead() == turn)
				{
					if (NewTrickGame.getLeadSuit() != trump)
					{
						if (!playOneTime)
						{
							if (soundOn)
							{
								switch (ShuffledDeck.getRank(cardPlayed[(count-1)%4]))
								{
								case ACE:
									Trump[ACE].play();
									break;
								case TEN:
									Trump[TEN].play();
									break;
								case KING:
									Trump[KING].play();
									break;
								case QUEEN:
									Trump[QUEEN].play();
									break;
								case JACK:
									Trump[JACK].play();
									break;
								}
							}

							playOneTime = true;
						}
					}
				}
			}
		}
	}
	else
	{
		if (mouseClick)
		{
			NewGame.toggleXState(PLAYING);
			NewGame.toggleXState(SCOREBOARD);
		}
		else
		{
			TapScreen.setVisible(true);
			TextBackground.setSize(50, 10);
			TextBackground.setPosition(26.0f, 68.5f);
			TextBackground.setVisible(true);
		}
	}
}

void app::handleScoreboard(void)
{
	bool meldFail = false;
	MeldAmount.setVisible(false);
	TapScreen.setVisible(false);
	turn = NewTrickGame.getLead();
	ingamebutton=false;
	incrementing = false;

	bid = NewBidGame.getBid();

	if (bid == 49)
		bid = 50;


	if (turn == -1)
		meldFail = true;

	if (!calcOnce && !meldFail)
	{
		if (turn % 2 == 0)
			NewTrickGame.lastTrick(BLUE);
		else
			NewTrickGame.lastTrick(RED);

		hTricks = NewTrickGame.getHTricks();
		vTricks = NewTrickGame.getVTricks();
        
		NewGame.checkForSaves(hTricks, vTricks, bidWinner, bid);
		calcOnce = true;
	}
	else if (meldFail && !calcOnce)
	{
		if (bidWinner % 2 == BLUE)
		{
			if (vMeld >= 20)
				NewGame.addToScore(RED, vMeld);
		}
		else
		{
			if (hMeld >= 20)
				NewGame.addToScore(BLUE, hMeld);
		}
        
        NewGame.addToScore(bidWinner % 2, -bid);
		calcOnce = true;
	}

	//CurrentTheme.HScore.incrementTo(abs(NewGame.getScore(0)));
	//CurrentTheme.VScore.incrementTo(abs(NewGame.getScore(1)));

	if (!mouseClick && (visScreen == 0))
	{
		if (vMeld < 20)
			vMeld = 0;
		if (hMeld < 20)
			hMeld = 0;

		short int vTrickScore;
		short int hTrickScore;

		if (vTricks < 20)
			vTrickScore = 0;
		else
			vTrickScore = vTricks;

		if (hTricks < 20)
			hTrickScore = 0;
		else
			hTrickScore = hTricks;

		HandResults.HandScoreData[0][0].showAll();

		if (bidWinner % 2 == RED)
		{
			if (HandResults.HandScoreData[0][3].getValue() != bid)
            {
				HandResults.HandScoreData[0][3].incrementTo(bid); //TODO: change to 4 when new scoreboard in
                PointClick.play();
				incrementing = true;
            }

			HandResults.HandScoreData[1][3].hideAll();

			if (CurrentTheme.V2Get.getValue() == 0)
			{
				if (HandResults.HandScoreData[0][0].getValue() != (vMeld + vTrickScore))
                {
					HandResults.HandScoreData[0][0].incrementTo(vMeld + vTrickScore);
                    PointClick.play();
					incrementing = true;
                }

				HandResults.VCheckOn.setVisible(true);
			}
			else
			{
				if (HandResults.HandScoreData[0][0].getValue() != -bid)
                {
					HandResults.HandScoreData[0][0].incrementTo(-bid);
                    PointClick.play();
					incrementing = true;
                }

				HandResults.VCheck.setVisible(true);
			}
			if ((CurrentTheme.H2Get.getValue() == 0) || meldFail)
			{
				if (HandResults.HandScoreData[1][0].getValue() != (hMeld + hTrickScore))
                {
					HandResults.HandScoreData[1][0].incrementTo(hMeld + hTrickScore);
                    PointClick.play();
					incrementing = true;
                }

				HandResults.HCheckOn.setVisible(true);
			}
			else
			{
				HandResults.HandScoreData[1][0].incrementTo(0, true);
				HandResults.HCheck.setVisible(true);
			}
		}
		else
		{
			if (HandResults.HandScoreData[1][3].getValue() != bid)
            {
				HandResults.HandScoreData[1][3].incrementTo(bid); //TODO: change to 4 when new scoreboard in
				incrementing = true;
                PointClick.play();
            }

			HandResults.HandScoreData[0][3].hideAll();

			if (CurrentTheme.H2Get.getValue() == 0)
			{
				if (HandResults.HandScoreData[1][0].getValue() != (hMeld + hTrickScore))
                {
					HandResults.HandScoreData[1][0].incrementTo(hMeld + hTrickScore);
					incrementing = true;
                    PointClick.play();
                }

				HandResults.HCheckOn.setVisible(true);
			}
			else
			{
				if (HandResults.HandScoreData[1][0].getValue() != -bid)
                {
					HandResults.HandScoreData[1][0].incrementTo(-bid);
					incrementing = true;
                    PointClick.play();
                }

				HandResults.HCheck.setVisible(true);
			}

			if ((CurrentTheme.V2Get.getValue() == 0) || meldFail)
			{
				if (HandResults.HandScoreData[0][0].getValue() != (vMeld + vTrickScore))
                {
					HandResults.HandScoreData[0][0].incrementTo(vMeld + vTrickScore);
					incrementing = true;
                    PointClick.play();
                }

				HandResults.VCheckOn.setVisible(true);
			}
			else
			{
				HandResults.HandScoreData[0][0].incrementTo(0, true);
				HandResults.VCheck.setVisible(true);
			}
		}

		if (HandResults.HandScoreData[0][2].getValue() != vMeld)
        {
            HandResults.HandScoreData[0][2].incrementTo(vMeld, true);
			incrementing = true;
            PointClick.play();
        }
		if (HandResults.HandScoreData[1][2].getValue() != hMeld)
        {
            HandResults.HandScoreData[1][2].incrementTo(hMeld, true);
			incrementing = true;
            PointClick.play();
        }
		if (HandResults.HandScoreData[0][1].getValue() != NewTrickGame.getVTricks())
        {
            HandResults.HandScoreData[0][1].incrementTo(NewTrickGame.getVTricks(), true);
			incrementing = true;
            PointClick.play();
        }
		if (HandResults.HandScoreData[1][1].getValue() != NewTrickGame.getHTricks())
        {
            HandResults.HandScoreData[1][1].incrementTo(NewTrickGame.getHTricks(), true);
			incrementing = true;
            PointClick.play();
        }

		for (int i = 0; i < 80; i++)
			CurrentTheme.Deck[i].hide();

		HandResults.Background.setVisible(true);
		//HandResults.Background.setDepth(50);
#if (PLATFORM == MOBILE)
		TapScreen.setPosition(Point((TapScreen.getX() + 5.0f, 70.0f);
#else
		TapScreen.setPosition(Point(TapScreen.getX(), 70.0f));
#endif
		TapScreen.setVisible(true);
		TextBackground.setSize(50, 10);
		TextBackground.setPosition(26.0f, 68.5f);
		TextBackground.setVisible(true);
	}
	else if ((visScreen == 1) && !mouseClick)
	{
		//incrementing = false;
		TapScreen.setPosition(Point(50.0f, 7.0f));
		TapScreen.setVisible(true);
		TextBackground.setPosition(26.0f, 5.0f);

		HandResults.hide();

		GameResults.Background.setVisible(true);
		GameResults.Background.setAlpha(255);
		GameResults.Header.setVisible(true);

		if (hand > 0)
		{
			if (!copyOnce)
			{
				if (hand > 10)
				{
					for (short int i = 0; i < 10; i++)
					{
						GameResults.GameScoreData[i][0].hideAll();
						GameResults.GameScoreData[i][1].hideAll();
						GameResults.GameScoreData[i][2].hideAll();
						GameResults.GameScoreData[i][0].setValue(GameResults.GameScoreData[i+1][0].getValue());
						GameResults.GameScoreData[i][1].setValue(GameResults.GameScoreData[i+1][1].getValue());
						GameResults.GameScoreData[i][2].setValue(GameResults.GameScoreData[i+1][2].getValue());
						copyOnce = true;
					}

					GameResults.GameScoreData[10][0].setValue(0);
					GameResults.GameScoreData[10][1].setValue(0);
				}
				/*else
				{
					GameResults.MiddleSection[hand].setPosition(GameResults.MiddleSection[hand-1].getX(), GameResults.MiddleSection[hand-1].getY() + (5.0f * (hand)));
					copyOnce = true;
				}*/
			}
		}

		for (short int i = 0; i < hand + 1; i++)
		{
			if (i > 10)
				break;

			GameResults.MiddleSection[i].setVisible(true);
		}

		GameResults.Total.setVisible(true);

		short int temphand = hand;

		if (temphand > 10)
			temphand = 10;

		hTricks = NewTrickGame.getHTricks();
		if (hTricks < 20)
			hTricks = 0;

		vTricks = NewTrickGame.getVTricks();
		if (vTricks < 20)
			vTricks = 0;

		//hMeld = NewGame.horizontalMeld;
		//vMeld = NewGame.verticalMeld;

		if (hMeld < 20)
			hMeld = 0;
		if (vMeld < 20)
			vMeld = 0;

		if (bidWinner % 2 == BLUE)
		{
			if (CurrentTheme.H2Get.getValue() == 0)
			{
				if (GameResults.GameScoreData[temphand][0].getValue() != (hMeld + hTricks))
                {
					GameResults.GameScoreData[temphand][0].incrementTo(hMeld + hTricks);
                    PointClick.play();
					incrementing = true;
                }
			}
			else
			{
				if (GameResults.GameScoreData[temphand][0].getValue() != -bid)
                {
					GameResults.GameScoreData[temphand][0].incrementTo(-bid); //need to make this negative somehow!
                    PointClick.play();
					incrementing = true;
                }
			}

			if ((CurrentTheme.V2Get.getValue() == 0) || (meldFail && vMeld))
			{
				if (GameResults.GameScoreData[temphand][1].getValue() != (vMeld + vTricks))
                {
					GameResults.GameScoreData[temphand][1].incrementTo(vMeld + vTricks);
                    PointClick.play();
					incrementing = true;
                }
			}
			else
			{
				GameResults.GameScoreData[temphand][1].incrementTo(0, true);
			}
		}
		else
		{
			if ((CurrentTheme.H2Get.getValue() == 0) || (meldFail && hMeld))
			{
				if (GameResults.GameScoreData[temphand][0].getValue() != (hMeld + hTricks))
                {
					GameResults.GameScoreData[temphand][0].incrementTo(hMeld + hTricks);
                    PointClick.play();
					incrementing = true;
                }
			}
			else
			{
				GameResults.GameScoreData[temphand][0].incrementTo(0, true);
			}

			if (CurrentTheme.V2Get.getValue() == 0)
			{
				if (GameResults.GameScoreData[temphand][1].getValue() != (vMeld + vTricks))
                {
					GameResults.GameScoreData[temphand][1].incrementTo(vMeld + vTricks);
                    PointClick.play();
					incrementing = true;
                }
			}
			else
			{
				if (GameResults.GameScoreData[temphand][1].getValue() != -bid)
                {
					GameResults.GameScoreData[temphand][1].incrementTo(-bid); //need to make this negative
                    PointClick.play();
					incrementing = true;
                }
			}
		}

		GameResults.GameScoreData[temphand][2].hideAll();
		GameResults.GameScoreData[temphand][2].setValue(hand+1);

		for (short int g = 0; g < hand; g++)
		{
			for (short int h = 0; h < 3; h++)
				GameResults.GameScoreData[g][h].showAll(true);
		}

        if (GameResults.GameScoreData[11][0].getValue() != hScore)
        {
            GameResults.GameScoreData[11][0].incrementTo(NewGame.getScore(BLUE), true);
            PointClick.play();
			incrementing = true;
        }
        
        if (GameResults.GameScoreData[11][1].getValue() != vScore)
        {
            GameResults.GameScoreData[11][1].incrementTo(NewGame.getScore(RED), true);
            PointClick.play();
			incrementing = true;
        }

		GameResults.GameScoreData[11][0].showAll(true);
		GameResults.GameScoreData[11][1].showAll(true);
		GameResults.GameScoreData[11][2].hideAll();

		if ((NewGame.getScore(0) >= 500) || (NewGame.getScore(1) >= 500))
		{
			GameOver.setPosition(Point(50.0f, 89.0f));
			if ((NewGame.getScore(0) >= 500) && (NewGame.getScore(1) < 500))
				GameOver.setString("Blue Team Wins!");
			else if ((NewGame.getScore(1) >= 500) && (NewGame.getScore(0) < 500))
				GameOver.setString("Red Team Wins!");
			else
			{
				if (bidWinner % 2)
					GameOver.setString("Red Team Wins!");
				else
					GameOver.setString("Blue Team Wins!");
			}
			//GameOver.setColor(RGBA;
			GameOver.setVisible(true);
		}
	}
	else if (visScreen == 2)
	{
		GameResults.hide();
		TapScreen.setVisible(false);
		TextBackground.setVisible(false);

		if ((NewGame.getScore(0) < 500) && (NewGame.getScore(1) < 500))
		{
			resetHand();
			NewGame.toggleXState(SCOREBOARD);
			NewGame.toggleXState(START);
			calcOnce = false;
			visScreen = 0;
			copyOnce = false;
		}
		else
			visScreen++;
	}
	else if (visScreen == 3)
	{
		//So and so Wins!
		resetGame();
		CurrentTheme.hide();

		hScore = 0;
		vScore = 0;
		visScreen = 0;
		hand = 0;
		delayStep = 0;
		calcOnce = false;
		copyOnce = false;

		GameResults.hide();

		for (float i = CurrentTheme.Background.getWidth(); i >= 0.0f; CurrentTheme.Background.setSize(--i))
			agk::Sync();

		NewGame.toggleXState(SCOREBOARD);
		NewGame.toggleXState(MENU);
	}
	/*else if (!incrementing)
	{
		visScreen++;
//		incrementing = false;
	}*/

	if (mouseClick && !incrementing)
		visScreen++;
}

void app::handleLoading(void)
{
	switch (count)
	{
	case 0:
        fadeSteeleLogo();
        break;
	case 1:
		loadLoadScreen();
		break;
	case 2:
		initLoadScreen();
		break;
	case 3:
		initVars();
		break;
	case 4:
        parseFile(); 
		break;
	case 5:
		setSoundData();
		break;
	case 6:
		setNumberData();
		break;
	case 7:
		MainMenu.setup(&Loading);
		break;
	case 8:
		Options.setup(&Loading);
		break;
	case 9:
		HandResults.setup(&Loading);
		break;
	case 10:
        GameResults.setup(&Loading, &HandResults.Parent);
        break;
	case 11:
		setSpriteData();
		break;
	case 12:
		setAnimatedSpriteData();
		break;
	case 13:
		finalSetup();
		break;
	case 14:
		NewGame.toggleXState(LOADING);
		//NewGame.toggleXState(MENU);
		count = -1;
		//Loading.hide();
		break;
	}

	count++;
}

void app::testScale(void)
{
	//for (short i = 0; i < 10; i++)
		//CurrentTheme.BCBubble.BidValue.setLocation(i, 54.0 - (6.5f * i), 46.5f, 0);
}

void app::writePlayerData(short unsigned int fileID, Player Selected)
{
	short int state = NewGame.getState();

	agk::WriteString(fileID, "Hand");
	for (short int i = 0; i < Selected.getCardsInHand(); i++)
	{
		agk::WriteString(fileID, "|");
		agk::WriteInteger(fileID, West.getCard(i));
	}

	if (state == PLAYING)
	{

		agk::WriteString(fileID, "Known");
		for (short int i = 0; i < 5; i++)
		{
			for (short int j = 0; j < 4; j++)
			{
				agk::WriteString(fileID, "|");
				agk::WriteInteger(fileID, Selected.known[i][j]);
			}
		}

		agk::WriteString(fileID, "Trumping");
		for (short int i = 0; i < 4; i++)
		{
			agk::WriteString(fileID, "|");
			if (Selected.trumpingSuit[i])
				agk::WriteInteger(fileID, 1);
			else
				agk::WriteInteger(fileID, 0);
		}
	}
	if (state == BIDDING)
	{
		agk::WriteString(fileID, "Bidding");
		agk::WriteString(fileID, "|");

		if (NewBidGame.getPass(Selected.getID()))
		{
			agk::WriteInteger(fileID, 1);
			agk::WriteString(fileID, "|");
			agk::WriteInteger(fileID, NewBidGame.getPassRound(Selected.getID()));
		}
		else
			agk::WriteInteger(fileID, 0);

	}
}
void app::formatPlayerData(short unsigned int fileID)
{
	agk::WriteString(fileID, "_WestData_");
	writePlayerData(fileID, West);
	agk::WriteString(fileID, "_NorthData_");
	writePlayerData(fileID, North);
	agk::WriteString(fileID, "_EastData_");
	writePlayerData(fileID, East);
	agk::WriteString(fileID, "_SouthData_");
	writePlayerData(fileID, South);
}

void app::formatDeckData(short unsigned int fileID)
{
	agk::WriteString(fileID, "_DeckData_");
	agk::WriteString(fileID, "Deck");
	for (short int i = 0; i < 80; i++)
	{
		agk::WriteString(fileID, "|");
		agk::WriteInteger(fileID, ShuffledDeck.getCard(i));
	}
}

void app::formatScores(short unsigned int fileID)
{
	agk::WriteString(fileID, "_Scores_");
	agk::WriteString(fileID, "Horizontal");
	agk::WriteString(fileID, "|");
	agk::WriteInteger(fileID, hScore);
	agk::WriteString(fileID, "Vertical");
	agk::WriteString(fileID, "|");
	agk::WriteInteger(fileID, vScore);
}

void app::formatBids(short unsigned int fileID)
{
	agk::WriteString(fileID, "_BidData_");
	agk::WriteString(fileID, "CurrentBid");
	agk::WriteString(fileID, "|");
	agk::WriteInteger(fileID, bid);
	agk::WriteString(fileID, "NumOfBids");
	agk::WriteString(fileID, "|");
	agk::WriteInteger(fileID, West.getNumOfBids());
	agk::WriteString(fileID, "|");
	agk::WriteInteger(fileID, North.getNumOfBids());
	agk::WriteString(fileID, "|");
	agk::WriteInteger(fileID, East.getNumOfBids());
	agk::WriteString(fileID, "|");
	agk::WriteInteger(fileID, South.getNumOfBids());
}

void app::formatMelds(short unsigned int fileID)
{
	//don't need to do anything special, just restart meldin process from count = 0;
}

void app::formatGame(short unsigned int fileID)
{
	agk::WriteString(fileID, "_GameData_");
	agk::WriteString(fileID, "Lead");
	agk::WriteString(fileID, "|");
	agk::WriteInteger(fileID, NewTrickGame.getLead());
	agk::WriteString(fileID, "CardsPlayed");
	agk::WriteInteger(fileID, count%4);
	for (short int i = 0; i < count%4; i++)
	{
		agk::WriteString(fileID, "|");
		agk::WriteInteger(fileID, cardPlayed[i]);
	}
	agk::WriteString(fileID, "Meld");
	agk::WriteString(fileID, "Vertical");
	agk::WriteString(fileID, "|");
	agk::WriteInteger(fileID, NewGame.verticalMeld);
	agk::WriteString(fileID, "Horizontal");
	agk::WriteString(fileID, "|");
	agk::WriteInteger(fileID, NewGame.horizontalMeld);
	agk::WriteString(fileID, "BidWinner");
	agk::WriteString(fileID, "|");
	agk::WriteInteger(fileID, NewBidGame.getBidWinner());
	agk::WriteString(fileID, "Bid");
	agk::WriteString(fileID, "|");
	agk::WriteInteger(fileID, bid);
}

void app::parseFile(void)
{
	unsigned short int fileID;
	bool getQuery = true;
	std::string query;

	if (agk::GetFileExists("SSDPS.DAT"))
		fileID = agk::OpenToRead("SSDPS.DAT");
	else
		return;

	while (!agk::FileEOF(fileID))
	{
		if (getQuery)
			query = agk::ReadString(fileID);

		if (query == "_State_")
		{
			NewGame.toggleXState(NewGame.getState());
			NewGame.toggleXState(agk::ReadInteger(fileID));
		}
		else if (query == "_Theme_")
		{
			tableTopStyle = agk::ReadInteger(fileID);
			getThemeDir(tableTopStyle, themeDir);
			//changeTheme(tableTopStyle);
		}
		else if (query == "_Language_")
			language = agk::ReadInteger(fileID);
#if DEMOBUILD
		else if (query == "_DemoTime_")
		{
			if (DEMOBUILD)
				timeUsedPrev = agk::ReadFloat(fileID);
		}
		else if (query == "_DemoSeed_")
		{
			if (DEMOBUILD)
				ShuffledDeck.setDeckCounter(agk::ReadInteger(fileID));
		}
#endif
		else if (query == "_Sound_")
		{
			if (agk::ReadInteger(fileID))
			{
				soundOn = true;
				Options.unMute();
				PointClick.setSystemVolume(100);
			}
			else
			{
				soundOn = false;
				Options.mute();
				PointClick.setSystemVolume(0);
			}
		}
		else if (query == "Turn")
			turn = agk::ReadInteger(fileID);
		else if (query == "Count")
			count = agk::ReadInteger(fileID);
		else if (query == "Dealer")
			dealer = agk::ReadInteger(fileID);
		else if (query == "_WestData_")
		{
			parsePlayerData(fileID, &West);
			parsePlayerData(fileID, &North);
			parsePlayerData(fileID, &East);
			query = parsePlayerData(fileID, &South);
			getQuery = false;
		}
		else if (query == "_Scores_")
		{
			agk::ReadString(fileID);
			agk::ReadString(fileID);
			hScore = agk::ReadInteger(fileID);
			agk::ReadString(fileID);
			agk::ReadString(fileID);
			vScore = agk::ReadInteger(fileID);
			getQuery = true;
		}
		else if (query == "_DeckData_")
		{
			char savedDeck[80];
			agk::ReadString(fileID);
			for (short int i = 0; i < 80; i++)
			{
				agk::ReadString(fileID);
				savedDeck[i] = agk::ReadInteger(fileID);
			}

			ShuffledDeck.setDeck(savedDeck);
		}
		else if (query == "_BidData_")
		{
			agk::ReadString(fileID); //currentBid
			agk::ReadString(fileID); //|
			NewBidGame.setBid(agk::ReadInteger(fileID));
			West.tabulateMeld();
			North.tabulateMeld();
			East.tabulateMeld();
			South.tabulateMeld();

			agk::ReadString(fileID);//numofbids
			agk::ReadString(fileID);//|
			short int numOfBids = agk::ReadInteger(fileID);
			for (short int i = 0; i < numOfBids; i++)
				West.incrementNumOfBids();

			agk::ReadString(fileID); //|
			numOfBids = agk::ReadInteger(fileID);
			for (short int i = 0; i < numOfBids; i++)
				North.incrementNumOfBids();

			agk::ReadString(fileID); //|
			numOfBids = agk::ReadInteger(fileID);
			for (short int i = 0; i < numOfBids; i++)
				East.incrementNumOfBids();

			agk::ReadString(fileID); //|
			numOfBids = agk::ReadInteger(fileID);
			for (short int i = 0; i < numOfBids; i++)
				South.incrementNumOfBids();
		}
		else if (query == "_GameData_")
		{
			agk::ReadString(fileID);
			agk::ReadString(fileID);
			NewTrickGame.setLead(agk::ReadInteger(fileID));
			agk::ReadString(fileID);
			short int numOfCards = agk::ReadInteger(fileID);
			for (short int i = 0; i < numOfCards; i++)
			{
				agk::ReadString(fileID);
				cardPlayed[i] = agk::ReadInteger(fileID);
			}
			agk::ReadString(fileID);
			agk::ReadString(fileID);
			agk::ReadString(fileID);
			NewGame.verticalMeld = agk::ReadInteger(fileID);
			agk::ReadString(fileID);
			agk::ReadString(fileID);
			NewGame.horizontalMeld = agk::ReadInteger(fileID);
			agk::ReadString(fileID);
			agk::ReadString(fileID);
			bidWinner = agk::ReadInteger(fileID);
			NewBidGame.setBidder(bidWinner);
			for (short int i = 0; i < 4; i++)
			{
				if (i != bidWinner)
					NewBidGame.setPass(i);
			}
			agk::ReadString(fileID);
			agk::ReadString(fileID);
			NewBidGame.setBid(agk::ReadInteger(fileID));
		}
	}

	//makeVisible(); //make everything visible
	//reDeal();
}

void app::reDeal()
{
	short int westToDeal = West.getCardsInHand();
	short int northToDeal = North.getCardsInHand();
	short int eastToDeal = East.getCardsInHand();
	short int southToDeal = South.getCardsInHand();
	short int totalToDeal = westToDeal + northToDeal + eastToDeal + southToDeal;
	short int newRound = 0;
	short int newTurn = (dealer + 1) % 4;

	while (totalToDeal > 0)
	{

		if (newTurn == WEST)
			foundLocation = CurrentTheme.Deck[0].updateCardLocs(newTurn, CurrentTheme.Deck, West.getCard(newRound * 4), West.getCard((newRound * 4) + 1), West.getCard((newRound * 4) + 2), West.getCard((newRound * 4) + 3), newRound);
		else if (newTurn == NORTH)
			foundLocation = CurrentTheme.Deck[0].updateCardLocs(newTurn, CurrentTheme.Deck, North.getCard(newRound * 4), North.getCard((newRound * 4) + 1), North.getCard((newRound * 4) + 2), North.getCard((newRound * 4) + 3), newRound);
		else if (newTurn == EAST)
			foundLocation = CurrentTheme.Deck[0].updateCardLocs(newTurn, CurrentTheme.Deck, East.getCard(newRound * 4), East.getCard((newRound * 4) + 1), East.getCard((newRound * 4) + 2), East.getCard((newRound * 4) + 3), newRound);
		else if (newTurn == SOUTH)
			foundLocation = CurrentTheme.Deck[0].updateCardLocs(newTurn, CurrentTheme.Deck, South.getCard(newRound * 4), South.getCard((newRound * 4) + 1), South.getCard((newRound * 4) + 2), South.getCard((newRound * 4) + 3), newRound);

		if (foundLocation) //at location!
		{
			foundLocation = false;
			newTurn = (newTurn + 1) % 4;
			totalToDeal -= 4;
		}
	}
}

std::string app::parsePlayerData(short unsigned int fileID, Player *Selected)
{
	//Stubbing out


	//bool lastField = false;
	std::string query = agk::ReadString(fileID);
/*
	while (!lastField)
	{
		if (query == "Hand")
		{
			query = agk::ReadString(fileID);

			while (query == "|")
			{
				Selected->dealToHand(agk::ReadInteger(fileID));
				query = agk::ReadString(fileID);
			}
		}
		else if (query == "Known")
		{
			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					query = agk::ReadString(fileID);
					Selected->known[i][j] = agk::ReadInteger(fileID);
				}
			}

			query = agk::ReadString(fileID);
		}
		else if (query == "Trumping")
		{
			for (int i = 0; i < 4; i++)
			{
				agk::ReadString(fileID);

				if (agk::ReadInteger(fileID))
					Selected->trumpingSuit[i] == true;
				else
					Selected->trumpingSuit[i] == false;
			}
		}
		else if (query == "Bidding")
		{
			query = agk::ReadString(fileID);

			if (query == "|")
			{
				if (agk::ReadInteger(fileID))
				{
					agk::ReadString(fileID);
					NewBidGame.setPass(Selected->getID(), agk::ReadInteger(fileID));
				}
			}
			//lastField = true;
		}
		else if (query == "|")
			query = agk::ReadString(fileID);
		else
			lastField = true;
	}
	*/
	return query;

}

void app::doEndGameSwitch(short unsigned int fileID, short int gameState)
{
	/*char prevState = NewGame.getPrevState();

	if ((gameState != OPTIONS) && (gameState != ANIMATION) && (gameState != MENU))
	{
		agk::WriteString(fileID, "_State_");
		agk::WriteInteger(fileID, gameState);*/

		agk::WriteString(fileID, "Theme");
		agk::WriteInteger(fileID, tableTopStyle);

		/*agk::WriteString(fileID, "Turn");
		agk::WriteInteger(fileID, turn);

		agk::WriteString(fileID, "Count");
		if (gameState == MELDING)
			agk::WriteInteger(fileID, 0); //start over
		else
			agk::WriteInteger(fileID, count);

		agk::WriteString(fileID, "Dealer");
		agk::WriteInteger(fileID, dealer);

		if (gameState >= MELDING)
			agk::WriteString(fileID, "Trump");

		formatPlayerData(fileID);
		formatScores(fileID);

	}

	switch(gameState)
	{
	case MENU://save theme
	case START://save scores and theme
		break;
	case OPTIONS: //Could be from anything past this point
		doEndGameSwitch(fileID, prevState);
		break;
	case DEALING:
		formatDeckData(fileID);
		break;
	case ANIMATION: //could be part of dealing or playing
		doEndGameSwitch(fileID, prevState);
		break;
	case SORTING: //same as bidding, I think
	case BIDDING:
		formatBids(fileID);
		break;
	case MELDING:
		//formatMelds(fileID);  //don't need to do anything special
		break;
	case PLAYING:
		formatGame(fileID);
		break;
	}*/

}

void app::initLoadScreen(void)
{
    float factor = 1.0f;
    float offset = 0.0f;
    
#if (PLATFORM == MOBILE)
    {
        factor = 1.5f;
        offset = -1.0f;
    }
#endif

	Legal = Text("Copyright 2013 Steele Game Studios");
	Legal.setColor(RGBA(255, 255, 255));
	Legal.setPosition(Point(32.5f, 92.5f + offset));
	Legal.setSize(3.0f*factor);
	Legal.setVisible(true);

	LoadingScreen.setSize(100.0f); //setSize(0.09f, 0.09f);
	LoadingScreen.setDepth(0);
	LoadingScreen.setPosition(0.0f, 0.0f); //7.0f, (float)((50.0/768.0)*100));
	LoadingScreen.setVisible(true);

	//loading test screen can be changed to a proper loading screen if needed
	LoadScreen1.setSize(55.0f);
	LoadScreen1.setDepth(1);
	LoadScreen1.setPosition(0.0f,90.0f);
	LoadScreen1.setVisible(true);

	Loading.setSize(5.0f);
	Loading.setPosition(Point(26.0f, 90.0f));
	Loading.setDepth(5);
	Loading.setFrame(1);
	agk::Sync();
}

void app::initSprites(void)
{
	TextBackground = Sprite("xmas/board_background.png"); 
}

void app::initNumbers(void)
{
}

void app::finalSetup(void)
{
	//char temp[64] = {NULL};
	unsigned short int fontImNum;

	initNumbers();
	initSprites();

	fontImNum = myLoadImage("default/Corben.png");

	gameResult = Text("");
	gameResult.setFontImage(fontImNum);
	gameResult.setVisible(false);
	gameResult.setSize(4.75f);
	gameResult.setColor(RGBA(245, 245, 245));
	gameResult.setPosition(Point(50.0f, 30.0f));

	Loading.incrementFrame();
	agk::Sync();

	gameResult4 = Text("");
	gameResult4.setFontImage(fontImNum);
	gameResult4.setSize(4.75f);
	gameResult4.setColor(RGBA(245, 245, 245));
	gameResult4.setVisible(false);
	gameResult4.setPosition(Point(50.0f, 35.0f));

	Loading.incrementFrame();
	agk::Sync();

	MeldFail = Text("");
	MeldFail.setFontImage(fontImNum);
	MeldFail.setSize(4.75f);
	MeldFail.setColor(RGBA(245, 245, 245));
	MeldFail.setVisible(false);
	MeldFail.setPosition(Point(50.0f, 35.0f));
    MeldFail.setDepth(9999);
	MeldFail.setAlignment(1);

	Loading.incrementFrame();
	agk::Sync();

	MeldAmount = Text("");
	MeldAmount.setSize(4.75f);
	MeldAmount.setFontImage(fontImNum);
	MeldAmount.setColor(RGBA(245, 245, 245));
	MeldAmount.setPosition(Point(50.0f, 35.0f));
	MeldAmount.setAlignment(1);
	MeldAmount.setVisible(false);

	Loading.incrementFrame();
	agk::Sync();

	GameOver = Text();
	GameOver.setSize(4.75f);
	GameOver.setFontImage(fontImNum);
	GameOver.setColor(RGBA(245, 245, 245));
	GameOver.setPosition(Point(50.0f, 35.0f));
	GameOver.setAlignment(1);
	GameOver.setVisible(false);

	Loading.incrementFrame();
	agk::Sync();

	switch (language)
	{
	case ENGLISH:
		TapScreen = Text("Tap Screen to Continue");
		break;
	case FRENCH:
		TapScreen = Text("Appuyez sur l'écran pour continuer");
		break;
	case ITALIAN:
		TapScreen = Text("Tocco dello schermo per continuare");
		break;
	case GERMAN:
		TapScreen = Text("Tippen sie auf bildschirm, um fortzufahren");
		break;
	case SPANISH:
		TapScreen = Text("Tocar la pantalla para continuar");
		break;
	}
	TapScreen.setFontImage(fontImNum);
	TapScreen.setSize(4.75f);
	TapScreen.setColor(RGBA(245, 245, 245));
	TapScreen.setPosition(Point(50.0f, 35.0f));
	TapScreen.setAlignment(1);
	TapScreen.setVisible(false);

	Loading.incrementFrame();
	agk::Sync();

	switch (language)
	{
	case ENGLISH:
		SwipeHelp = Text("Touch card to select a card.\nSwipe up to play selected card.");
		break;
	case FRENCH:
		SwipeHelp = Text("Touchez carte pour le sélectionner.\nGlisser pour jouer la carte sélectionnée.");
		break;
	case ITALIAN:
		SwipeHelp = Text("Tocco carta per selezionarla.\nScorrere fino a giocare la carta selezionato.");
		break;
	case GERMAN:
		SwipeHelp = Text("Touch-karte, um sie auszuwählen.\nStreichen bis zur gewählten karte spielen.");
		break;
	case SPANISH:
		SwipeHelp = Text("Tarjeta de contacto para seleccionarlo.\nPasa para jugar la carta elegida.");
		break;
	}
	SwipeHelp.setFontImage(fontImNum);
	SwipeHelp.setSize(4.75f);
	SwipeHelp.setColor(RGBA(245, 245, 245));
	SwipeHelp.setPosition(Point(50.0f, 65.0f));
	SwipeHelp.setAlignment(1);
	SwipeHelp.setVisible(false);
	
	Loading.incrementFrame();
	agk::Sync();

	switch (language)
	{
	case ENGLISH:
		ChooseTrump = Text("Choose your trump suit.\nPotential suits must have a marriage.");
		break;
	case FRENCH:
		ChooseTrump = Text("Choisissez votre couler d'atout.\nCostumes potentiel doit avoir un mariage.");
		break;
	case ITALIAN:
		ChooseTrump = Text("Scegliere il seme di briscola.\nSi adatta alle potenzialità deve avere un matrimonio.");
		break;
	case GERMAN:
		ChooseTrump = Text("Wählen sie ihre trumpffarbe.\nPotenzial anzüge müssen eine ehe.");
		break;
	case SPANISH:
		ChooseTrump = Text("Elige tu palo de triunfo.\nDemandas potenciales deben tener un matrimonio.");
		break;
	}
	ChooseTrump.setFontImage(fontImNum);
	ChooseTrump.setSize(4.75f);
	ChooseTrump.setColor(RGBA(245, 245, 245));
	ChooseTrump.setPosition(Point(50.0f, 65.0f));
	ChooseTrump.setAlignment(1);
	ChooseTrump.setVisible(false);
	
	Loading.incrementFrame();
	agk::Sync();

	switch (language)
	{
	case ENGLISH:
		OptionsHelp = Text("To choose another Theme,\nswipe left or right.");
		break;
	case FRENCH:
		OptionsHelp = Text("De choisir un autre thème,\nfaites glisser à gauche ou à droite.");
		break;
	case ITALIAN:
		OptionsHelp = Text("Di scegliere un altro tema,\nscorrere a destra oa sinistra.");
		break;
	case GERMAN:
		OptionsHelp = Text("Zu wählen, ein weiteres thema,\nlinks oder rechts streichen.");
		break;
	case SPANISH:
		OptionsHelp = Text("A elegir otra tema,\ndeslizar a izquierda o derecha.");
		break;
	}
	OptionsHelp.setFontImage(fontImNum);
	OptionsHelp.setSize(4.75f);
	OptionsHelp.setColor(RGBA(245, 245, 245));
	OptionsHelp.setPosition(Point(50.0f, 65.0f));
	OptionsHelp.setAlignment(1);
	OptionsHelp.setVisible(false);
	
	Loading.incrementFrame();
	agk::Sync();

	Legal.setVisible(false);
	Legal.~Text();

#if DEMOBUILD
	DemoEndText = Text("Sorry, but your one hour has been reached.\nThe full game will be released soon,\nFind us on Facebook (Steele Studios)\nand on Twitter (@SteeleStudios)\nto get more details!");
	DemoEndText.setFontImage(fontImNum);
	DemoEndText.hide();
	DemoEndText.setSize(4.75f);
	DemoEndText.setColor(RGBA(245, 245, 245);
	DemoEndText.setPosition(Point((50.0f, 30.0f);
	DemoEndText.setAlignment(1);
	DemoEndText.setDepth(10001);
#endif

	delayStep = 0;
}

/*void app::setCardData(void)
{
	//similar setup to sprites, but need a front image
	for(int i = 0; i < 20; i++)
	{
		for (int j = i * 4; j < (i * 4) + 4; j++)
		{
			int tempImageF;
			int tempImageB;
			int tempImageO;
			char tempFileName[32];
			char tempFileNameO[64];
			char tempChar[64];

			switch (i)
			{
			case 0:
				strcpy(tempFileName, "ace_hearts.png");
				break;
			case 1:
				strcpy(tempFileName, "10_hearts.png");
				break;
			case 2:
				strcpy(tempFileName, "king_hearts.png");
				break;
			case 3:
				strcpy(tempFileName, "queen_hearts.png");
				break;
			case 4:
				strcpy(tempFileName, "jack_hearts.png");
				break;
			case 5:
				strcpy(tempFileName, "ace_clubs.png");
				break;
			case 6:
				strcpy(tempFileName, "10_clubs.png");
				break;
			case 7:
				strcpy(tempFileName, "king_clubs.png");
				break;
			case 8:
				strcpy(tempFileName, "queen_clubs.png");
				break;
			case 9:
				strcpy(tempFileName, "jack_clubs.png");
				break;
			case 10:
				strcpy(tempFileName, "ace_diamonds.png");
				break;
			case 11:
				strcpy(tempFileName, "10_diamonds.png");
				break;
			case 12:
				strcpy(tempFileName, "king_diamonds.png");
				break;
			case 13:
				strcpy(tempFileName, "queen_diamonds.png");
				break;
			case 14:
				strcpy(tempFileName, "jack_diamonds.png");
				break;
			case 15:
				strcpy(tempFileName, "ace_spades.png");
				break;
			case 16:
				strcpy(tempFileName, "10_spades.png");
				break;
			case 17:
				strcpy(tempFileName, "king_spades.png");
				break;
			case 18:
				strcpy(tempFileName, "queen_spades.png");
				break;
			case 19:
				strcpy(tempFileName, "jack_spades.png");
				break;
			}

			tempImageF = myLoadImage(fileLoc(tempFileName, tempChar));
			tempImageB = myLoadImage(fileLoc("card_back.png", tempChar));

			strncpy(tempFileNameO, tempFileName, strlen(tempFileName)-4);
			strcat(tempFileNameO, "m.png");

			tempImageO = myLoadImage(fileLoc(tempFileNameO, tempChar));

			CurrentTheme.Deck[j].setData(tempImageF, tempImageB, tempImageO, i, i/5);
			//CurrentTheme.Deck[j].setFrontImage(i+1);
			//CurrentTheme.Deck[j].setSize(-1.0f, 12.5f);
			CurrentTheme.Deck[j].setSize(0.037f);
			CurrentTheme.Deck[j].hide();

			Loading.incrementFrame();
			agk::Sync();
		}
	}
}*/

void app::setAnimatedSpriteData(void)
{
    float factor = 0.0f;
#if (PLATFORM == MOBILE)
	TrumpSymbol = AnimatedSprite(25, 25, 32, loadingImage[0], "Load Suit Rotation.png");
#else
    TrumpSymbol = AnimatedSprite(75, 75, 32, loadingImage[0], "Load Suit Rotation.png");
#endif
	TrumpSymbol.setFrame(1);
	TrumpSymbol.setDepth(5);
    
#if (PLATFORM == MOBILE)
    factor = -0.5;
#endif
    
	if (tableTopStyle == 0)
	{
		TrumpSymbol.setSize(4.25f + factor);
		TrumpSymbol.setPosition(Point(91.25f, 70.0f));
	}
	else if (tableTopStyle == 1)
	{
		TrumpSymbol.setSize(5.0f + factor);
		TrumpSymbol.setPosition(Point(91.5f, 69.0f));
	}
	else if (tableTopStyle == 2)
	{
		TrumpSymbol.setSize(5.0f + factor);
		TrumpSymbol.setPosition(Point(91.0f, 69.0f));
	}
	else if (tableTopStyle == 3)
	{
		TrumpSymbol.setSize(5.0f + factor);
		TrumpSymbol.setPosition(Point(91.0f, 72.0f));
	}
	else
	{
		TrumpSymbol.setSize(5.0f + factor);
		TrumpSymbol.setPosition(Point(90.0f, 69.0f));
	}
	TrumpSymbol.setVisible(false);

	Loading.incrementFrame();
	agk::Sync();
}

void app::setSpriteData(void)
{
	/*******************************
	*		Steps to setup a sprite!
	*		1: SetData(imageNo, spriteNo, "Sprite Name")
	*		2: setSize or setSize
	*		3: setPriority
	*		4: display
	*		5: hide
	*******************************/
	//int tempPImage = HandResults.getPImage();
	//char temp[64];

	//Step1: Set Data Field-------------------------------------------------
    unsigned int imgNum;
    
    imgNum = HandResults.Parent.getImageNumber("woodoval.png");
	NBack.setImage(imgNum);
	SBack.setImage(imgNum);
	WBack.setImage(imgNum);
	EBack.setImage(imgNum);
    
#if DEMOBUILD
	DemoEndBack.setImageNumber(imgNum);
#endif

	Loading.incrementFrame();
	agk::Sync();
	//End Step 1------------------------------------------------------

	//Begin Step 2: Scale/Stretch/Size--------------------------------
	NBack.setSize(13.5f);
	EBack.setSize(13.5f);
	SBack.setSize(13.5f);
	WBack.setSize(13.5f);
	DemoEndBack.setSize(85.0f, 35.0f);

	Loading.incrementFrame();
	agk::Sync();
	//End Step 2-------------------------------------------------------

	//Start Step 3: Set Priority---------------------------------------
	NBack.setDepth(30);  //need to be high to get over cards
	SBack.setDepth(30);
	WBack.setDepth(30);
	EBack.setDepth(30);
	DemoEndBack.setDepth(10000);

	Loading.incrementFrame();
	agk::Sync();
	//End Step 3--------------------------------------------------------

	//Start Step 4: display--------------------------------------------

	//End Step 4------------------------------------------------------------

	//Start Step 5: hide---------------------------------------------------

	//Options.hide();

	NBack.setVisible(false);
	SBack.setVisible(false);
	WBack.setVisible(false);
	EBack.setVisible(false);
	DemoEndBack.setVisible(false);

	Loading.incrementFrame();
	agk::Sync();
	//End Step 5------------------------------------------------------------

	//Optional Step 6: Setting Imprint, only for shrink close/grow open effects
	/*NBack.setImprint(43.0f,8.0f);
	SBack.setImprint(43.0f,84.5f);
	WBack.setImprint(4.3f,36.5f);
	EBack.setImprint(81.75f,36.5f);*/

	if (DEMOBUILD)
	{
		//DemoEndBack.setImprint(7.0f, 25.0f);
		DemoEndBack.setAlpha(220);


		/*while (!DemoEndBack.hasShrunk())
			DemoEndBack.updateShrinkClose(-1);*/
	}

	Loading.incrementFrame();
	agk::Sync();
	//End Step 6------------------------------------------------------------
}

void app::setNumberData(void)
{

}

char* app::fileLoc(const char fileName[], char newFilename[])
{
	strcpy(newFilename, themeDir);
	strcat(newFilename, fileName);

	if (agk::GetFileExists(newFilename))
		return newFilename;
	else
	{
		strcpy(newFilename, "default/");
		strcat(newFilename, fileName);

		return newFilename;
	}
}

short unsigned int app::myLoadImage(const char fileLoc[], short unsigned int imageNum)
{
	unsigned short int newImageNum = 0;
    short queue = 0;
    
    if (fileLoc[0] == 'd')
        queue = 1;

	if (imageNum != 0)
	{
		if (agk::GetFileExists(fileLoc))
		{
			agk::LoadImage(imageNum, fileLoc);
            
            if (fileLoc[0] != 'd')
                addToQueue(imageNum, queue);
		}

		return imageNum;
	}

    newImageNum = agk::LoadImage(fileLoc);

    if (fileLoc[0] != 'd')
        addToQueue(newImageNum, queue);
    
	return newImageNum;
}

//no checks! make sure the file exists before calling!
/*void app::loadDefaultSprites(char dir[])
{
	char temp[64];

	myLoadImage(fileLoc("sound_btn_off.png", temp));
	myLoadImage(fileLoc("sound_btn_on.png", temp));

	Loading.incrementFrame();
	agk::Sync();

	myLoadImage(fileLoc("board table.png", temp));
	myLoadImage(fileLoc("board_background.png", temp));
	myLoadImage(fileLoc("board_header.png", temp));
	myLoadImage(fileLoc("board_hand score.png", temp));
	myLoadImage(fileLoc("board_total score.png", temp));

	myLoadImage(fileLoc("Suit Rotation.png", temp));

	Loading.incrementFrame();
	agk::Sync();

	myLoadImage(fileLoc("Options_Off.png", temp));
	myLoadImage(fileLoc("Options_On.png", temp));
	myLoadImage(fileLoc("Options_Down.png", temp));

	Loading.incrementFrame();
	agk::Sync();

	myLoadImage(fileLoc("Check box.png", temp));
	myLoadImage(fileLoc("Check box with check.png", temp));

	Loading.incrementFrame();
	agk::Sync();

	myLoadImage(fileLoc("Resume_Off.png", temp));
	myLoadImage(fileLoc("Resume_On.png", temp));
	myLoadImage(fileLoc("Resume_Down.png", temp));
	myLoadImage(fileLoc("Main_Menu_Off.png", temp));
	myLoadImage(fileLoc("Main_Menu_On.png", temp));
	myLoadImage(fileLoc("Main_Menu_Down.png", temp));
	myLoadImage(fileLoc("New_Game_Off.png", temp));
	myLoadImage(fileLoc("New_Game_On.png", temp));
	myLoadImage(fileLoc("New_Game_Down.png", temp));

	Loading.incrementFrame();
	agk::Sync();

	//Text Font


	Loading.incrementFrame();
	agk::Sync();

	myLoadImage(fileLoc("background_g.jpg", temp));    //themes
	myLoadImage(fileLoc("background_r.jpg", temp));    //themes
	myLoadImage(fileLoc("background_a.jpg", temp));    //themes
	myLoadImage(fileLoc("background_f.jpg", temp));    //themes
	myLoadImage(fileLoc("background_e.jpg", temp));    //themes
	myLoadImage(fileLoc("background_w.jpg", temp));    //themes

	Loading.incrementFrame();
	agk::Sync();
}*/

void app::initDisplay(void)
{
	

}

void app::initVars()
{
	menuPressed=true;
	ingamebutton=true;

#if DEMOBUILD
	demoOver = false;
#endif
	copyOnce = false;
	visScreen = 0;
	calcOnce = false;
	soundOn = true;
	wipingCards = false;
	//count = 0;
	// = 100;
#if DEMOBUILD
	dealer = 0;
#else
#if (OS != IOS)
	dealer = agk::Random(0, 3);
#else
    dealer = arc4random() % 4;
#endif
#endif

    initQueue();
	language = ENGLISH;
	turn = (dealer+1)%4;
	//int state = 0;
	round = 0;
	foundLocation = false;
	playingAnimation = false;
	passInitialAnim = false;
	quit = false;
	menuChoice = 0;
	mouseX = 0;
	mouseY = 0;
	mouseClick = false;
	cardsRevealed = false;
	hand = 0;
	pauseGame = false;
	tableTopStyle = 0; //default
	contFlag = true;  //for animating the cards moving to whoever took the trick
	hScore = 0;
	vScore = 0;
	mouseDelay = 0;
	delayStep = 0;
	bidWinner = -1;

	Loading.incrementFrame();
	agk::Sync();

	West.setPlayerID(WEST);
	North.setPlayerID(NORTH);
	East.setPlayerID(EAST);
	South.setPlayerID(SOUTH);

	Loading.incrementFrame();
	agk::Sync();
}


/*void toggleLayout(Sprite *View)
{
	View->updateShrinkClose(-1);

	switch(View->getImageNumber())
	{
	case 30:
		View->changeImage(31);
		break;
	case 31:
		View->changeImage(30);
		break;
	case 32:
		View->changeImage(33);
		break;
	case 33:
		View->changeImage(32);
		break;
	}

	View->updateGrowOpen(100);
}*/

void toggleDebug()
{
	debug = !debug;
}

short int app::displayKnown(int turn)
{
	short int offset = 18; //using old coords
	short int count = 0;
	short int extras = 0;
	short int cardNumber = -1;
	float firstCardX;
	float firstCardY;
    float bubbleOffset = 0.0f;
	Player *Selected;

#if (PLATFORM == MOBILE)
        bubbleOffset = -5.0f;
#endif

	switch (turn)
	{
	case WEST:
		CurrentTheme.BidMeldBubbles.MeldBubble[WEST].setPosition(19.5, 7.0);
		CurrentTheme.BidMeldBubbles.MeldBubble[WEST].setVisible(true);
		CurrentTheme.BidMeldBubbles.MeldBubble[WEST].setDepth(2);
		Selected = &West;
		break;
	case NORTH:
        if (tableTopStyle == 2)
            CurrentTheme.BidMeldBubbles.MeldBubble[NORTH].setPosition(19.5f, 7.0f - (offset - 15));
        else
            CurrentTheme.BidMeldBubbles.MeldBubble[NORTH].setPosition(19.5f, 7.0f);
		CurrentTheme.BidMeldBubbles.MeldBubble[NORTH].setVisible(true);
		CurrentTheme.BidMeldBubbles.MeldBubble[NORTH].setDepth(2);
		Selected = &North;
		break;
	case EAST:
        if (tableTopStyle == 2)
            CurrentTheme.BidMeldBubbles.MeldBubble[EAST].setPosition(22.0f, 8.0f - (offset - 10));
        else
            CurrentTheme.BidMeldBubbles.MeldBubble[EAST].setPosition(22.0f, 8.0f);
		CurrentTheme.BidMeldBubbles.MeldBubble[EAST].setVisible(true);
		CurrentTheme.BidMeldBubbles.MeldBubble[EAST].setDepth(2);
		Selected = &East;
		break;
	case SOUTH:
        //if ((tableTopStyle == 0) || (tableTopStyle == 5) || (tableTopStyle == 1))
			CurrentTheme.BidMeldBubbles.MeldBubble[SOUTH].setPosition(23.0f, 14.5f + bubbleOffset);
		//else
		//	CurrentTheme.BidMeldBubbles.MeldBubble[SOUTH].setPosition(23.0f, 23.5f);
		CurrentTheme.BidMeldBubbles.MeldBubble[SOUTH].setVisible(true);
		CurrentTheme.BidMeldBubbles.MeldBubble[SOUTH].setDepth(2);
		Selected = &South;
		break;
	}

	firstCardX = CurrentTheme.BidMeldBubbles.getFirstCardPos(tableTopStyle, turn, true);
	firstCardY = CurrentTheme.BidMeldBubbles.getFirstCardPos(tableTopStyle, turn, false);

	if (Selected->getDoubleRunSuit() == trump)
	{
		for (short int rank = 0; rank < 5; rank++)
		{
			if (Selected->known[rank][trump] < 2)
				Selected->known[rank][trump] = 2;
		}
	}

	for(short int i = 0; i < 4; i++)
	{
		for (short int j = 0; j < 5; j++)
		{
//			int cardOffset;

			if (Selected->known[j][i] > 0)
			{
				//int cardImage = j + (i * 5) + 1; //get general card

				if (extras == 0)
					extras = Selected->known[j][i];

				cardNumber = Selected->getCard(Selected->getCardFromPlayerHand(j, i)+(extras-1));

				//if (cardNumber % 4 == 0)
				//	cardOffset = -1;
				//else
				//	cardOffset = 0;

				if (Selected->getID() == WEST) //card is Rotated
					CurrentTheme.Deck[cardNumber].unRotate();
				else if (Selected->getID() == EAST) //card is Rotated
					CurrentTheme.Deck[cardNumber].unRotate();

				// code to try to get the meld to bold cards

				CurrentTheme.Deck[cardNumber].turnFaceUp();

				CurrentTheme.Deck[cardNumber].display(firstCardX + (float)((100*((offset * count)/1024.0))), firstCardY);
				CurrentTheme.Deck[cardNumber].setDepth(count+3);
				extras--;
				count++;

				if (extras != 0)
					j--;
			}
		}
	}

	return count;
}

void app::updateTopLeft()
{
	//float x = WEST_LOC_X - 88;
	//float y = NORTH_LOC_Y + 58;
	short hSaveAmount = 20;
	short vSaveAmount = 20;

	if ((bidWinner == -1) || (NewTrickGame.getTrumpSuit() == -1))
	{
		CurrentTheme.H2Get.setValue(0);
		CurrentTheme.V2Get.setValue(0);
		CurrentTheme.HMeld.setValue(0);
		CurrentTheme.VMeld.setValue(0);
		CurrentTheme.HMeld.hideAll();
		CurrentTheme.VMeld.hideAll();
		CurrentTheme.H2Get.hideAll();
		CurrentTheme.V2Get.hideAll();
		return;
	}

	//dbSetTextFont("Rockwell");
	if (bidWinner % 2 == 0)
		hSaveAmount = bid - hMeld;
	else
		vSaveAmount = bid - vMeld;

	if (hSaveAmount < 20)
		hSaveAmount = 20;
	if (vSaveAmount < 20)
		vSaveAmount = 20;

	if (hTricks != -1)
		hSaveAmount -= hTricks;
	if (vTricks != -1)
		vSaveAmount -= vTricks;

	if (hSaveAmount < 0)
		hSaveAmount = 0;
	if (vSaveAmount < 0)
		vSaveAmount = 0;

	if (hMeld < 20)
		hMeld = 0;
	if (vMeld < 20)
		vMeld = 0;

	if (hMeld >= 20)
	{
		if (CurrentTheme.HMeld.getValue() != hMeld)
		{

			CurrentTheme.HMeld.incrementTo(hMeld);
			PointClick.play();
		}
	}
	else
	{
		CurrentTheme.HMeld.setValue(0);
		CurrentTheme.HMeld.showAll(true);
	}

	if (vMeld >= 20)
	{
		if (CurrentTheme.VMeld.getValue() != vMeld)
		{
			CurrentTheme.VMeld.incrementTo(vMeld);
			PointClick.play();
		}
	}
	else
	{
		CurrentTheme.VMeld.setValue(0);
		CurrentTheme.HMeld.showAll(true);
	}

	//CurrentTheme.HMeld.hideAll();
	if (CurrentTheme.H2Get.getValue() != hSaveAmount)
	{
		CurrentTheme.H2Get.incrementTo(hSaveAmount);
		PointClick.play();
	}

	if (CurrentTheme.V2Get.getValue() != vSaveAmount)
	{
		CurrentTheme.V2Get.incrementTo(vSaveAmount);
		PointClick.play();
	}
}

void app::updateTopRight(void)
{
	//CurrentTheme.BidScore.hideAll();

	/*if (bid >= 100)
	{
		CurrentTheme.BidScore.setPosition(2, 1);
		//CurrentTheme.Bid[0].setPosition(75.5, 10.0);
		CurrentTheme.BidScore.show(2, 1);
	}*/

	if (bid > 50)
	{

		if (CurrentTheme.BidScore.getValue() != bid)
		{
			CurrentTheme.BidScore.incrementTo(bid);
			PointClick.play();
		}
		//CurrentTheme.BidScore.setPosition(0, bid%10);

		//CurrentTheme.BidScore.show(1, bid%100/10);
		//CurrentTheme.BidScore.show(0, bid%10);
	}

	if (bid <= 50)
	{
		//CurrentTheme.BidScore.hideAll();
		//CurrentTheme.BidScore.setDigit(0, 0);
		//CurrentTheme.BidScore.setDigit(1, 5);
		//CurrentTheme.BidScore.setDigit(2, 0);
		CurrentTheme.BidScore.setValue(50);
		/*hide(0, CurrentTheme.BidScore.getDigit(0));
		CurrentTheme.BidScore.hide(1, CurrentTheme.BidScore.getDigit(1));
		CurrentTheme.BidScore.hide(2, CurrentTheme.BidScore.getDigit(2));*/
	}

	if (gameState >= MELDING)
	{
		if (gameState == PLAYING)
		{
			TrumpSymbol.stopSprite();
			TrumpSymbol.setFrame((8 * trump) + 1);
		}

		if (!TrumpSymbol.getVisible())
		{
			TrumpSymbol.play(5.0, true, (8 * trump) + 1, 8 * (trump + 1));
			TrumpSymbol.setVisible(true);
		}
	}
	else if (gameState != ANIMATION)
		TrumpSymbol.setVisible(false);
}

///////////////////////////////////////////////////////////////////////////////////////////
///  hScore = East-West Score
///  vScore = North-South Score
void app::updateBottomLeft(void)
{
	hScore = NewGame.getScore(BLUE);
	vScore = NewGame.getScore(RED);

	if (!CurrentTheme.HScore.getVisible())
	{
		CurrentTheme.HScore.setValue(hScore);
		CurrentTheme.HScore.showAll();
	}

	if (!CurrentTheme.VScore.getVisible())
	{
		CurrentTheme.VScore.setValue(vScore);
		CurrentTheme.VScore.showAll();
	}

	if (CurrentTheme.VScore.getValue() != vScore)
	{
		if (CurrentTheme.VScore.getValue() != vScore)
		{
			CurrentTheme.VScore.incrementTo(vScore, true);
#if (OS == DROID)
			if(CurrentTheme.VScore.getValue()%5==0)
#endif
                PointClick.play();
		}
	}
	else if (vScore == 0)
		CurrentTheme.VScore.incrementTo(0, true);

	if (CurrentTheme.HScore.getValue() != hScore)
	{
		if (CurrentTheme.HScore.getValue() != hScore)
		{
			CurrentTheme.HScore.incrementTo(hScore, true);
#if (OS == DROID)
			if(CurrentTheme.HScore.getValue()%5==0)
#endif
                PointClick.play();
		}
	}
	else if (hScore == 0)
		CurrentTheme.HScore.incrementTo(0, true);
}

const float findTargetX(short int player)
{
	if (player == WEST)
		return 100*(WEST_LOC_X/1024);
	else if (player == NORTH)
		return 100*(NORTH_LOC_X);
	else if (player == EAST)
		return 100*(EAST_LOC_X/1024);
	else if (player == SOUTH)
		return 100*(SOUTH_LOC_X/1024);
	else
		return -1;
}

const float findTargetY(short int player)
{
	if (player == WEST)
		return (float)(100*(WEST_LOC_Y/768.0));
	else if (player == NORTH)
		return (float)(100*(NORTH_LOC_Y/768.0));
	else if (player == EAST)
		return (float)(100*(EAST_LOC_Y/768.0));
	else if (player == SOUTH)
		return (float)(100*(SOUTH_LOC_Y/768.0));
	else
		return -1.0f;
}

void app::displayAllKnown(void)
{
	//char suit[9];
	//char rank[6];

/*	dbSetTextSize(20);
	dbText(x[WEST], y[NORTH], "WEST");
	dbText(x[NORTH], y[NORTH], "NORTH");
	dbText(x[EAST], y[NORTH], "EAST");
	dbText(x[SOUTH], y[NORTH], "SOUTH");
	dbLine(x[WEST], y[NORTH]+20, x[SOUTH]+90, y[NORTH]+20);
	y[WEST] += 20;
	y[NORTH] += 20;
	y[EAST] += 20;
	y[SOUTH] += 20;

	for(int i = 0; i <= SPADES; i++)
	{
		switch (i)
		{
		case HEARTS:
			strcpy(suit, "HEARTS");
			break;
		case DIAMONDS:
			strcpy(suit, "DIAMONDS");
			break;
		case SPADES:
			strcpy(suit, "SPADES");
			break;
		case CLUBS:
			strcpy(suit, "CLUBS");
		}

		for (int j = 0; j <= JACK; j++)
		{
			switch (j)
			{
			case ACE:
				strcpy(rank, "ACE");
				break;
			case TEN:
				strcpy(rank, "TEN");
				break;
			case KING:
				strcpy(rank, "KING");
				break;
			case QUEEN:
				strcpy(rank, "QUEEN");
				break;
			case JACK:
				strcpy(rank, "JACK");
				break;
			}

			if (West.known[j][i] > -1)
			{
				if (West.newKnown[j][i] != -1)
					dbInk(dbRGB(255, 0, 0), dbRGB(0, 0, 0));
				else
					dbInk(dbRGB(0, 0, 0), dbRGB(0, 0, 0));

				dbSetCursor(x[WEST], y[WEST]);
				dbPrint((double) West.known[j][i]);
				dbText(x[WEST]+10, y[WEST], rank);
				dbText(x[WEST]+70, y[WEST], "of");
				dbText(x[WEST]+90, y[WEST], suit);
				y[WEST] += 20;
			}

			if (North.known[j][i] > -1)
			{
				if (North.newKnown[j][i] > -1)
					dbInk(dbRGB(255, 0, 0), dbRGB(0, 0, 0));
				else
					dbInk(dbRGB(0, 0, 0), dbRGB(0, 0, 0));
				dbSetCursor(x[NORTH], y[NORTH]);
				dbPrint((double) North.known[j][i]);
				dbText(x[NORTH]+10, y[NORTH], rank);
				dbText(x[NORTH]+70, y[NORTH], "of");
				dbText(x[NORTH]+90, y[NORTH], suit);
				y[NORTH] += 20;
			}

			if (East.known[j][i] > -1)
			{
				if (East.newKnown[j][i] > -1)
					dbInk(dbRGB(255, 0, 0), dbRGB(0, 0, 0));
				else
					dbInk(dbRGB(0, 0, 0), dbRGB(0, 0, 0));
				dbSetCursor(x[EAST], y[EAST]);
				dbPrint((double) East.known[j][i]);
				dbText(x[EAST]+10, y[EAST], rank);
				dbText(x[EAST]+70, y[EAST], "of");
				dbText(x[EAST]+90, y[EAST], suit);
				y[EAST] += 20;
			}

			if (South.known[j][i] > -1)
			{
				if (South.newKnown[j][i] > -1)
					dbInk(dbRGB(255, 0, 0), dbRGB(0, 0, 0));
				else
					dbInk(dbRGB(0, 0, 0), dbRGB(0, 0, 0));
				dbSetCursor(x[SOUTH], y[SOUTH]);
				dbPrint((double) South.known[j][i]);
				dbText(x[SOUTH]+10, y[SOUTH], rank);
				dbText(x[SOUTH]+70, y[SOUTH], "of");
				dbText(x[SOUTH]+90, y[SOUTH], suit);
				y[SOUTH] += 20;
			}

			if ((j == JACK) && (i == SPADES))
			{
				dbInk(dbRGB(255, 0, 0), dbRGB(255, 0, 0));
				dbLine(x[WEST], y[WEST], x[WEST]+110, y[WEST]);
				dbLine(x[NORTH], y[NORTH], x[NORTH]+110, y[NORTH]);
				dbLine(x[EAST], y[EAST], x[EAST]+110, y[EAST]);
				dbLine(x[SOUTH], y[SOUTH], x[SOUTH]+110, y[SOUTH]);
				for (int k = HEARTS; k <= SPADES; k++)
				{
					if (West.trumpingSuit[k])
					{
						if (k == HEARTS)
							dbText(x[WEST], y[WEST], "HEARTS");
						else if (k == CLUBS)
							dbText(x[WEST], y[WEST], "CLUBS");
						else if (k == DIAMONDS)
							dbText(x[WEST], y[WEST], "DIAMONDS");
						else if (k == SPADES)
							dbText(x[WEST], y[WEST], "SPADES");

						y[WEST] += 20;
					}

					if (South.trumpingSuit[k])
					{
						if (k == HEARTS)
							dbText(x[SOUTH], y[SOUTH], "HEARTS");
						else if (k == CLUBS)
							dbText(x[SOUTH], y[SOUTH], "CLUBS");
						else if (k == DIAMONDS)
							dbText(x[SOUTH], y[SOUTH], "DIAMONDS");
						else if (k == SPADES)
							dbText(x[SOUTH], y[SOUTH], "SPADES");

						y[SOUTH] += 20;
					}

					if (East.trumpingSuit[k])
					{
						if (k == HEARTS)
							dbText(x[EAST], y[EAST], "HEARTS");
						else if (k == CLUBS)
							dbText(x[EAST], y[EAST], "CLUBS");
						else if (k == DIAMONDS)
							dbText(x[EAST], y[EAST], "DIAMONDS");
						else if (k == SPADES)
							dbText(x[EAST], y[EAST], "SPADES");

						y[EAST] += 20;
					}

					if (North.trumpingSuit[k])
					{
						if (k == HEARTS)
							dbText(x[NORTH], y[NORTH], "HEARTS");
						else if (k == CLUBS)
							dbText(x[NORTH], y[NORTH], "CLUBS");
						else if (k == DIAMONDS)
							dbText(x[NORTH], y[NORTH], "DIAMONDS");
						else if (k == SPADES)
							dbText(x[NORTH], y[NORTH], "SPADES");

						y[NORTH] += 20;
					}
				}
			}
		}
	}*/
}

void app::displayPlayerControlBubble(void)
{
    short int tempBid = bid + 1;
    float offset = 0.0f;
    
#if (PLATFORM == MOBILE)
        offset = -2.0f;
#endif

	if (tableTopStyle != 4)
		CurrentTheme.BCBubble.display(27.5f, 32.0f + offset);
	else
		CurrentTheme.BCBubble.display(27.5f, 23.5f + offset);

	if (tableTopStyle == 0)
	{
		for (short int i = 0; i < 10; i++)
			CurrentTheme.BCBubble.BidValue.setLocation(i, 54.0f - (6.5f * i), 44.5f, 0); //2, 0
	}
    else if (tableTopStyle == 1)
    {
        for (short i = 0; i < 10; i++)
            CurrentTheme.BCBubble.BidValue.setLocation(i, 54.0f - (6.5f * i), 49.5, 0);
    }
	else if (tableTopStyle == 2)
	{
		for (short int i = 0; i < 10; i++)
			CurrentTheme.BCBubble.BidValue.setLocation(i, 54.0f - (6.5f * i), 49.5f, 0); //2, 0
	}
	else if (tableTopStyle == 3)
	{
		for (short i = 0; i < 10; i++)
			CurrentTheme.BCBubble.BidValue.setLocation(i, 53.5f - (6.5f * i), 45.0f, 0);
	}
	else if (tableTopStyle == 4)
	{
		for (short int i = 0; i < 10; i++)
			CurrentTheme.BCBubble.BidValue.setLocation(i, 53.0f - (5.5f * i), 46.0f, 0); //2, 0
	}

	CurrentTheme.BCBubble.BidValue.setValue(bid+1);

	if (CurrentTheme.BCBubble.BidValue.getValue() > 99)
		CurrentTheme.BCBubble.BidValue.display(2, 1);
	else
		CurrentTheme.BCBubble.BidValue.display(2, 0);

	CurrentTheme.BCBubble.BidValue.display(1, tempBid%100/10);
	CurrentTheme.BCBubble.BidValue.display(0, tempBid%10);

	CurrentTheme.BCBubble.show();
}

bool app::southBid()
{
//////////////////////////////////////////////////////////////////////////////////////////
//Start of MeldBid controls!
//////////////////////////////////////////////////////////////////////////////////////////
	buttonDelay = 0.5f;
#if (IOS == OS)
    #if (PLATFORM == TABLET) // +/- for Red/Ancient , +/- for Green,  , Bid for Red/Ancient , Bid for Green, Pass for Red/Ancient , Pass for Green
    const float yMinValue[6] = { 48.25f, 39.3f, 38.25f, 33.0f, 62.0f, 57.25f };
    const float yMaxValue[6] = { 62.0f, 59.5f, 48.25f, 42.25f, 71.5f, 69.25f };
    #elif (PLATFORM == MOBILE)
    const float yMinValue[6] = { 50.0f, 40.0f, 35.0f, 30.0f, 65.0f, 60.0f };
    const float yMaxValue[6] = { 65.0f, 60.0f, 50.0f, 45.0f, 80.0f, 80.0f };
    #endif
#elif (OS == DROID)
	const float yMinValue[6] = { 50.0f, 40.0f, 35.0f, 30.0f, 65.0f, 60.0f };
    const float yMaxValue[6] = { 65.0f, 60.0f, 50.0f, 45.0f, 80.0f, 80.0f };
#elif (OS == WINDOWS)
    const float yMinValue[6] = { 48.25f, 39.3f, 38.25f, 33.0f, 62.0f, 57.25f };
    const float yMaxValue[6] = { 62.0f, 59.5f, 48.25f, 42.25f, 71.5f, 69.25f };
#endif

	if (NewBidGame.getPass(SOUTH))
		return true;

	if (!CurrentTheme.BCBubble.isVisible())
		displayPlayerControlBubble();

	if (VisualBid == 255)
	{
		if (bid == 49)
            VisualBid = 51;
		else if (bid <60)
			VisualBid = bid + 1;
		else
			VisualBid = bid + 5;
	}

	if (CurrentTheme.BCBubble.BidValue.getValue() != VisualBid)
	{
		PointClick.play();
		CurrentTheme.BCBubble.BidValue.incrementTo(VisualBid);
	}

	if (mouseClick)
	{
		//BidPlus
		if ((mouseX >= (60.9f )) && (mouseX <= (71.8f )))
		{
			if ((tableTopStyle != 0) && (tableTopStyle != 5) && (tableTopStyle != 3))
			{
				if ((mouseY >= (yMinValue[0] )) && (mouseY <= (yMaxValue[0] )))
					menuChoice = 4;
			}
			else
			{
				if ((mouseY >= (yMinValue[1])) && (mouseY <= (yMaxValue[1])))
					menuChoice = 4;
			}
		}

		//BidMinus
		if ((mouseX >= (28.1f )) && (mouseX <= (40.0f )))
		{
			if ((tableTopStyle != 0) && (tableTopStyle != 5) && (tableTopStyle != 3))
			{
				if ((mouseY >= (yMinValue[0])) && (mouseY <= (yMaxValue[0])))
					menuChoice = 2;
			}
			else
			{
				if ((mouseY >= (yMinValue[1])) && (mouseY <= (yMaxValue[1])))
					menuChoice = 2;
			}
		}

		//Bid
		if ((mouseX >= (39.25f )) && (mouseX <= (60.75f )))
		{
			if ((tableTopStyle != 0) && (tableTopStyle != 5) && (tableTopStyle != 3))
			{
				if (tableTopStyle != 3)
				{
					if ((mouseY >= (yMinValue[2])) && (mouseY <= (yMaxValue[2])))
						menuChoice = 1;
				}
				else
				{
					if ((mouseY >= (27.0f )) && (mouseY <= (38.5f )))
						menuChoice = 1;
				}
			}
			else
			{
				if ((mouseY >= (yMinValue[3])) && (mouseY <= (yMaxValue[3])))
					menuChoice = 1;
			}
		}

		//BidPass
		if ((mouseX >= (39.25f )) && (mouseX <= (60.75f )))
		{
			if ((tableTopStyle != 0) && (tableTopStyle != 5) && (tableTopStyle != 3))
			{
				if ((mouseY >= (yMinValue[4])) && (mouseY <= (yMaxValue[4])))
					menuChoice = 3;
			}
			else
			{
				if ((mouseY >= (yMinValue[5])) && (mouseY <= (yMaxValue[5])))
					menuChoice = 3;
			}
		}

		buttonDelay = 1.0f;
	}

	if (menuChoice)
	{
		mouseClick = false;

		if ((menuChoice == 4) && (VisualBid == 195))
		{
			delayStep = 0;
			menuChoice = 0;
			return false;
		}

		if ((menuChoice == 2) && (VisualBid == bid + 1))
		{
			delayStep = 0;
			menuChoice = 0;
			return false;
		}

		if(CurrentTheme.BCBubble.animation(menuChoice, delayStep))
		{
			if(menuChoice == 4) //BidPlus
			{
				//Increase bid by 1 if < 60, and by 5 if >= 60
				if (VisualBid < 60)
					VisualBid++;

				else if (VisualBid >= 60)
					VisualBid+=5;

				delayStep = 0;
				menuChoice = 0;
				return false;
			}
			else if (menuChoice == 2) //BidMinus
			{
				//Decrease bid by 1 if < 60, and by 5 if >= 60
				//buttonAnimation(&BidMinusDown);

				if ((VisualBid <= 60) && (VisualBid > bid + 1))
					VisualBid--;
				else if ((VisualBid > 60) && (VisualBid > bid + 5))
					VisualBid-=5;

				delayStep = 0;
				menuChoice = 0;
				return false;
			}
			else if (menuChoice == 1) //Bid
			{
				//Pass the final bid
				if (count < 4)
					NewBidGame.playerMeldBid(&South, &North, VisualBid);
				else
					NewBidGame.playerTakeBid(&South, &North, count/4, VisualBid);

				CurrentTheme.BCBubble.BidValue.hideAll();

				VisualBid = 255;
				delayStep = 0;
				menuChoice = 0;

				return true;
			}
			else if (menuChoice == 3) //BidPass
			{
				//Pass on your turn

				if (count < 4)
					NewBidGame.playerMeldBid(&South, &North, 0);
				else
					NewBidGame.playerTakeBid(&South, &North, count/4, 0);

				for (short int i = 0; i < 3; i++)
					CurrentTheme.BCBubble.BidValue.hide(i, CurrentTheme.BCBubble.BidValue.getDigit(i));

				VisualBid = 255;
				delayStep = 0;
				menuChoice = 0;

				return true;
			}
		}
	}
	return false;
}

void app::southTrumpSelection()
{
//////////////////////////////////////////////////////////////////////////////////////////
//Start of TrumpSelection
//////////////////////////////////////////////////////////////////////////////////////////
	CurrentTheme.TSBubble.show();

	short int checkClubs = South.getNumOfMarriages(CLUBS);
	short int checkDiamonds = South.getNumOfMarriages(DIAMONDS);
	short int checkHearts = South.getNumOfMarriages(HEARTS);
	short int checkSpades = South.getNumOfMarriages(SPADES);
    
#if (OS == IOS)
    #if (PLATFORM == TABLET) // Hearts/Spades , Clubs/Diamonds
    const float yMinValue[2] = { 33.5f, 49.5f };
    const float yMaxValue[2] = { 49.5f, 66.0f };
    #elif (PLATFORM == MOBILE)
    const float yMinValue[2] = { 35.0f, 55.0f};
    const float yMaxValue[2] = { 55.0f, 80.0f};
    #endif
#elif (OS == DROID)
	const float yMinValue[2] = { 35.0f, 55.0f};
    const float yMaxValue[2] = { 55.0f, 80.0f};
#elif (OS == WINDOWS)
	const float yMinValue[2] = { 33.5f, 49.5f };
    const float yMaxValue[2] = { 49.5f, 66.0f };
#endif

	if (checkClubs == 0)
	{
		CurrentTheme.TSBubble.Clubs.ButtonUp.setVisible(false);
		CurrentTheme.TSBubble.Clubs.ButtonGray.setVisible(true);
	}

	if (checkDiamonds == 0)
	{
		CurrentTheme.TSBubble.Diamonds.ButtonUp.setVisible(false);
		CurrentTheme.TSBubble.Diamonds.ButtonGray.setVisible(true);
	}

	if (checkHearts == 0)
	{
		CurrentTheme.TSBubble.Hearts.ButtonGray.setVisible(true);
		CurrentTheme.TSBubble.Hearts.ButtonUp.setVisible(false);
	}

	if (checkSpades == 0)
	{
		CurrentTheme.TSBubble.Spades.ButtonGray.setVisible(true);
		CurrentTheme.TSBubble.Spades.ButtonUp.setVisible(false);
	}

	if (mouseClick)
	{
		//Club location
		if ((mouseX >= (28.61f )) && (mouseX <= (47.5f )))
		{
				if ((mouseY >= (yMinValue[1])) && (mouseY <= (yMaxValue[1])))
					menuChoice = 2;
		}

		//Diamond location
		if ((mouseX > (47.5f )) && (mouseX <= (67.5f )))
		{
				if ((mouseY >= (yMinValue[1])) && (mouseY <= (yMaxValue[1])))
					menuChoice = 3;
		}

		//Heart location
		if ((mouseX >= (28.61f )) && (mouseX <= (47.5f )))
		{
				if ((mouseY >= (yMinValue[0])) && (mouseY < (yMaxValue[0])))
					menuChoice = 1;
		}

		//Spade location
		if ((mouseX > (47.5f )) && (mouseX <= (67.5f )))
		{
				if ((mouseY >= (yMinValue[0])) && (mouseY < (yMaxValue[0])))
					menuChoice = 4;
		}

	}

	if (CurrentTheme.TSBubble.animation(menuChoice, delayStep))
	{
		if(menuChoice == 2) //Club selection
		{
			if(checkClubs != 0)
			{
				NewTrickGame.setTrumpSuit(CLUBS);
				menuChoice = 0;
			}
		}
		else if (menuChoice == 3) //Diamond selection
		{
			if (checkDiamonds != 0)
			{
				NewTrickGame.setTrumpSuit(DIAMONDS);
				menuChoice = 0;
			}
		}
		else if (menuChoice == 1) //Heart location
		{
			if (checkHearts != 0)
			{
				NewTrickGame.setTrumpSuit(HEARTS);
				menuChoice = 0;
			}
		}
		else if (menuChoice == 4) //Spade location
		{
			if (checkSpades != 0)
			{
				NewTrickGame.setTrumpSuit(SPADES);
				menuChoice = 0;
			}
		}
	}
}

void app::resetHand(void)
{
	//TrumpSymbol.~AnimatedSprite();
	NewBidGame.reset();
	NewTrickGame.reset();
	ShuffledDeck.reset(DEMOBUILD);
	NewGame.resetHand();
	West.reset();
	North.reset();
	East.reset();
	South.reset();
	HandResults.reset();
	HandResults.hide();

	CurrentTheme.BidScore.hide(2, CurrentTheme.BidScore.getDigit(2));
	CurrentTheme.BidScore.hide(1, CurrentTheme.BidScore.getDigit(1));
	CurrentTheme.BidScore.hide(0, CurrentTheme.BidScore.getDigit(0));

	hScore = NewGame.getScore(0);
	vScore = NewGame.getScore(1);
	dealer = (dealer + 1) % 4;
	hand++;

	//SBack.setImprint(44.0f,84.5f);
	SBack.setVisible(false);

	//WBack.setImprint(4.5f,40.0f);
	WBack.setVisible(false);

	//EBack.setImprint(81.7f,40.0f);
	EBack.setVisible(false);

	NBack.setVisible(false);
}

void app::inGameMenuButton(void)
{
	if(ingamebutton)
	{
		if (mouseClick)
		{
			if ((mouseX >= (74.4f )) && (mouseX <= (98.0f )))
			{
					if ((mouseY >= 0.0f) && (mouseY <= (15.59f )))
						menuChoice = -1;
			}
		}

		if(menuChoice == -1)
		{
			if (CurrentTheme.InGameMenuButton.animation(delayStep))
			{
				CurrentTheme.hide();

				if (NewGame.getState() == MELDING)
					CurrentTheme.BidMeldBubbles.hide();

				NBack.setVisible(false);
				EBack.setVisible(false);
				WBack.setVisible(false);
				SBack.setVisible(false);

				gameResult.setVisible(false);
				gameResult4.setVisible(false);

				hideAllText();

				delayStep = 0;
				menuChoice = 0;
				
				if (CurrentTheme.BidScore.isVisible())
					CurrentTheme.BidScore.hideAll();
                
                //Options.setup(&Loading);

				NewGame.toggleXState(OPTIONS);

				for (float i = Options.Background.getWidth(); i < 100.0f; Options.Background.setSize(++i))
					agk::Sync();
			}
		}
	}
}

void app::hideAllText(void)
{
	ChooseTrump.setVisible(false);
	DemoEndText.setVisible(false);
    gameResult.setVisible(false);
    gameResult4.setVisible(false);
    MeldFail.setVisible(false);
    MeldAmount.setVisible(false);
	OptionsHelp.setVisible(false);
    Legal.setVisible(false);
	SwipeHelp.setVisible(false);
    TapScreen.setVisible(false);
    GameOver.setVisible(false);
	TextBackground.setVisible(false);
}

/*void app::growBackground()
{
	if (!Options.Background.hasShrunk())
	{
		Options.Background.updateShrinkClose(-1);
		return;
	}

	if (CurrentTheme.Background.isGrowing())
	{
		CurrentTheme.Background.updateGrowOpen(100.0f);
		return;
	}
	else if (!CurrentTheme.Background.hasGrown())
	{
		CurrentTheme.Background.updateGrowOpen(100.0f);
		CurrentTheme.Background.setDepth(1);
		return;
	}

	NewGame.toggleXState(GROWBACKGROUND);
	NewGame.toggleXState(NewGame.getPrevState());

	//CurrentTheme.Bid.setValue(bid);
	CurrentTheme.show(NewGame.getState());

	//if (NewGame.getState() == MELDING)
	//	CurrentTheme.BidMeldBubbles.show(turn);
}*/

void app::showAllCards(void)
{
	for (int i = 0; i < 80; i++)
		CurrentTheme.Deck[i].turnFaceUp();
}

/*void app::findValueForDebugging(float value)
{
    int tempValue;
    
    tempValue = value;
}*/

void addToQueue(unsigned int imageNumber, short queue)
{
    if (queue == 0)
        themeQueue[themeQueuePointer++] = imageNumber;
    /*else
        menuQueue[menuQueuePointer++] = imageNumber;*/
}

void app::clearQueue(AnimatedSprite *Symbol, short queue)
{
    if (queue == 0)
    {
        for (int i = themeQueuePointer - 1; i > 0; i--)
        {
            agk::DeleteImage(themeQueue[i]);
        
            if (i % 5 == 0)
            {
                Symbol->incrementFrame();
                agk::Sync();
            }
        }
        
        themeQueuePointer = 0;
    }
    /*else
    {
        for (int i = menuQueuePointer - 1; i > 0; i--)
        {
            agk::DeleteImage(menuQueue[i]);
            
            if (i % 5 == 0)
            {
                Symbol->incrementFrame();
                agk::Sync();
            }
        }
        
        menuQueuePointer = 0;
    }*/
}

void initQueue(void)
{
    themeQueuePointer = 0;
    menuQueuePointer = 0;
}