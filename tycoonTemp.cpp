#pragma execution_character_set("utf-8")

#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <algorithm>
#include "macros.h"
#include <chrono>
#include <thread>
#include <windows.h>
#include <conio.h>

#include <io.h>
#include <fcntl.h>

using namespace std;
using namespace chrono_literals;
string colors = "SHCD";
vector<string> cardNames = {
	"JOKER", "THREE", "FOUR", "FIVE", "SIX", "SEVEN", "NINE", "TEN", "JACK", "QUEEN", "KING", "ACE", "TWO","EIGHT"
};


class Card
{
public:
	int rank;
	char suit;
	bool isWild;
	bool isSpecial;
	bool status;
	Card(int r, char s, bool w, bool spec, bool stat) {
		rank = r;
		suit = s;
		isWild = w;
		isSpecial = spec;
		status = stat;

	}
	string GetColor(char color) {
		switch (color) {
		case(SPADE):
			return "Black";
			break;
		case(HEART):
			return "Grey";
			break;
		case(CLUB):
			return "Red";
			break;
		case(DIAMOND):
			return "White";
			break;
		case(WILD_COLOR):
			return "Wild";
			break;
		default:
			break;
		}
	}
	string GetSuit(char suitChar) {
		switch (suitChar) {
		case(SPADE):
			return u8"\u2660";
			break;
		case(HEART):
			return u8"\u2665";
			break;
		case(CLUB):
			return u8"\u2666";
			break;
		case(DIAMOND):
			return u8"\u2663";
			break;
		default:
			return "Wild";
			break;
		}
	}

};

class Player
{
public:
	string Name;
	bool isCPU;
	vector<Card> hand;
	Player(string whatName, bool whatType)
	{
		Name = whatName;
		isCPU = whatType;
	}
	~Player()
	{
		//cout << "Destructor called" << endl; 
	}
};

void initPlayers(vector<Player>& players, string playName, int numOpp = 2)
{
	cout << "PLAYERS\n";
	cout << "-----------------\n";
	players.emplace_back(playName, userPlayer);
	string oppName = "CPU";
	cout << L"> " << playName.c_str() << endl;
	for (int x = 1; x <= numOpp; x++)
	{
		oppName += to_string(x);
		cout << L"> " << oppName.c_str() << endl;
		players.emplace_back(oppName, botPlayer);
		oppName.pop_back();
	}
	cout << "-----------------\n";
}

void displayCard(Card card) {

	switch (card.suit) { // Displays cards with colors in the terminal
	case(SPADE):
		cout << BLACK_TEXT;
		break;
	case(HEART):
		cout << GREY_TEXT;
		break;
	case(CLUB):
		cout << RED_TEXT;
		break;
	case(DIAMOND):
		cout << WHITE_TEXT;
		break;
	default:
		cout << GREY_TEXT;
		break;
	}
	cout << "[" << card.GetSuit(card.suit) << cardNames[card.rank -1] << "]" << RESET_TEXT;
	
}

vector<Card> createDeck() {
	vector<Card> deck;
	char suits[] = { 'S', 'H', 'C', 'D' };
	for (char s : suits) {

		// Deal two of each rank for each color
		for (int i = 2; i <= 13; i++)
		{
			if ((i != 13) && (i != 14))
			{
				deck.push_back(Card(i, s, notWild, notSpecial, notPlayed));


			}
		}
		// The two and 8 are here
		deck.push_back(Card(13, s, notWild, special, notPlayed));
		deck.push_back(Card(14, s, notWild, special, notPlayed));

	}
	// Two Joers
	deck.push_back(Card(1, 'j', wild, special, notPlayed));
	deck.push_back(Card(1, 'j', wild, special, notPlayed));
	//string temp;
	//for (Card s : deck) 
	//{
	//	temp = to_string(s.rank);
	//	displayCard(s);
	//}
	//cout << endl;
	// Shuffle
	random_device rd;
	mt19937 g(rd());
	shuffle(deck.begin(), deck.end(), g);

	return deck;
}


