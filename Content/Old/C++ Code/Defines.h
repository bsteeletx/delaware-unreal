#define TABLET 0
#define MOBILE 1
#define PC 2

#define IOS 0
#define DROID 1
#define WINDOWS 2

#define PLATFORM PC
#define OS WINDOWS

#define DEMOBUILD 0

#if ((PLATFORM == MOBILE) && (OS == IOS)) //iPhone and iPod Touch
#define WEST_LOC_X  6.83f //6.6f for ratio 1.3333
#define WEST_LOC_Y  16.35f //16.35 for ratio 1.333
#define NORTH_LOC_X  66.9f //66.3 for ratio 1.333
#define NORTH_LOC_Y  4.25f //4.75 for ratio 1.333
#define EAST_LOC_X  84.0f //83.9 for ratio 1.333
#define EAST_LOC_Y  43.8f //42.25 for ratio 1/333
#define SOUTH_LOC_X  3.95f //3.95 for ratio 1.333
#define SOUTH_LOC_Y  79.5f //79.75 for ratio 1.333
#define CARDWIDTHN 2.06f //2.0 for ratio 1.333
#define CARDWIDTHH 1.00f //1.05 for ratio 1.333
#define CARDWIDTHS 4.38f //4.35 for ratio 1.333
#define CARDSIZE 8.65f //9.0f for ratio 1.333
#define ASPECT 1.5f
#elif ((PLATFORM == TABLET) && (OS == IOS)) //iPad
#define WEST_LOC_X  6.6f
#define WEST_LOC_Y  16.35f
#define NORTH_LOC_X  66.3f
#define NORTH_LOC_Y  4.75f
#define EAST_LOC_X  83.9f
#define EAST_LOC_Y  45.0f
#define SOUTH_LOC_X  3.95f
#define SOUTH_LOC_Y  79.75f
#define CARDWIDTHN 2.0f //2.0 for ratio 1.333
#define CARDWIDTHH 1.05f //1.05 for ratio 1.333
#define CARDWIDTHS 4.35f //4.35 for ratio 1.333
#define CARDSIZE 9.0f //9.0f for ratio 1.333
#define ASPECT 1.333f;
#elif (OS == DROID)
#define WEST_LOC_X  6.83f //6.6f for ratio 1.3333
#define WEST_LOC_Y  16.35f //16.35 for ratio 1.333
#define NORTH_LOC_X  66.9f //66.3 for ratio 1.333
#define NORTH_LOC_Y  4.25f //4.75 for ratio 1.333
#define EAST_LOC_X  84.0f //83.9 for ratio 1.333
#define EAST_LOC_Y  43.8f //42.25 for ratio 1/333
#define SOUTH_LOC_X  3.95f //3.95 for ratio 1.333
#define SOUTH_LOC_Y  79.5f //79.75 for ratio 1.333
#define CARDWIDTHN 2.06f //2.0 for ratio 1.333
#define CARDWIDTHH 1.00f //1.05 for ratio 1.333
#define CARDWIDTHS 4.38f //4.35 for ratio 1.333
#define CARDSIZE 8.65f //9.0f for ratio 1.333
#define ASPECT 1.7f
#elif (OS == WINDOWS)
#define WEST_LOC_X  6.83f //6.6f for ratio 1.3333
#define WEST_LOC_Y  16.35f //16.35 for ratio 1.333
#define NORTH_LOC_X  66.9f //66.3 for ratio 1.333
#define NORTH_LOC_Y  4.25f //4.75 for ratio 1.333
#define EAST_LOC_X  84.0f //83.9 for ratio 1.333
#define EAST_LOC_Y  43.8f //42.25 for ratio 1/333
#define SOUTH_LOC_X  3.95f //3.95 for ratio 1.333
#define SOUTH_LOC_Y  79.5f //79.75 for ratio 1.333
#define CARDWIDTHN 2.06f //2.0 for ratio 1.333
#define CARDWIDTHH 1.00f //1.05 for ratio 1.333
#define CARDWIDTHS 4.38f //4.35 for ratio 1.333
#define CARDSIZE 8.65f //9.0f for ratio 1.333
#define ASPECT 1.333f
#endif

#define CENTER_X 50.0f
#define CENTER_Y 50.0f

#define NUM_TABLE_TOP 4

#define WEST  0
#define NORTH  1
#define EAST  2
#define SOUTH  3

#define HEARTS  0
#define CLUBS  1
#define DIAMONDS  2
#define SPADES  3

#define ACE  0
#define TEN  1
#define KING  2
#define QUEEN  3
#define JACK  4

#define SECONDS 1000

#define LOADING  0
#define MENU  1
#define OPTIONS  2
#define INGAMEMENU  3
#define INGAMEOPTIONS  4
#define GROWBACKGROUND  5
#define START  6
#define DEALING  7
#define ANIMATION  8
#define SORTING  9
#define BIDDING  10
#define MELDING  11
#define PLAYING  12
#define SCOREBOARD  13

#define ENGLISH 0
#define FRENCH 1
#define ITALIAN 2
#define GERMAN 3
#define SPANISH 4

#define BLUE 0
#define RED 1

#define GREEN 0
#define ANCIENT 2
#define FUTURE 5
#define ELECTRIC 4
#define NOEL 3

#define WEST 0
#define NORTH 1
#define EAST 2
#define SOUTH 3
