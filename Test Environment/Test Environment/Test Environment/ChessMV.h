#pragma once
#include "SFML/Graphics.hpp"
#include <time.h>
#include <iostream>
#include <string>
#include "ChessGame.h"

#define FILD_SIZE 8					// размер игрового поля
#define CELL_SIZE 66				// Размер ячейки привязан к размеру картинки (чуть больше для создания линий между ячейками)
#define FONT_SIZE 24				// Размер шрифта
#define BLACK Color(0, 0, 0)		// Чёрный цвет
#define WHITE Color(255, 255, 255)	// Белый цвет
#define TEXT_TYPE Text::Bold		// Тип текста
#define TEXT_COLOR Color::Black		// Цвет текста
#define COUNT_CHECKER 12

using namespace sf;

enum class CellType						//Типы ячейки
{
	Blue,
	White,
	Green,
	Yellow
};

struct TextureAndText
{
	Font font;
	Text* txt;
	Text* txtInfo;

	Texture white;
	Texture blue;
	Texture green;
	Texture yellow;
	Texture newGameButton;

	/// <фигуры>
	Texture PawnWH;
	Texture KnightWH;
	Texture BishopWH;
	Texture RookWH;
	Texture QueenWH;
	Texture KingWH;

	Texture PawnBL;
	Texture KnightBL;
	Texture BishopBL;
	Texture RookBL;
	Texture QueenBL;
	Texture KingBL;
	/// </фигуры>

	Sprite whiteSprite;
	Sprite blueSprite;
	Sprite yellowSprite;
	Sprite greenSprite;
	Sprite newGameButtonSprite;
	/// <фигуры>
	Sprite PawnWHSprite;
	Sprite KnightWHSprite;
	Sprite BishopWHSprite;
	Sprite RookWHSprite;
	Sprite QueenWHSprite;
	Sprite KingWHSprite;

	Sprite PawnBLSprite;
	Sprite KnightBLSprite;
	Sprite BishopBLSprite;
	Sprite RookBLSprite;
	Sprite QueenBLSprite;
	Sprite KingBLSprite;
	/// </фигуры>

	
};

struct Board
{
	const ChessGame::Figure* gridLogic[FILD_SIZE][FILD_SIZE];
	CellType gridView[FILD_SIZE][FILD_SIZE];

	int size = 8;					// Размер игрового поля в плашках
	int arraySize = size * size;	// Размер массива плашек
	int fieldSize = 670;			// Размер игрового поля в пикселях
	int cellSize = 70;				// Размер плашки в пикселях
};

struct WindowGame
{
	std::string name;
	int windowHight;
	int windowWidth;
	RenderWindow* app;
	Event e;
	Board gameBoard;
	TextureAndText gameTextureAndText;
};

class ChessMV
{
public:
	ChessMV();

	ChessMV(std::string name, int windowHight, int windowWidth);

	~ChessMV() { delete windowGame; }

	void Start();

	void ConnectGame(ChessGame* game) { this->game = game; };

private:

	void Events();

	void SelectСheckers();
	void GameBoardInit();
	void GameBoardUpdate();
	void GameBoardTextUpdate();
	void GameBoardStaticUpdate();
	void GameBoardAndСheckersUpdate();
	void LoadTexturesAndText();
	void StartNewGame();

	void UpdateFromGame();

	WindowGame* windowGame;
	ChessGame* game;
};