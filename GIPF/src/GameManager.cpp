#include "GameManager.h"

GameManager::GameManager()
{
	board = nullptr;
	height = 0;
	width = 0;
	maxLetter = NULL;
	isMapLoaded = false;
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

	if (strcmp(buffer, "DO_MOVE") == 0)
		cout << move() << "\n\n";
	else if (strcmp(buffer, "LOAD_GAME_BOARD") == 0)
		cout << loadBoard() << "\n\n";
	else if (strcmp(buffer, "PRINT_GAME_BOARD") == 0)
		printBoard();
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

string GameManager::loadBoard()
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
	int counter = 0;
	int numberOfTooLongRows = 0;
	int numberOfBlackPawnsInRow = 0;
	int numberOfWhitePawnsInRow = 0;

	getchar();
	for (int i = 0; i < height; i++)
	{
		spaceLength = (i < edgeLength ? edgeLength - i - 1 : i - edgeLength + 1);
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
				{
					isMapLoaded = false;
					return string("WRONG_BOARD_ROW_LENGTH");
				}
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

		board[i + 1][spaceLength] = '+';
		board[i + 1][width + 3 - spaceLength] = '+';

		if(counter != (i < edgeLength ? edgeLength + i : height - (i - edgeLength + 1)))
		{
			isMapLoaded = false;
			return string("WRONG_BOARD_ROW_LENGTH");
		}
	
		if (numberOfWhitePawnsInRow >= numberOfTriggerPawns)
			numberOfTooLongRows++;
		if(numberOfBlackPawnsInRow >= numberOfTriggerPawns)
			numberOfTooLongRows++;
	}

	if (numberOfWhitePawnsOnBoard > numberOfWhitePawns - numberOfWhitePawnsInReserve)
	{
		isMapLoaded = false;
		return string("WRONG_WHITE_PAWNS_NUMBER");
	}
	else if (numberOfBlackPawnsOnBoard > numberOfBlackPawns - numberOfBlackPawnsInReserve)
	{
		isMapLoaded = false;
		return string("WRONG_BLACK_PAWNS_NUMBER");
	}

	char tempLetter = 'b';
	int tempRow = 0;
	int tempColumn = 0;

	for(int i = 1; i < height; i++)
	{
		spaceLength = (i < edgeLength ? edgeLength - i + 2 : i - edgeLength + 2);
		numberOfTooLongRows += checkRow(i, spaceLength, 2, 0);
	}

	for (int i = 0; i < height; i++)
	{
		temp = NULL;
		getPosition(tempRow, tempColumn, tempLetter, 2);
		numberOfWhitePawnsInRow = 0;
		numberOfBlackPawnsInRow = 0;
		numberOfTooLongRows += checkRow(tempRow, tempColumn, 1, -1);
		numberOfTooLongRows += checkRow(tempRow, width + 3 - tempColumn, -1, -1);
		tempLetter++;
	}

	if (numberOfTooLongRows != 0)
	{
		isMapLoaded = false;

		if(numberOfTooLongRows == 1)
			return string("ERROR_FOUND_1_ROW_OF_LENGTH_K");
		else
			return string("ERROR_FOUND_" + std::to_string(numberOfTooLongRows) + "_ROWS_OF_LENGTH_K");
	}

	isMapLoaded = true;
	return string("BOARD_STATE_OK");
}

void GameManager::printBoard()
{
	if (!isMapLoaded)
	{
		cout << "EMPTY_BOARD\n\n";
		return;
	}

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
	
	/*for (int i = 0; i < height + 2; i++)
	{
		for (int j = 0; j < width + 4; j++)
		{
			temp = board[i][j];
			cout << temp;
		}
		cout << '\n';
	}
	cout << '\n';*/
}

