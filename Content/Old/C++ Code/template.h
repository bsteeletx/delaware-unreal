#ifndef _H_APP
#define _H_APP

// Link to AGK libraries
#include "Theme.h"
#include "GameScore.h"
#include "HandScore.h"
#include "Text.h"
#include "Player.h"
#include "Sound.h"
#include "MyOptions.h"
#include "MyMenu.h"
#include "Numbers.h"
#include "Sprite.h"
#include "Defines.h"
#include <string>
#include "agk.h"
#include "Deck.h"
#include "ParentImage.h"

// Global values for the app
class app
{
public:

    // constructor
    app() { memset ( this, 0, sizeof(app)); }

    // main app functions
    void Begin( void );
    void Loop( void );
    void End( void );

    // main vars
    unsigned int m_DeviceWidth;
    unsigned int m_DeviceHeight;

	bool allIn;
	float aspect;

    short unsigned int bid;
    short int bidWinner;
    float buttonDelay;

    bool calcOnce;
    short int cardPlayed[4];
    short int cards[4];
    bool cardsRevealed;
    bool contFlag;  //for animating the cards moving to whoever took the trick
    bool copyOnce;
    short int count;

    short int dealer;
    float dealerOffset[2];
    short int delayStep;

#if DEMOBUILD
    bool demoOver;
#endif

    bool foundLocation;

    short gameState;
    float gameTimer;

    short int hand;
    short hMeld;
    short int hScore;
    short int hTricks;

	unsigned short loadingImage[4];
    bool incrementing;

	short language;
	unsigned int loopCount;

    short int menuChoice;
    float mouseX;
    float mouseY;
    bool mouseClick;
    float mouseDelay;

	bool passInitialAnim;
    bool pauseGame;
    bool playingAnimation;
    bool playOneTime;

    bool quit;

    short int round;

    bool soundOn;

    short int tableTopStyle; //default
    char themeDir[32];
    short trump;
    short int turn;

	float update;

    short int visScreen;
    unsigned short int VisualBid;
    short vMeld;
    short int vTricks;
    short int vScore;

    bool wipingCards;

    bool hold;
    bool swipe;
    float ylower;
    float yupper;
	float xstartpos;
    float ystartpos;

    bool ingamebutton;
    bool menuPressed;

    //Objects
    AnimatedSprite Loading;
    AnimatedSprite TrumpSymbol;

    BidGame NewBidGame;

    Deck ShuffledDeck;

    GameScore GameResults;

    HandScore HandResults;

    MyMenu MainMenu;
    MyOptions Options;

    PinochleGame NewGame;
    Player West;
    Player North;
    Player East;
    Player South;

    Sound Cards1;
    Sound Cards2;
    Sound Cards3;
    Sound Cards4;
    Sound Cards5;
    Sound Cards6;
    Sound Cards7;
    Sound Cards8;
    Sound Cards9;
    Sound Cards10;
    Sound PointClick;
    Sound Trump[5];
    Sprite DemoEndBack;
    Sprite EBack;
    Sprite LoadingScreen;
    Sprite LoadScreen1;
    Sprite NBack;
    Sprite SBack;
    Sprite SteeleLogo;
	Sprite TextBackground;
	Sprite ThemeOptions;
    Sprite WBack;

    Text ChooseTrump;
	Text DemoEndText;
	Text GameOver;
    Text gameResult;
    Text gameResult4;
    Text MeldFail;
    Text MeldAmount;
	Text OptionsHelp;
    Text Legal;
	Text SwipeHelp;
    Text TapScreen;
    Theme CurrentTheme;
    TrickGame NewTrickGame;

private:

    void buttonAnimation(Sprite *buttonDown);

    bool changeTheme(short int style, bool init=false);

    void displayAllKnown(void);
    short int displayKnown(int turn);
    void displayPlayerControlBubble(void);
    void doEndGameSwitch(short unsigned int fileID, short int gameState);

    void fadeSteeleLogo(void);
    char* fileLoc(const char fileName[], char newFilename[]);
    void finalSetup(void);
    void findValueForDebugging(float value);
    void formatBids(short unsigned int fileID);
    void formatDeckData(short unsigned int fileID);
    void formatGame(short unsigned int fileID);
    void formatMelds(short unsigned int fileID);
    void formatPlayerData(short unsigned int fileID);
    void formatScores(short unsigned int fileID);

    void getThemeDir(short int style, char buffer[]);
    //void growBackground();

    void handleMenu(void);
    void handleInGameMenu();
    void handleLoading(void);
    void handleOptions(bool init=false);
    void handleInGameOptions(void);
    void handleStart(void);
    void handleDealing(void);
    void handleAnimation(void);
    void handleScoreboard(void);
    void handleSorting(void);
    void handleBidding(void);
    void handleMelding(void);
    void handlePlaying(void);
	void hideAllText(void);

    void inGameMenuButton(void);
    void initVars(void);
    void initDisplay(void);
    void initLoadScreen(void);
    void initNumbers(void);
    void initSprites(void);
    void initSteeleLogo(void);

    //void loadDefaultSprites(char dir[]);
    void loadLoadScreen(void);
    void loadSteeleLogo(void);

    short unsigned int myLoadImage(const char fileLoc[], short unsigned int imageNum=0);

    void parseFile(void);
    std::string parsePlayerData(short unsigned int fileID, Player *Selected);

    void reDeal(void);
    void resetGame(bool startup=false);
    void resetHand(void);

    void setAnimatedSpriteData(void);
    void setNumberData(void);
    void setSoundData(void);
    void setSpriteData(void);
    void setThemeOptionImage(char buffer[]);
	void showAllCards(void);
    bool southBid();
    void southTrumpSelection();

    void testScale(void);
    void toggleDebug();
    void toggleLayout(Sprite *);

    void unDisplayKnown(void);
    void updateBottomLeft(void);
    //void updateBottomLeft(int hScore, int vScore, AnimatedSprite *Horizontal, AnimatedSprite *Vertical);
    void updateMenu(void);
    //void updateMenu(Sprite *MenuButtons);
    void updateScore(void);
    void updateTopLeft(void);
    void updateTopRight(void);

    void writeDeckData(int fileID);
    void writePlayerData(short unsigned int fileID, Player Selected);

    void clearQueue(AnimatedSprite *Symbol, short queue);
};

extern app App;

#endif

// Allow us to use the LoadImage function name
#ifdef LoadImage
 #undef LoadImage
#endif

//helper functions
void addToQueue(unsigned int imageNumber, short queue);
void initQueue(void);
