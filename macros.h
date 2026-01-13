#define skip (10)
#define reverse (11)
#define drawTwo (12)
#define wildCard (13)
#define wildDrawFour (14)
#define notWild false
#define wild true
#define notSpecial false
#define special true
#define played true
#define notPlayed false
#define powerOn true
#define powerOff false
//Uno specific
#define RED 'R'
#define GREEN 'G'
#define YELLOW 'Y'
#define BLUE 'B'

#define RED_TEXT     "\033[31m"
#define GREEN_TEXT   "\033[32m"
//32
#define YELLOW_TEXT  "\033[33m"
//33
#define BLUE_TEXT    "\033[34m"
#define PURPLE_TEXT "\033[35m"
#define ORANGE_TEXT "\033[38;5;208m"

//Traditional playing card Specific
#define SPADE 'S'
#define CLUB 'C' 
#define DIAMOND 'D'
#define HEART 'H'

//already defined red text
#define GREY_TEXT   "\033[37m"
#define WHITE_TEXT   "\033[97m"
#define BLACK_TEXT   "\033[90m"

//Icons 
#define SPADE_ICON '2660'
#define CLUB_ICON '2665' 
#define DIAMOND_ICON '2666'
#define HEART_ICON '2663'

#define RESET_TEXT   "\033[0m"
#define CLEAR_SCREEN "\033[2J\033[1;1H"
#define CLEAR_LINE "\r\033[2K"
#define UNO (1)
#define WILD_COLOR 'W'
#define START 'S'
#define TERMINATE 'X'
#define userPlayer false
#define botPlayer true
#define DEV_MODE 'D'
#define BOT_MODE_CODE
bool devMode = false;
bool botMode = false;
#define HUMAN (0)
#define STARTING_HAND_SIZE (7)
#define DELAY (1s)