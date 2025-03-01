//requires C++17

#include "Maze.h"

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