void topDiscardInfo(Card lastPlayedCard, int potentialDraw, bool canRespond) {
	cout << endl << "Last played card: ";
	displayCard(lastPlayedCard);
	if (lastPlayedCard.isWild) {
		cout << "\nCard color: " << lastPlayedCard.GetColor(lastPlayedCard.suit);
	}
	if (canRespond) {
		switch (lastPlayedCard.rank) {
		case(drawTwo):
			cout << "\nOpponent played a draw two! You must play a draw two from your hand or input 0 to draw " << potentialDraw << " cards and pass your turn.\n";
			break;
		case(wildDrawFour):
			cout << "\nOpponent played a draw four! You must play a draw four from your hand or input 0 to draw " << potentialDraw << " cards and pass your turn.\n";
			break;
		default:
			break;
		}
	}

}

void sortHand(vector<Card>& hand, int type = 0) {
	//I am lazy and am using bubble sort, the efficiencies don't matter too much with as small of a scale. 
	int n = hand.size();
	if (type == 0)
	{
		for (int i = 0; i < n - 1; i++)
		{

			bool flag = false;
			for (int j = 0; j < n - i - 1; j++)
			{
				if (hand[j].rank > hand[j + 1].rank)
				{

					swap(hand[j], hand[j + 1]);

					flag = true;
				}
			}

			if (!flag)
				break;
		}
	}
	else
		//Added the sorting for the color first, then sorts by the number. Unsure if wanting to remove just number sorting or allow swapping but leaving in the swapping as an option.
	{
		for (int i = 0; i < n - 1; i++)
		{

			bool flag = false;
			for (int j = 0; j < n - i - 1; j++)
			{
				if (hand[j].suit < hand[j + 1].suit)
				{

					swap(hand[j], hand[j + 1]);

					flag = true;
				}
			}

			for (int j = 0; j < n - i - 1; j++)
			{
				if (hand[j].suit == hand[j + 1].suit)
				{

					if (hand[j].rank > hand[j + 1].rank)
					{

						swap(hand[j], hand[j + 1]);

						flag = true;
					}
				}
			}

			if (!flag)
				break;
		}
	}

}

int selectCard(vector<Card>& hand) {
	int selectedCard = 0;
	char input;
	vector<Card> selectedCards;

	cout << "Use [A] left, [D] right, [P] play the selected card, [O] pass the turn." << endl;

	while (true) {
		cout << CLEAR_LINE;

		for (int i = 0; i < hand.size(); i++) {
			if (i == selectedCard) cout << ">";
			else cout << " ";
			displayCard(hand[i]);
		}

		cout << flush;

		input = _getch();
		input = tolower(input);

		if (input == 'a' && selectedCard > 0)
		{
			selectedCard--;
		}
		else if (input == 'd' && selectedCard < hand.size() - 1)
		{
			selectedCard++;
		}
		else if (input == 'p')
		{
			//selectedCards.push_back(hand(selectedCard));
			return selectedCard;
		}
		else if (input == 'o')
		{
			return -1000;
		}
		else if (input == 'x')
		{
			return -2000;
		}
	}
}


void reshuffle(vector<Card>& deck, vector<Card>& discardPile) {
	if (discardPile.size() <= 1) {
		return;
	}
	for (auto card : discardPile) {
		if (card.isWild) {
			card.suit = WILD_COLOR;
		}
		card.status = notPlayed;
	}
	Card topCard = discardPile.back();
	discardPile.pop_back();
	deck.insert(deck.end(), discardPile.begin(), discardPile.end());
	discardPile.clear();
	discardPile.push_back(topCard);
	random_device rd;
	mt19937 g(rd());
	shuffle(deck.begin(), deck.end(), g);
	cout << "\n| Discard pile shuffled into deck |\n" << endl;
}

Card draw(vector<Card>& deck, vector<Card>& hand, vector<Card>& discardPile) {
	if (deck.empty()) {
		reshuffle(deck, discardPile);
	}
	Card drawCard = deck.back();
	hand.push_back(drawCard);
	deck.pop_back();
	return drawCard;
}

