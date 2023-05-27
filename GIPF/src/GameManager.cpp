#include "GameManager.h"

GameManager::GameManager()
{
	functions.insert(element("LOAD_GAME_BOARD", &GameManager::loadBorad));
	board = nullptr;
	setup();
}

GameManager::~GameManager()
{
}

void GameManager::start()
{
	string input;
	while (cin >> input)
	{
		try
		{
			if(functions.find(input) != functions.end())
				(this->*functions[input])();
		}
		catch (const MapLoadException& e)
		{
			cout << e.what() << '\n';
		}
		catch (...)
		{
			cout << "An unknown error occured\n";
		}
	}
}

void GameManager::setup()
{
	edgeLength = 0;
	numberOfTriggerPawns = 0;

	numberOfWhitePawns = 0;
	numberOfWhitePawnsInReserve = 0;
	numberOfWhitePawnsOnBoard = 0;

	numberOfBlackPawns = 0;
	numberOfBlackPawnsInReserve = 0;
	numberOfBlackPawnsOnBoard = 0;

	isWhiteTurn = true;
}

void GameManager::loadBorad()
{
	setup();
	char turn;
	cin >> edgeLength >> numberOfTriggerPawns >> numberOfWhitePawns >> numberOfBlackPawns;
	cin >> numberOfWhitePawnsInReserve >> numberOfBlackPawnsInReserve >> turn;
	isWhiteTurn = (turn == 'W' ? true : false);

	int height = 2 * edgeLength - 1;
	int width = 2 * height - 1;

	char temp = NULL;
	int spaceLength = 0;
	bool flag = true;
	int counter = 0;

	getchar();
	for (int i = 0; i < height; i++)
	{
		spaceLength = (i < edgeLength ? edgeLength - i - 1 : i - edgeLength + 1);
		flag = true;
		counter = 0;

		for (int j = 0; j <= width; j++)
		{
			if(flag)
				temp = getchar();
			
			if(temp < ' ')
			{
				flag = false;
				temp = NULL;
			}
			else
			{
				if (flag && !(j < spaceLength || j > width - spaceLength))
				{
					counter++;
					switch (temp)
					{
						case 'W':
						{
							numberOfWhitePawnsOnBoard++;
							break;
						}
						case 'B':
						{
							numberOfBlackPawnsOnBoard++;
							break;
						}
					}
				}
			}
		}
		if(counter != width - 2 * spaceLength || flag)
			throw MapLoadException("WRONG_BOARD_ROW_LENGTH");
	}

	if (numberOfWhitePawnsOnBoard != numberOfWhitePawns - numberOfWhitePawnsInReserve)
		throw MapLoadException("WRONG_WHITE_PAWNS_NUMBER");
	else if (numberOfBlackPawnsOnBoard != numberOfBlackPawns - numberOfBlackPawnsInReserve)
		throw MapLoadException("WRONG_BLACK_PAWNS_NUMBER");

	cout << "BOARD_STATE_OK\n";
	
}