string GameManager::move()
{
	string query;
	std::getline(std::cin, query);
	std::istringstream iss(query);
	
	string coords;
	string who;
	string from;
	string to;

	iss >> coords >> who >> from >> to;
	
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
		return string("BAD_MOVE_" + first + "_IS_WRONG_INDEX");
	else if (endLetter > maxLetter || endNumber > maxEndNumber)
		return string("BAD_MOVE_" + second + "_IS_WRONG_INDEX");
	else if (((startLetter == 'a' || startLetter == maxLetter) && startNumber > edgeLength + 1) || (startLetter != 'a' && startLetter != maxLetter && startNumber != 1 && startNumber < maxStartNumber))
		return string("BAD_MOVE_" + first + "_IS_WRONG_STARTING_FIELD");
	else if (endLetter == 'a' || endLetter == maxLetter || endNumber == 1 || endNumber >= maxEndNumber)
		return string("BAD_MOVE_" + second + "_IS_WRONG_DESTINATION_FIELD");
	else if (abs(endLetter - startLetter) > 1 || abs(endNumber - startNumber) > 1)
		return string("UNKNOWN_MOVE_DIRECTION");

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
			return string("BAD_MOVE_ROW_IS_FULL");
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

	string message = captureThePawns(who, from, to);
	if(message != "")
		return message;


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
	return string("MOVE_COMMITTED");
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

int GameManager::checkRow(int startRow, int startColumn, int horizontalIterator, int verticalIterator, bool remove)
{
	char temp = NULL;
	char previous = NULL;
	int numberInRow = 1;
	int counter = 0;
	while (true)
	{
		temp = board[startRow][startColumn];

		if (temp == previous && temp != '_' && temp != '+')
			numberInRow++;
		else
		{
			if (numberInRow >= numberOfTriggerPawns)
			{
				counter++;
				if (remove)
				{
					int tempRow = startRow;
					int tempColumn = startColumn;
					char symbol = board[tempRow][tempColumn];

					while (symbol != '_' && symbol != '+')
					{
						tempRow += verticalIterator;
						tempColumn += horizontalIterator;
						symbol = board[tempRow][tempColumn];
					}

					tempRow -= verticalIterator;
					tempColumn -= horizontalIterator;
					
					while (true)
					{
						symbol = board[tempRow][tempColumn];
						if(symbol == '_' || symbol == '+')
							break;

						board[tempRow][tempColumn] = '_';
						tempRow -= verticalIterator;
						tempColumn -= horizontalIterator;
					}

					previous == 'W' ? numberOfWhitePawnsInReserve += numberInRow : numberOfBlackPawnsInReserve += numberInRow;
				}
			}
			numberInRow = 1;
		}

		if (temp == '+')
			break;

		previous = temp;
		startRow += verticalIterator;
		startColumn += horizontalIterator;
	}

	if (numberInRow >= numberOfTriggerPawns)
		counter++;

	return counter;
}

string GameManager::captureThePawns(const string& who, const string& from, const string& to)
{
	char symbol = (who != "" ? who[0] - 32 : NULL);
	
	int spaceLength = 0;
	char tempLetter = 'b';
	int firstRow = 0;
	int firstColumn = 0;
	int secondRow = 0;
	int secondColumn = 0;

	if (from != "")
	{
		char firstLetter, secondLetter;
		int firstNumber, secondNumber;

		firstLetter = from[0];
		firstNumber = stoi(from.substr(1));

		secondLetter = to[0];
		secondNumber = stoi(to.substr(1));


		getPosition(firstRow, firstColumn, firstLetter, firstNumber);
		getPosition(secondRow, secondColumn, secondLetter, secondNumber);
		int horizontalDirection = secondColumn - firstColumn;
		int horizontalIter = (horizontalDirection > 0) - (horizontalDirection < 0);
		int verticalDirection = secondRow - firstRow;
		int verticalIter = (verticalDirection > 0) - (verticalDirection < 0);
		
		if(verticalIter == 0)
			horizontalIter *= 2;

		char temp = NULL;
		int counter = 1;
		int howManyInRow = 0;

		while (true)
		{
			temp = board[firstRow][firstColumn];

			if (temp == '_' || temp == '+')
				break;

			if(temp != symbol && counter < numberOfTriggerPawns)
				return string("WRONG_COLOR_OF_CHOSEN_ROW");
			else if(temp == symbol)
				howManyInRow++;


			firstRow += verticalIter;
			firstColumn += horizontalIter;
			counter++;

			if(firstRow == secondRow && firstColumn == secondColumn && counter < numberOfTriggerPawns)
				return string("WRONG_INDEX_OF_CHOSEN_ROW");
		}	

		firstRow -= verticalIter;
		firstColumn -= horizontalIter;
		temp = board[firstRow][firstColumn];

		while (temp != '_' && temp != '+')
		{
			temp = board[firstRow][firstColumn];
			board[firstRow][firstColumn] = '_';
			firstRow -= verticalIter;
			firstColumn -= horizontalIter;
		}
		
		if(symbol == 'W')
			numberOfWhitePawnsInReserve += howManyInRow;
		else
			numberOfBlackPawnsInReserve += howManyInRow;

		return string("");
	}


	for (int i = 1; i < height; i++)
	{
		spaceLength = (i < edgeLength ? edgeLength - i + 2 : i - edgeLength + 2);
		checkRow(i, spaceLength, 2, 0, true);
	}

	for (int i = 0; i < height; i++)
	{
		symbol = NULL;
		getPosition(firstRow, firstColumn, tempLetter, 2);
		checkRow(firstRow, firstColumn, 1, -1, true);
		checkRow(firstRow, width + 3 - firstColumn, -1, -1, true);
		tempLetter++;
	}

	return string("");
}