bool isValidCard(Card card, Card lastPlayedCard) {
	return card.rank > lastPlayedCard.rank
		|| card.isWild
		? true : false;
}


bool checkWin(Player currentPlayer, bool& gamePower)
{
	return false;

}


void welcomeScreen() {

	cout << "========================================" << endl;
	// Raw String Literal R"( ... )" handles the spacing automatically
	cout << R"(
♠♠♠♠♠   ♥   ♥   ♣♣♣♣♣   ♦♦♦♦♦   ♦♦♦♦♦   ♠   ♠
  ♠     ♥   ♥   ♣       ♦   ♦   ♦   ♦   ♠♠  ♠
  ♠      ♥ ♥    ♣       ♦   ♦   ♦   ♦   ♠ ♠ ♠
  ♠       ♥     ♣       ♦   ♦   ♦   ♦   ♠  ♠♠
  ♠       ♥     ♣♣♣♣♣   ♦♦♦♦♦   ♦♦♦♦♦   ♠   ♠
					)" << endl;

	cout << "========================================" << endl;
	cout << "            WELCOME TO TYCOON!          " << endl;
	cout << "========================================" << endl;
	return;
}


void clearScreen() {
	// \033[2J clears the screen
	// \033[1;1H moves the cursor to the top-left corner
	cout << CLEAR_SCREEN;
}


void dealCards(vector<Card>& deck, Player& humanPlayer, vector<Player>& players, int numOpp, vector<Card>& discardPile) {
	int totalPlayers = numOpp + 1; 
	int trueHand = 54 / totalPlayers;
	
	if (devMode) {
		Card devWildCard = Card(wildCard, WILD_COLOR, wild, special, notPlayed);
		Card devWildDrawFour = Card(wildDrawFour, WILD_COLOR, wild, special, notPlayed);
		Card devDrawTwo = Card(drawTwo, BLUE, notWild, special, notPlayed);
		for (int i = 0; i < STARTING_HAND_SIZE; i++) {
			humanPlayer.hand.push_back(devWildDrawFour);
		}
		for (int x = 0; x < numOpp; x++) {
			for (int i = 0; i < STARTING_HAND_SIZE; i++) {
				//players[x + 1].hand.push_back(devWildDrawFour);
				draw(deck, players[x + 1].hand, discardPile);
			}
		}
	}
	else {
		cout << endl <<  to_string(trueHand) << endl;
		for (int i = 0; i < trueHand; i++) {
			draw(deck, humanPlayer.hand, discardPile);
		}
		for (int x = 0; x < numOpp; x++) {
			for (int i = 0; i < trueHand; i++) {
				draw(deck, players[x + 1].hand, discardPile);
			}
		}
	}
}



void specialCardLogic(Card playedCard,int& turnCount,vector<Player>& players,int currentPlayer) {
	if (playedCard.rank == 14) {
		cout << players[currentPlayer].Name
			<< " played an 8! The trick is cleared and they play again.\n";

		turnCount--;
	}
}



void playerCardLogic(vector<Player>& players, string colors, Card& playedCard,int& cardToPlay, int& turnCount, int& iterator,int& potentialDraw, bool& canRespond, vector<Card>& deck,
	vector<Card>& discardPile, string& wildColor, Card& lastPlayedCard,int& nextPlayer) 
{
	players[0].hand.erase(players[0].hand.begin() + (cardToPlay - 1));
	playedCard.status = played;
	if (playedCard.isSpecial) {
		specialCardLogic(playedCard, turnCount, players, HUMAN);
	}
	discardPile.push_back(playedCard);
	lastPlayedCard = playedCard;
}



