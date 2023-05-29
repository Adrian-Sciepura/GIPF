#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <iostream>
#include <functional>
#include <string>
#include <map>
#include <string.h>

#include "MapLoadException.h"
#include "MoveException.h"

using std::cout;
using std::cin;
using std::string;
using std::map;

class GameManager
{
private:
	char** board;
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
	void loadBoard();
	void printBoard();
	void move();
	void getPosition(int& x, int& y, int letter, int number);
	int checkRow(int startRow, int startColumn, int horizontalIterator, int verticalIterator);

public:
	GameManager();
	~GameManager();

	void start();
};

#endif