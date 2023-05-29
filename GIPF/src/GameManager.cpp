#include "GameManager.h"

GameManager::GameManager()
{
	board = nullptr;
	height = 0;
	width = 0;
	maxLetter = NULL;
	setup();
}

GameManager::~GameManager()
{
	if (board != nullptr)
	{
		for (int i = 0; i < height + 2; i++)
			delete[] board[i];
		delete[] board;
	}
}

void GameManager::start()
{
	string input;
	char buffer[100];
	char key = NULL;
	int counter = 0;

	while (key != '\xff')
	{
		key = getchar();
		buffer[counter] = key;
		counter++;

		if(key == ' ' || key == '\n' || key == '\xff')
		{
			counter--;
			buffer[counter] = '\0';
			runAction(buffer, counter);
			counter = 0;
		}
	}
}

void GameManager::runAction(char* buffer, int length)
{
	if (length < 7)
		return;

	try
	{
		if (strcmp(buffer, "DO_MOVE") == 0)
			move();
		else if (strcmp(buffer, "LOAD_GAME_BOARD") == 0)
			loadBoard();
		else if (strcmp(buffer, "PRINT_GAME_BOARD") == 0)
			printBoard();
	}
	catch (const MapLoadException& e)
	{
		cout << e.what() << "\n\n";
	}
	catch (const MoveException& e)
	{
		cout << e.what() << "\n\n";
	}
	catch (...)
	{
		cout << "AN UNKNOWN ERROR OCCURED\n\n";
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

void GameManager::loadBoard()
{
	if (board != nullptr)
	{
		for (int i = 0; i < height + 2; i++)
			delete[] board[i];
		delete[] board;
	}

	setup();
	char turn;
	int spaceLength = 0;
	cin >> edgeLength >> numberOfTriggerPawns >> numberOfWhitePawns >> numberOfBlackPawns;
	cin >> numberOfWhitePawnsInReserve >> numberOfBlackPawnsInReserve >> turn;
	isWhiteTurn = (turn == 'W' ? true : false);

	height = 2 * edgeLength - 1;
	width = 2 * height - 1;

	board = new char* [height + 2];
	for (int i = 0; i < height + 2; i++)
	{
		spaceLength = (i < edgeLength ? edgeLength - i : i - edgeLength);
		board[i] = new char[width + 4];
		for (int j = 0; j < width + 4; j++)
			if (j >= spaceLength && j <= width + 4 - spaceLength && (j - spaceLength) % 2 == 0)
				board[i][j] = '+';
			else
				board[i][j] = ' ';
	}

	maxLetter = 'a' + (width + 4) / 2;

	char temp = NULL;
	bool flag = true;
	int counter = 0;

	getchar();
	for (int i = 0; i < height; i++)
	{
		spaceLength = (i < edgeLength ? edgeLength - i - 1 : i - edgeLength + 1);
		flag = true;
		counter = 0;
		int j = 0;

		while(true)
		{
			temp = getchar();
			if (temp == '\n')
				break;

			if (j > width)
			{
				if (temp != ' ')
					throw MapLoadException("WRONG_BOARD_ROW_LENGTH");
				continue;
			}

			board[i + 1][j + 2] = temp;

			switch (temp)
			{
				case 'W':
				{
					counter++;
					numberOfWhitePawnsOnBoard++;
					break;
				}
				case 'B':
				{
					counter++;
					numberOfBlackPawnsOnBoard++;
					break;
				}
				case '_':
				{
					counter++;
					break;
				}
			}
			j++;
		}

		if(counter != (i < edgeLength ? edgeLength + i : height - (i - edgeLength + 1)))
			throw MapLoadException("WRONG_BOARD_ROW_LENGTH");
	}

	if (numberOfWhitePawnsOnBoard > numberOfWhitePawns - numberOfWhitePawnsInReserve)
		throw MapLoadException("WRONG_WHITE_PAWNS_NUMBER");
	else if (numberOfBlackPawnsOnBoard > numberOfBlackPawns - numberOfBlackPawnsInReserve)
		throw MapLoadException("WRONG_BLACK_PAWNS_NUMBER");

	cout << "BOARD_STATE_OK\n\n";
}

void GameManager::printBoard()
{
	char temp = NULL;
	cout << edgeLength << ' ' << numberOfTriggerPawns << ' ' << numberOfWhitePawns << ' ' << numberOfBlackPawns << '\n';
	cout << numberOfWhitePawnsInReserve << ' ' << numberOfBlackPawnsInReserve << ' ' << (isWhiteTurn ? 'W' : 'B') << '\n';

	for (int i = 1; i < height + 1; i++)
	{
		for (int j = 2; j < width + 3 - (i > edgeLength ? i - edgeLength : 0); j++)
		{
			temp = board[i][j];
			
			if(temp == '+')
				temp = ' ';

			cout << temp;
		}

		cout << '\n';
	}
	cout << '\n';
}

void GameManager::move()
{
	string coords;
	cin >> coords;

	int x = coords.find('-');
	string first = coords.substr(0, x);
	string second = coords.substr(x + 1, coords.length() - 1);
	
	char startLetter = first[0];
	int startNumber = stoi(first.substr(1));

	char endLetter = second[0];
	int endNumber = stoi(second.substr(1));

	int maxNumber = 2 * edgeLength + 1;
	char middleLetter = 'a' + edgeLength;
	int maxStartNumber = maxNumber - abs(startLetter - middleLetter);
	int maxEndNumber = maxNumber - abs(endLetter - middleLetter);

	if (startLetter > maxLetter || startNumber > maxStartNumber)
	{
		cout << "BAD_MOVE_" + first + "_IS_WRONG_INDEX\n\n";
		return;
		//throw MoveException("BAD_MOVE_" + first + "_IS_WRONG_INDEX");
	}
	else if (endLetter > maxLetter || endNumber > maxEndNumber)
	{
		cout << "BAD_MOVE_" + second + "_IS_WRONG_INDEX\n\n";
		return;
		//throw MoveException("BAD_MOVE_" + second + "_IS_WRONG_INDEX");
	}
	else if (((startLetter == 'a' || startLetter == maxLetter) && startNumber > edgeLength + 1) || (startLetter != 'a' && startLetter != maxLetter && startNumber != 1 && startNumber < maxStartNumber))
	{
		cout << "BAD_MOVE_" + first + "_IS_WRONG_STARTING_FIELD\n\n";
		return;
		//throw MoveException("BAD_MOVE_" + first + "_IS_WRONG_STARTING_FIELD");
	}
	else if (endLetter == 'a' || endLetter == maxLetter || endNumber == 1 || endNumber >= maxEndNumber)
	{
		cout << "BAD_MOVE_" + second + "_IS_WRONG_DESTINATION_FIELD\n\n";
		return;
		//throw MoveException("BAD_MOVE_" + second + "_IS_WRONG_DESTINATION_FIELD");
	}
	else if (abs(endLetter - startLetter) > 1 || abs(endNumber - startNumber) > 1)
	{
		cout << "UNKNOWN_MOVE_DIRECTION\n\n";
		return;
		//throw MoveException("UNKNOWN_MOVE_DIRECTION");
	}

	int firstColumn, firstRow, secondColumn, secondRow;
	getPosition(firstRow, firstColumn, startLetter, startNumber);
	getPosition(secondRow, secondColumn, endLetter, endNumber);

	int horizontalDirection = secondColumn - firstColumn;
	int verticalDirection = secondRow - firstRow;

	char symbol = NULL;
	int horizontalIter = (horizontalDirection > 0) - (horizontalDirection < 0);
	int verticalIter = (verticalDirection > 0) - (verticalDirection < 0);
	
	if (verticalIter == 0)
		horizontalIter *= 2;
	
	int verticalCount = 0;
	int horizontalCount = 0;

	while (true)
	{
		symbol = board[secondRow + verticalCount][secondColumn + horizontalCount];

		if (symbol == '+')
		{
			cout << "BAD_MOVE_ROW_IS_FULL\n\n";
			return;
			//throw MoveException("BAD_MOVE_ROW_IS_FULL");
		}
		else if (symbol == '_')
			break;

		verticalCount += verticalIter;
		horizontalCount += horizontalIter;
	}

	char tempSymbol = NULL;

	while (verticalCount != 0 || horizontalCount != 0)
	{
		board[secondRow + verticalCount][secondColumn + horizontalCount] = board[secondRow + verticalCount - verticalIter][secondColumn + horizontalCount - horizontalIter];
		verticalCount -= verticalIter;
		horizontalCount -= horizontalIter;
	}

	board[secondRow][secondColumn] = (isWhiteTurn ? 'W' : 'B');

	if (isWhiteTurn)
	{
		numberOfWhitePawnsInReserve--;
		numberOfWhitePawnsOnBoard++;
	}
	else
	{
		numberOfBlackPawnsInReserve--;
		numberOfBlackPawnsOnBoard++;
	}

	isWhiteTurn = !isWhiteTurn;
	cout << "MOVE_COMMITTED\n\n";
}

void GameManager::getPosition(int& x, int& y, int letter, int number)
{
	y = (letter - 'a') * 2;
	x = (height + 2) / 2;
	int temp = (letter - 'a' >= edgeLength ? edgeLength + 1 : letter - 'a' + 1);
	if (number > temp)
	{
		y += number - temp;
		x -= number - temp;
	}
	else if(number < temp)
	{
		y -= temp - number;
		x += temp - number;
	}
}