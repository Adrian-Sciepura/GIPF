#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <iostream>
#include <string>
#include <string.h>
#include <sstream>

using std::cout;
using std::cin;
using std::string;
using std::istringstream;

class GameManager
{
private:
	char** board;
	bool isMapLoaded;
	int height;
	int width;
	char maxLetter;

	int edgeLength;
	int numberOfTriggerPawns;
	
	int numberOfWhitePawns;
	int numberOfWhitePawnsInReserve;
	int numberOfWhitePawnsOnBoard;
	
	int numberOfBlackPawns;
	int numberOfBlackPawnsInReserve;
	int numberOfBlackPawnsOnBoard;

	bool isWhiteTurn;

	void runAction(char* buffer, int length);
	void setup();
	string loadBoard();
	void printBoard();
	string move();
	void getPosition(int& x, int& y, int letter, int number);
	int checkRow(int startRow, int startColumn, int horizontalIterator, int verticalIterator, bool remove = false);
	string captureThePawns(const string& who, const string& from, const string& to);

public:
	GameManager();
	~GameManager();

	void start();
};

#endif