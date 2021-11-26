#pragma once
#include "SFML/Graphics.hpp"
#include <time.h>
#include <iostream>
#include <string>
#include "CheckersGame.h"

#define FILD_SIZE 8					// ‡ÁÏÂ Ë„Ó‚Ó„Ó ÔÓÎˇ
#define CELL_SIZE 66				// –‡ÁÏÂ ˇ˜ÂÈÍË ÔË‚ˇÁ‡Ì Í ‡ÁÏÂÛ Í‡ÚËÌÍË (˜ÛÚ¸ ·ÓÎ¸¯Â ‰Îˇ ÒÓÁ‰‡ÌËˇ ÎËÌËÈ ÏÂÊ‰Û ˇ˜ÂÈÍ‡ÏË)
#define FONT_SIZE 24				// –‡ÁÏÂ ¯ËÙÚ‡
#define BLACK Color(0, 0, 0)		// ◊∏Ì˚È ˆ‚ÂÚ
#define WHITE Color(255, 255, 255)	// ¡ÂÎ˚È ˆ‚ÂÚ
#define TEXT_TYPE Text::Bold		// “ËÔ ÚÂÍÒÚ‡
#define TEXT_COLOR Color::Black		// ÷‚ÂÚ ÚÂÍÒÚ‡
#define COUNT_CHECKER 12

using namespace sf;

enum class CellType						//“ËÔ˚ ˇ˜ÂÈÍË
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
	Texture black—hecker;
	Texture white—hecker;
	Texture black—heckerDM;
	Texture white—heckerDM;
	Texture newGameButton;

	Sprite whiteSprite;
	Sprite blueSprite;
	Sprite yellowSprite;
	Sprite greenSprite;
	Sprite black—heckerSprite;
	Sprite white—heckerSprite;
	Sprite black—heckerSpriteDM;
	Sprite white—heckerSpriteDM;
	Sprite newGameButtonSprite;
};

struct Board
{
	const Checker* gridLogic[FILD_SIZE][FILD_SIZE];
	CellType gridView[FILD_SIZE][FILD_SIZE];

	int size = 8;					// –‡ÁÏÂ Ë„Ó‚Ó„Ó ÔÓÎˇ ‚ ÔÎ‡¯Í‡ı
	int arraySize = size * size;	// –‡ÁÏÂ Ï‡ÒÒË‚‡ ÔÎ‡¯ÂÍ
	int fieldSize = 670;			// –‡ÁÏÂ Ë„Ó‚Ó„Ó ÔÓÎˇ ‚ ÔËÍÒÂÎˇı
	int cellSize = 70;				// –‡ÁÏÂ ÔÎ‡¯ÍË ‚ ÔËÍÒÂÎˇı
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

class ModelView
{
public:
	ModelView();

	ModelView(std::string name, int windowHight, int windowWidth);

	~ModelView() { delete windowGame; }

	void Start();

	void ConnectGame(CheckersGame* game) { this->game = game; };

private:

	void Events();

	void Select—heckers();
	void GameBoardInit();
	void GameBoardUpdate();
	void GameBoardTextUpdate();
	void GameBoardStaticUpdate();
	void GameBoardAnd—heckersUpdate();
	void LoadTexturesAndText();
	void StartNewGame();

	void UpdateFromGame();

	WindowGame* windowGame;
	CheckersGame* game;
};