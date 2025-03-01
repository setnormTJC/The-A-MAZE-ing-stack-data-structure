#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

class FileHeader
{
	/*will make PRIVATE all of the bmp fields that (probably) never change
	- ex: 1st two bytes should always be B and M*/

	std::array<char, 2> filetype = { 'B', 'M' }; //indices 0 and 1  
	//const char* filetype = "BM"; //indices 0 and 1  -> problematic type!

	unsigned int reserved1And2 = 0x00'00'00'00; //(unclear meaning) indices 6 - 9 
	unsigned int indexOfPixelData = 0x00'00'00'36; //indices 10 - 13

public:
	unsigned int fileSize{}; //indices 2 - 5 (size is in bytes)
	//should perhaps be set by the ImageBMP class //(since ImageBMP will have info on pixel data and infoheader) 

	FileHeader() = default;

	friend class ImageBMP;
	friend class MazeImageBMP;

};

class InfoHeader
{
	unsigned int infoHeaderSize = 0x00'00'00'28; //indices 14 - 17, in bytes
	short planes = 0x00'01; //indices 26 - 27 ["always" 1 (meaning unclear)] 

	short bitsPerPixel = 0x00'20; //CAREFUL! not always 32! -> indices 28 - 29 (32 bits - 24 for G,B,R, and 8 for Alpha)
	unsigned int compressionMethod = 0x00'00'00'00; //indices 30 - 33
	unsigned int sizeOfPixelData{}; //indices 34 - 37
	std::vector<int> remainingHeaderFields =
	{
		0x00'00'00'00,//xPixelsPerMeter
		0x00'00'00'00,//yPixelsPerMeter
		0x00'00'00'00,//color indexing thing
		0x00'00'00'00//"important" color count 
	};

public:
	unsigned int imageWidth = 0; //indices 18 - 21
	unsigned int imageHeight = 0; //indices 22 - 25

	InfoHeader() = default;

	unsigned int getInfoHeaderSize() const;
	unsigned int getSizeOfPixelData() const;

	friend class ImageBMP;
	friend class MazeImageBMP;

};

/*NOTE: little-endian BGRA order is used here*/
enum class ColorEnum : unsigned int
{
	//A, R, G, B
	Black = 0xFF'00'00'00,
	White = 0xFF'FF'FF'FF,

	Red = 0xFF'FF'00'00,
	Green = 0xFF'00'FF'00,
	Blue = 0xFF'00'00'FF,

	Yellow = 0xFF'FF'FF'00,
	Cyan = 0xFF'00'FF'FF,
	Magenta = 0xFF'FF'00'FF,

	//chessboard-specific colors: 
	DarkSquareColor = 0xFF'BA'61'34,
	LightSquareColor = 0xFF'EF'D7'B5,
	BoardBorder = 0xFF'6C'1E'1C,

	//using some images from this page: https://commons.wikimedia.org/wiki/Category:Chess_bitmap_pieces
	WKnightBgrdColor = 0xFF'FF'FF'CB,

	RedBgrd = 0xFF'ED'1C'24
};

struct Color
{
	unsigned int bgra = 0x00'00'00'00;

	Color() = default;
	Color(unsigned int bgra);
	Color(unsigned int b, unsigned int g, unsigned int r);
	Color(unsigned int b, unsigned int g, unsigned int r, unsigned int a);
	Color(ColorEnum colorEnum);

	unsigned int convertToUnsignedInt();
};

class PixelData
{
public:
	std::vector<std::vector<Color>> pixelMatrix;

	PixelData() = default;
};

class ImageBMP
{
	void readFileHeaderFromFile(std::ifstream& fin);
	void readInfoHeaderFromFile(std::ifstream& fin);
	void readPixelDataFromFile(std::ifstream& fin);
public:
	FileHeader fileHeader;
	InfoHeader infoHeader;
	PixelData pixelData;

	ImageBMP() = default;

	ImageBMP(unsigned int imageWidth, unsigned int imageHeight, const Color& fillColor, const Color& middleDotColor);

	ImageBMP(unsigned int imageWidth, unsigned int imageHeight, const Color& fillColor);

	ImageBMP(const std::string& filepath);


	void readImageBMP(std::string inputFilename);

	void doublescaleImageBMP();

	void drawRectangleOutline(unsigned int x0, unsigned int y0,
		unsigned int rectangleWidth, unsigned int rectangleHeight, const Color& color);

	void fillRectangleWithColor(unsigned int x0, unsigned int y0,
		unsigned int rectangleWidth, unsigned int rectangleHeight, const Color& color);

	void setPixelToColor_withThickness(unsigned int x, unsigned int y, const Color& color, unsigned int thickness);

	void drawLine(int x0, int y0, int xf, int yf, const Color& color);

	void writeImageFile(std::string filename);
};

class MazeImageBMP : public ImageBMP
{
private: 
	const int PIXELS_PER_MAZE_STEP = 75; 

	ImageBMP stickMan; 
	/*Anticipate upArrow getting rotated to appropriate direction*/
	ImageBMP upArrow; 

public: 
	/*reads in the stickman.bmp image*/
	MazeImageBMP();

	//MazeImageBMP(const int numberOfRowsInMaze, const int numberOfColumnsInMaze);
	void setMazeImageDimensions(const int numberOfRowsInMaze, const int numberOfColumnsInMaze, const ColorEnum& fillColor);

	void drawMaze(const std::vector<std::vector<char>>& maze, const std::map< std::pair<int, int>, std::string>& rowAndColumnToMoveDirection);

	void drawStickFigure(int x0, int y0);

	/*The upArrow member variable is, shockingly, oriented UP by default. @param arrowDirection -> a single counter clockwise rotation gets applied if this is "Left" */
	void drawArrow(int x0, int y0, const std::string& arrowDirection);


};

#pragma region auxiliary functions 
std::vector<std::vector<char>> rotateMatrixClockwise
(std::vector<std::vector<char>>& originalMatrix, int originalNumberOfRows, int originalNumberOfCols);

std::vector<std::vector<int>> rotateIntMatrixClockwise(std::vector<std::vector<int>>& originalMatrix, int originalNumberOfRows, int originalNumberOfCols);

std::vector<std::vector<Color>> rotateColorMatrixCounterClockwise(std::vector<std::vector<Color>>& originalMatrix, int originalNumberOfRows, int originalNumberOfCols);

#pragma endregion
