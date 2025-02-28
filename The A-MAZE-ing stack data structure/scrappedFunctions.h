#pragma once
#include <string>
#include <iostream>

std::string getUpDownLeftOrRight(const int i)
{
	std::string direction;
	switch (i)
	{
	case 0:
		direction = "Up";
		break;
	case 1:
		direction = "Down";
		break;
	case 2:
		direction = "Left";
		break;
	case 3:
		direction = "Right";
		break;

	default:
		std::cout << "How did you get here?\n";
		__debugbreak();
		break;
	}

	return direction;
}
