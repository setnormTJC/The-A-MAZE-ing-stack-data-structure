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

void Maze::printMaze() const 
{
	for (const std::vector<char>& rowInMaze : maze)
	{
		for (const char character : rowInMaze)
		{
			if (character == 'C' || character == '2')
				std::cout << "\033[31m" << character << "\033[0m"; // Red color
			else
				std::cout << character;
		}
		std::cout << "\n";
	}
}

std::pair<int, int> Maze::getLocationOfCharacterIn2DMaze( const char theCharacterToFind) const 
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

void Maze::getMapOfNeighbors()
{
	mapOfNeighbors["Down"] = maze[currentRowAndColumn.first + 1][currentRowAndColumn.second];
	mapOfNeighbors["Left"] = maze[currentRowAndColumn.first][currentRowAndColumn.second - 1];
	mapOfNeighbors["Right"] = maze[currentRowAndColumn.first][currentRowAndColumn.second + 1];
	mapOfNeighbors["Up"] = maze[currentRowAndColumn.first - 1][currentRowAndColumn.second];
}

std::string Maze::getBacktrackingDirection(const std::string& lastDirection) const

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

void Maze::backtrack()
{
	if (needToBacktrack == false)
	{
		return;
	}

	while(!stackOfDirections.empty())
	{
		std::string lastDirection = stackOfDirections.top();
		std::string backtrackingDirection = getBacktrackingDirection(lastDirection);
		stackOfDirections.pop();

		//now move, check if still at a dead end, then make recursive call: 
		std::cout << "\t\t\tBACKTRACKING " << backtrackingDirection << "\n"; 

		moveToNextPosition(backtrackingDirection);  
		printMaze(); 

		getMapOfNeighbors();
		bool isNewMoveAvailable = addNextMoveToStackOfDirections();

		if (isNewMoveAvailable)
		{
			needToBacktrack = false;
			return; //we have stepped back far enough that new path is available
		}
	}

	std::cout << "No more moves left. Maze is unsolvable!\n";
	throw std::runtime_error("Maze is unsolvable.");
}

bool Maze::addNextMoveToStackOfDirections()

{

#if _MSVC_LANG >= 201703L
	for (const auto& [direction, neighborInThatDirection] : mapOfNeighbors) //"structured bindings"
	{
		if (neighborInThatDirection == ' ')
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

void Maze::moveToNextPosition( const std::string& nextDirection)

{

	int currentRow = currentRowAndColumn.first;
	int currentCol = currentRowAndColumn.second;

	maze[currentRow][currentCol] = 'V';

	/*unrelated if to that above: */
	if (nextDirection == "Up" )
	{
		currentRowAndColumn = { currentRow - 1, currentCol };
	}
	else if (nextDirection == "Down")
	{
		currentRowAndColumn = { currentRow + 1, currentCol };
	}
	else if (nextDirection == "Left")
	{
		currentRowAndColumn = { currentRow, currentCol - 1 };
	}
	else if (nextDirection == "Right")
	{
		currentRowAndColumn = { currentRow, currentCol + 1 };
	}

	// Mark current position with C if visiting for the first time, else mark with '2'
	

	maze[currentRowAndColumn.first][currentRowAndColumn.second] = 'C';
	
}


void Maze::traverseMaze()

{

	while (currentRowAndColumn != goalRowAndColumn)
	{
		/*Ordering of neighbors: Down, Left, Right, Up (alphabetized by map key -> string)*/
		getMapOfNeighbors();

		bool isNewMoveAvailable = addNextMoveToStackOfDirections();

		if (isNewMoveAvailable)
		{
			std::cout << "Moving " << stackOfDirections.top() << "\n";
	
			rowAndColumnToMoveDirection.insert({ currentRowAndColumn, stackOfDirections.top() });
			//auto previousRowAndColumn = currentRowAndColumn; //for showing direction of movement in image of maze

			moveToNextPosition(stackOfDirections.top());

			mazeImage.drawMaze(maze, rowAndColumnToMoveDirection);

			const char* mazeImageFilename = "mazey.bmp";
			mazeImage.writeImageFile(mazeImageFilename);

			std::cout << "Any key to continue\n";
			std::cin.get();

			system(mazeImageFilename);


			//printMaze();
			//std::cout << "\n\n";

		}

		else
		{
			std::cout << "\t\t\t\tDead end, dead end, dead end!\n";



			needToBacktrack = true;
			backtrack();
		}

	}

}


Maze::Maze(const std::vector<std::string>& stringMaze)
{
	maze = convert1DStringArrayTo2DCharArray(stringMaze);
	currentRowAndColumn = getLocationOfCharacterIn2DMaze('S');
	goalRowAndColumn = getLocationOfCharacterIn2DMaze('E');


	//the image file stuff: 
	mazeImage.setMazeImageDimensions( maze.size(), maze[0].size(), ColorEnum::Cyan);
	

	mazeImage.drawMaze(maze, {});//!

	const char* mazeImageFilename = "mazey.bmp"; 
	mazeImage.writeImageFile(mazeImageFilename); 

	//display starting maze: 
	system(mazeImageFilename);

}

Maze::Maze(const int numberOfDesiredRows, const int numberOfDesiredColumns)
{
	//How do it do it? :)
}
