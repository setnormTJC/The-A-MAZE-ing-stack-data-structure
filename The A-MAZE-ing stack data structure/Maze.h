#pragma once

#include "ImageBMP.h"


#include <stack>


class Maze
{
private: 


	/*******************************Private vars:***************************/
	std::vector<std::vector<char>> maze; 
	std::pair<int, int> currentRowAndColumn;
	std::pair<int, int> goalRowAndColumn;

	std::stack<std::string> stackOfDirections;
	std::map<std::string, char> mapOfNeighbors;

	/*Ex: moved Right from (1, 1)*/
	std::map< std::pair<int, int>, std::string> rowAndColumnToMoveDirection; 

	bool needToBacktrack = false; 

	//For visualization (and fun): 
	MazeImageBMP mazeImage;

	/*******************************Private funcs:***************************/
	std::pair<int, int> getLocationOfCharacterIn2DMaze( const char theCharacterToFind) const;
	std::string getBacktrackingDirection(const std::string& lastDirection) const;

	void getMapOfNeighbors();
	void backtrack();

	/*Picks the first neighbor that is not a wall or a 'V' (as in "visited")
	* @returns TRUE if a move was found -> FALSE if dead end was reached
	*/
	bool addNextMoveToStackOfDirections();

	/*Updates the maze by placing a 'V' in previous position and a 'C' in next (new) position 
	* @param -> symbolOfVisitation = 'V' if first visit, = '2' if revisiting 
	*/
	void moveToNextPosition(const std::string& nextDirection);

public: 
	Maze() = delete;
	/*Converts the argument `stringMaze` (a 1D array) to a the private member var `maze` (a 2D char array)*/
	Maze(const std::vector<std::string>& stringMaze);
	/*The idea with this constructor is to generate some "random" (but SOLVABLE) maze for the client - using some fancy algorithm*/
	Maze(const int numberOfDesiredRows, const int numberOfDesiredColumns);

	void printMaze() const;

	/*
	@param maze -> passed BY VALUE in anticipation of modifying the copy (printing current position as 'C')
	*/
	void traverseMaze();

};

/*Non-member, helper method:*/
std::vector<std::vector<char>> convert1DStringArrayTo2DCharArray(const std::vector<std::string>& stringMaze);