//requires C++17

#include "Maze.h"

int main()
{
	/*There are pros and cons of 1D string array versus 2D char array*/
	//std::vector<std::string> stringMaze =
	//{
	//	"TTTTTTTTTTTTTTTTTT",
	//	"LS           M   R",
	//	"LMM M MMM MMM M MR",
	//	"L   M   M   M M  R",
	//	"L MMM MMM M M MM R",
	//	"L     M M M M    R",
	//	"LMMMM M M M MMMMMR",
	//	"L   M M   M      R",
	//	"L M M MMM MMMMMM R",
	//	"L M   M       M  R",
	//	"LMMMM MMMMMMMMM MR",
	//	"L   M   M     M ER",
	//	"BBBBBBBBBBBBBBBBBB"
	//};

	std::vector<std::string> stringMaze =
	{
		"WWWWWWWWWWWWWWWWWW",
		"WS           W   W",
		"WWWW W WWW WWW W W",
		"W   W   W   W W  W",
		"W WWW WWW W W WW W",
		"W     W W W      W",
		"WWWW W   WWWWWWW W",
		"W     W   W      W",
		"W W W WWW WWWWWW W",
		"W W           W  W",
		"WWWW WWWWWWWWWWW W",
		"W             WWEW",
		"WWWWWWWWWWWWWWWWWW"
	};

	try
	{
		Maze maze(stringMaze); 

		maze.traverseMaze(); 
	}

	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";
	}


}

