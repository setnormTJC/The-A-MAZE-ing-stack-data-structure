#pragma once
#include<array> 
#include <iostream>
#include <vector>
#include <stack>
#include <map>


std::vector<std::vector<char>> convert1DStringArrayTo2DCharArray(const std::vector<std::string>& stringMaze);

void printMaze(const std::vector<std::vector<char>>& maze);

std::pair<int, int> getLocationOfCharacterIn2DMaze(const std::vector<std::vector<char>>& maze, const char theCharacterToFind);


std::map<std::string, char> getMapOfNeighbors(const std::pair<int, int>& currentRowAndColumn, const std::vector<std::vector<char>>& maze);


std::string getBacktrackingDirection(const std::string& lastDirection);

void backtrack(std::vector<std::vector<char>>& maze, std::stack<std::string>& stackOfDirections,
	std::pair<int, int>& currentRowAndColumn, bool& needToBacktrack);

/*Picks the first neighbor that is not a wall or a 'V' (as in "visited")
* @returns TRUE if a move was found -> FALSE if dead end was reached
*/
bool addNextMoveToStackOfDirections(const std::map<std::string, char>& mapOfNeighbors, std::stack<std::string>& stackOfDirections);

/*Updates the maze by placing a 'V' in previous position and a 'C' in next (new) position */
void moveToNextPosition
(std::vector<std::vector<char>>& maze, std::pair<int, int>& currentRowAndColumn, const std::string& nextDirection);

/*
@param maze -> passed BY VALUE in anticipation of modifying the copy (printing current position as 'C')
*/
void traverseMaze(std::vector<std::vector<char>> maze);