Card initRound(string& playerName, int& playerCount, vector<Player>& players, vector<Card>& deck, vector<Card>& discardPile, int& potentialDraw) {
	cout << "Enter your name: ";
	while (!(cin >> playerName))
	{
		cout << "Invalid input. Try again with a valid name";
		cin.clear();
		cin.ignore(10000, '\n');
	}
	if (playerName == "CPU0") {
		botMode = true;
	}

	cout << "Enter the total amount of opponents you want (maximum of 10 for now): ";
	while (!(cin >> playerCount))
	{
		cout << "Invalid input. Try again with ";
		cin.clear();
		cin.ignore(10000, '\n');
	}
	clearScreen();
	cout << "Bot mode active. Enjoy the show!\n";
	cout << "A total of " << playerCount + 1 << " players are competing. Good luck!\n" << endl;

	initPlayers(players, playerName, playerCount);
	dealCards(deck, players[0], players, playerCount, discardPile);
	Card firstCard = draw(deck, discardPile, discardPile);
	if (firstCard.isSpecial) {
		while (firstCard.isSpecial) {
			firstCard = draw(deck, discardPile, discardPile);
		}
	}
	firstCard.status = played;
	return firstCard;
}



void cpuTurn(int currentPlayer, Card& lastPlayedCard, vector<Player>& players,vector<Card>& deck, vector<Card>& discardPile, int& turnCount) {

	Player& cpu = players[currentPlayer];
	bool playedd = false;

	
	for (int i = 0; i < cpu.hand.size(); i++) {
		if (cpu.hand[i].rank > lastPlayedCard.rank) 
		{
			Card playedCard = cpu.hand[i];
			cpu.hand.erase(cpu.hand.begin() + i); 
			playedCard.status = played;
			discardPile.push_back(playedCard);
			lastPlayedCard = playedCard;

			cout << cpu.Name << " played: ";
			displayCard(playedCard);
			cout << endl;

			playedd = true;
			if (playedCard.isSpecial) {
				specialCardLogic(playedCard, turnCount, players, HUMAN);
			}
			break;
		}
	}

	if (!playedd) 
	{
		// No card to play, CPU passes
		cout << cpu.Name << " has no valid card to play and passes the turn." << endl;
	}

	this_thread::sleep_for(DELAY);
}



