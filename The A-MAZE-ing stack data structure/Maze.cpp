#include "Maze.h"

std::vector<std::vector<char>> convert1DStringArrayTo2DCharArray(const std::vector<std::string>& stringMaze)
{
	std::vector<std::vector<char>> maze;

	for (const std::string& rowOfCharactersInMaze : stringMaze)
	{
		std::vector<char> currentRowInMaze;
		for (const char character : rowOfCharactersInMaze)
		{
			currentRowInMaze.push_back(character);
		}
		maze.push_back(currentRowInMaze);
	}

	return maze;
}

void printMaze(const std::vector<std::vector<char>>& maze)
{
	for (const std::vector<char>& rowInMaze : maze)
	{
		for (const char character : rowInMaze)
		{
			std::cout << character;
		}
		std::cout << "\n";
	}
}

std::pair<int, int> getLocationOfCharacterIn2DMaze(const std::vector<std::vector<char>>& maze, const char theCharacterToFind)
{
	std::pair<int, int> charactersRowAndColumn = { -1,-1 };

	for (int row = 0; row < maze.size(); ++row)
	{
		for (int col = 0; col < maze[0].size(); ++col)
		{
			if (maze[row][col] == theCharacterToFind)
			{
				charactersRowAndColumn = { row, col };

				return charactersRowAndColumn;
			}
		}
	}

	std::cout << "Character " << theCharacterToFind << " not found in maze!\n";
	__debugbreak();
	return charactersRowAndColumn; //this will be -1, -1
}

std::map<std::string, char> getMapOfNeighbors(const std::pair<int, int>& currentRowAndColumn, const std::vector<std::vector<char>>& maze)
{
	std::map<std::string, char> mapOfNeighbors;

	mapOfNeighbors["Down"] = maze[currentRowAndColumn.first + 1][currentRowAndColumn.second];
	mapOfNeighbors["Left"] = maze[currentRowAndColumn.first][currentRowAndColumn.second - 1];
	mapOfNeighbors["Right"] = maze[currentRowAndColumn.first][currentRowAndColumn.second + 1];
	mapOfNeighbors["Up"] = maze[currentRowAndColumn.first - 1][currentRowAndColumn.second];

	return mapOfNeighbors;
}

std::string getBacktrackingDirection(const std::string& lastDirection)

{

	if (lastDirection == "Up")
	{
		return "Down";
	}

	else if (lastDirection == "Down")
	{
		return "Up";
	}

	else if (lastDirection == "Left")
	{
		return "Right";
	}

	else if (lastDirection == "Right")
	{
		return "Left";
	}

	else
	{
		std::cout << "How'd ya get here?\n";
		__debugbreak();
	}

}

void backtrack(std::vector<std::vector<char>>& maze, std::stack<std::string>& stackOfDirections, std::pair<int, int>& currentRowAndColumn, bool& needToBacktrack)

{
	if (needToBacktrack == false)
	{
		return;
	}

	else
	{
		std::string lastDirection = stackOfDirections.top();
		std::string backtrackingDirection = getBacktrackingDirection(lastDirection);
		stackOfDirections.pop();
		//stackOfDirections.push(backtrackingDirection);

		//now move, check if still at a dead end, then make recursive call: 
		std::cout << "\t\t\tBACKTRACKING " << backtrackingDirection << "\n"; 
		//remove "to the" -> sounds dumb

		moveToNextPosition(maze, currentRowAndColumn, backtrackingDirection);
		printMaze(maze); 

		std::map<std::string, char> mapOfNeighbors = getMapOfNeighbors(currentRowAndColumn, maze);
		bool isNewMoveAvailable = addNextMoveToStackOfDirections(mapOfNeighbors, stackOfDirections);

		if (isNewMoveAvailable)
		{
			needToBacktrack = false;
			return; //we have stepped back far enough that new path is available
		}

		else
		{
			needToBacktrack = true;
			backtrack(maze, stackOfDirections, currentRowAndColumn, needToBacktrack);
		}

	}
}

bool addNextMoveToStackOfDirections(const std::map<std::string, char>& mapOfNeighbors, std::stack<std::string>& stackOfDirections)

{

#if _MSVC_LANG >= 201703L
	for (const auto& [direction, neighborInThatDirection] : mapOfNeighbors) //"structured bindings"
	{
		if (neighborInThatDirection != '#' && neighborInThatDirection != 'V')
		{
			stackOfDirections.push(direction);
			return true;
		}
	}
#else 
	throw std::exception("C++17 is REQUIRED to run this program");
#endif

	//Q: Now what does it mean if all neighbors are either a wall ('#') OR have been previously visited? 
	return false; //It's a dead end!

}

void moveToNextPosition(std::vector<std::vector<char>>& maze, std::pair<int, int>& currentRowAndColumn, const std::string& nextDirection)

{

	int currentRow = currentRowAndColumn.first;
	int currentCol = currentRowAndColumn.second;

	maze[currentRow][currentCol] = 'V';

	if (nextDirection == "Up")
	{
		maze[currentRow - 1][currentCol] = 'C';
		currentRowAndColumn = { currentRow - 1, currentCol };
	}

	else if (nextDirection == "Down")
	{
		maze[currentRow + 1][currentCol] = 'C';
		currentRowAndColumn = { currentRow + 1, currentCol };
	}

	else if (nextDirection == "Left")
	{
		maze[currentRow][currentCol - 1] = 'C';
		currentRowAndColumn = { currentRow, currentCol - 1 };
	}

	else if (nextDirection == "Right")
	{
		if (maze[currentRow][currentCol + 1] == '#') //just a silly, temporary debugging thing
		{
			std::cout << "No busting through walls, Kool-aid man!\n";
			__debugbreak(); 
		}
		maze[currentRow][currentCol + 1] = 'C';
		currentRowAndColumn = { currentRow, currentCol + 1 };
	}

	else
	{
		std::cout << "again, how did you get here?\n";
		__debugbreak();
	}
}

void traverseMaze(std::vector<std::vector<char>> maze)

{
	std::pair<int, int> currentRowAndColumn = getLocationOfCharacterIn2DMaze(maze, 'S');

	std::pair<int, int> goalRowAndColumn = getLocationOfCharacterIn2DMaze(maze, 'E');

	std::stack<std::string> stackOfDirections;

	while (currentRowAndColumn != goalRowAndColumn)
	{
		/*Ordering of neighbors: Down, Left, Right, Up (alphabetized by map key -> string)*/
		std::map<std::string, char> mapOfNeighbors = getMapOfNeighbors(currentRowAndColumn, maze);

		bool isNewMoveAvailable = addNextMoveToStackOfDirections(mapOfNeighbors, stackOfDirections);

		if (isNewMoveAvailable)
		{
			std::cout << "Moving " << stackOfDirections.top() << "\n";
			//std::cin.get(); 
			moveToNextPosition(maze, currentRowAndColumn, stackOfDirections.top());

			printMaze(maze);
			std::cout << "\n\n";

		}

		else
		{
			std::cout << "\t\t\t\tDead end, dead end, dead end!\n";
			bool needToBacktrack = true;
			backtrack(maze, stackOfDirections, currentRowAndColumn, needToBacktrack);
		}

	}

}

