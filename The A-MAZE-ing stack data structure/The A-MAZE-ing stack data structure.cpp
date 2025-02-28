//requires C++17

#include<array> 
#include <iostream>
#include <vector>
#include <stack>
#include <map>


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


/*Picks the first neighbor that is not a wall or a 'V' (as in "visited")*/
void addNextMoveToStackOfDirections(const std::map<std::string, char>& mapOfNeighbors, std::stack<std::string>& stackOfDirections)
{
#if _MSVC_LANG >= 201703L
	for (const auto& [direction, neighborInThatDirection]  : mapOfNeighbors) //"structured bindings"
	{
		if (neighborInThatDirection != '#' && neighborInThatDirection != 'V')
		{
			stackOfDirections.push(direction); 
			return; 
		}
	}
#else 
	throw std::exception("C++17 is REQUIRED to run this program");
#endif


	//Q: Now what does it mean if all neighbors are either a wall ('#') OR have been previously visited? 
	//A: Dead end and need to backtrack, I think: 

	//cue the music: "Dead end, dead end, dead end!"


	//stackOfDirections.pop(); 

}

/*Updates the maze by placing a 'V' in previous position and a 'C' in next (new) position */
void moveToNextPosition
	(std::vector<std::vector<char>>& maze, std::pair<int, int>& currentRowAndColumn, const std::string& nextDirection)
{

	int currentRow = currentRowAndColumn.first; 
	int currentCol = currentRowAndColumn.second; 

	maze[currentRow][currentCol] = 'V'; 

	if (nextDirection == "Up")
	{
		maze[currentRow - 1][currentCol] = 'C';
		currentRowAndColumn = { currentRow - 1, currentCol};
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
		maze[currentRow][currentCol + 1] = 'C'; 
		currentRowAndColumn = { currentRow, currentCol + 1 };
	}

	else
	{
		std::cout << "again, how did you get here?\n";
		__debugbreak();
	}
}

/*
@param maze -> passed BY VALUE in anticipation of modifying the copy (printing current position as 'C')
*/
void traverseMaze(std::vector<std::vector<char>> maze)
{
	std::pair<int, int> currentRowAndColumn = getLocationOfCharacterIn2DMaze(maze, 'S');
	
	std::pair<int, int> goalRowAndColumn = getLocationOfCharacterIn2DMaze(maze, 'E'); 

	std::stack<std::string> stackOfDirections; 

	while (currentRowAndColumn != goalRowAndColumn)
	{
		

		//std::array<char, 4> neighbors = getNeighbors(currentRowAndColumn, maze); 
		/*Ordering of neighbors: Down, Left, Right, Up (alphabetized by map key -> string)*/
		std::map<std::string, char> mapOfNeighbors = getMapOfNeighbors(currentRowAndColumn, maze);

		addNextMoveToStackOfDirections(mapOfNeighbors, stackOfDirections);

		std::cout << "Moving " << stackOfDirections.top() << "\n";

		moveToNextPosition(maze, currentRowAndColumn, stackOfDirections.top());

		printMaze(maze);
		std::cout << "\n\n";

		

	}

}

int main()
{
	/*13 rows, 18 columns*/
	std::vector<std::string> stringMaze = 
	{
		"##################",
		"#S   #       #   #",
		"### # ### ### # ##",
		"#   #   #   # #  #",
		"# ### ### # # ## #",
		"#     # # # #    #",
		"##### # # # ######",
		"#   # #   #      #",
		"# # # ### ###### #",
		"# #   #       #  #",
		"##### ######### ##",
		"#   #   #     # E#",
		"##################"
	};

	//int COLUMNS_IN_MAZE = stringMaze[0].size(); 

	std::vector<std::vector<char>> maze = 
		convert1DStringArrayTo2DCharArray(stringMaze);
	
	//std::cout << "Row count: " << maze.size() << "\nColumn count: " << maze[0].size() << "\n";
	//printMaze(maze); 

	try
	{
		traverseMaze(maze); 
	}

	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";
	}


}