void playTycoon() {
	//Commented out some of the variables that are no longer useful
	welcomeScreen();
	int potentialDraw = 0;

	int highestOfRound = 0;
	bool roundOver = false;
	int cardToPlay;
	bool gamePower = powerOn;
	//bool validCardPlayed;
	string wildColor;
	vector<Card> deck = createDeck();
	vector<Card> discardPile;
	vector<Card> tempHand;
	vector<int> tempPlaceCounter;

	//vector<Card> playerHand;
	//vector<Card> opponentHand;
	int iterator = 1;
	bool canRespond = false;
	string playerName;
	int playerCount = 0;
	vector<Player> players;
	int turnCount = players.size() * 50;
	int nextPlayer = 0;
	int currentPlayer = 0;
	Card lastPlayedCard = initRound(playerName, playerCount, players, deck, discardPile, potentialDraw);
	int round = 1;
	while (gamePower) {
		
		currentPlayer = turnCount % players.size();
		nextPlayer = (turnCount + iterator) % players.size();
		if (turnCount % players.size() == 0 && !botMode) {
			cout << "| CURRENT HAND SIZES |" << endl;
			for (auto player : players) {
				cout << player.Name << ": " << player.hand.size() << endl;
			}
			topDiscardInfo(lastPlayedCard, potentialDraw, canRespond);

			sortHand(players[0].hand);

			cout << endl << "Your hand: " << endl;
			if (devMode) 
			{
				for (auto player : players) 
				{
					if (player.isCPU) 
					{
						cout << "\nHand of " << player.Name << " (devMode on)\n";
						for (auto cards : player.hand) 
						{
							displayCard(cards);
						}
					}
				}
			}
			if (!canRespond) {

				cardToPlay = selectCard(players[0].hand) + 1;
				while (true) {


					if ((cardToPlay == -1999) || (cardToPlay == -999)) {
						break;
					} 
					

					tempHand.push_back(players[0].hand[cardToPlay-1]);
					tempPlaceCounter.push_back(cardToPlay);


					cout << endl << "Your current play: " << endl;
					for (auto cardss : tempHand)
					{
						displayCard(cardss);
					}
					cout << endl << endl;
					cardToPlay = selectCard(players[0].hand) + 1;

					while (players[0].hand[cardToPlay - 1].rank != tempHand[0].rank)
					{
						
						cout << endl << endl << "Incorrectly chosen card! Select one that matches the other chosen card in rank! " << endl << endl;
						cardToPlay = selectCard(players[0].hand) + 1;

						if ((cardToPlay == -1999) || (cardToPlay == -999)) {
							break;
						}

					}
				}
				
			}
			//else {
			//	cout << "\nYou must play a card with the greater number last one or input 0 to pass " << potentialDraw << " cards!\n";
			//	cin >> cardToPlay;
			//}

			if (canRespond && cardToPlay > 0) {
				Card playedCard = players[0].hand[cardToPlay - 1];
				if (playedCard.rank == lastPlayedCard.rank) {
					canRespond = false;
				}
			}
			if (cardToPlay > 0 && cardToPlay <= players[0].hand.size() && !canRespond) 
			{
				Card playedCard = players[0].hand[cardToPlay - 1];
				if (isValidCard(playedCard, lastPlayedCard)) {
					clearScreen();
					playerCardLogic(players, colors, playedCard, cardToPlay, turnCount, iterator, potentialDraw, canRespond, deck, discardPile, wildColor, lastPlayedCard, nextPlayer); // 
				}
				else 
				{
					cout << "Invalid play! Select another card or draw to pass your turn." << endl;
					continue;
				}
			}
			else if (cardToPlay == -999) 
			{ // slightly adjusted the execution to fit the new method. 
				cout << players[0].Name << " passed this turn, keep over." << endl;
				roundOver = true;
			}
			else if (cardToPlay == -1999) 
			{ // slightly adjusted the execution to fit the new method. 
				cout << endl << "Game prematureyl ended!" << endl;
				gamePower = false;
				roundOver = true;
			}

			else 
			{
				if (canRespond) 
				{
					cout << "\nInvalid play!\nYou must play a card with the same draw effect as the one just played or input 0 to draw " << potentialDraw << "cards!\n";
				}
				else 
				{
					cout << "\nInvalid play!\n";
				}
				continue;
			}
		}
		else
		{
			cout << players[currentPlayer].Name << "'s turn!\n";
			cout << "Last played card: ";
			displayCard(lastPlayedCard);
			//if (lastPlayedCard.isWild) {
			//	cout << " (Chosen color: " << lastPlayedCard.GetColor(lastPlayedCard.suit) << ") \n";
			//}
			cout << endl;
			this_thread::sleep_for(DELAY);
			cpuTurn(currentPlayer, lastPlayedCard, players, deck, discardPile, turnCount);
			cout << endl;
			if (botMode) round++;
		}

		if (roundOver == true) {
			highestOfRound = 0;
			roundOver == false;
		}
		turnCount += iterator;
		checkWin(players[currentPlayer], gamePower);
	}
}


void mainMenu() {
	string userInput;
	bool programPower = powerOn;
	cout << "Input \"S\" to start playing!" << endl << "At any time, input \"X\" to terminate the program.\nInput \"D\" to launch the game in dev mode (hand is all wilds by default, can be changed in deal function.)" << endl;
	while (programPower) {
		cout << ">> ";
		cin >> userInput;
		switch (toupper(userInput[0])) {
		case(START):
			//clearScreen();
			cout << "Game starting..." << endl;
			playTycoon();
			createDeck();

			programPower = powerOff;
			break;
		case(TERMINATE):
			cout << "Terminating program... Thanks for playing!" << endl;
			programPower = powerOff;
			break;
		case(DEV_MODE):
			//clearScreen();
			cout << "Game starting in dev mode..." << endl;
			devMode = true;
			//playUno();
			programPower = powerOff;
			break;
		default:
			cout << "Unknown command!" << endl;
			break;
		}
	}
	return;
}



int main() {
	
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);


	// Output Unicode characters
	//std::wcout << L"Español, Über, Crème brûlée, 你好, こんにちは" << std::endl;

	
	mainMenu();
	return 0;
}

