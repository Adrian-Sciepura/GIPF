#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <iostream>
#include <functional>
#include <string>
#include <map>
#include <string.h>

#include "MapLoadException.h"

using std::cout;
using std::cin;
using std::string;
using std::map;

class GameManager
{
private:
	/*using managerFunction = void (GameManager::*)();
	using element = std::pair<string, managerFunction>;*/

	char** board;
	int edgeLength;
	int numberOfTriggerPawns;
	
	int numberOfWhitePawns;
	int numberOfWhitePawnsInReserve;
	int numberOfWhitePawnsOnBoard;
	
	int numberOfBlackPawns;
	int numberOfBlackPawnsInReserve;
	int numberOfBlackPawnsOnBoard;

	bool isWhiteTurn;
	//map<string, managerFunction> functions;

	void runAction(char* buffer, int length);
	void setup();
	void loadBorad();

public:
	GameManager();
	~GameManager();

	void start();
};

#endif