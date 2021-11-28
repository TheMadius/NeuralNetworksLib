//#pragma once
//#include "SFML/Graphics.hpp"
//#include <time.h>
//#include <iostream>
//#include <string>
//#include "CheckersGame.h"
//
//#define FILD_SIZE 8					// размер игрового поля
//#define CELL_SIZE 66				// Размер ячейки привязан к размеру картинки (чуть больше для создания линий между ячейками)
//#define FONT_SIZE 24				// Размер шрифта
//#define BLACK Color(0, 0, 0)		// Чёрный цвет
//#define WHITE Color(255, 255, 255)	// Белый цвет
//#define TEXT_TYPE Text::Bold		// Тип текста
//#define TEXT_COLOR Color::Black		// Цвет текста
//#define COUNT_CHECKER 12
//
//using namespace sf;
//
//class ModelView
//{
//public:
//	ModelView();
//
//	ModelView(std::string name, int windowHight, int windowWidth);
//
//	~ModelView() { delete windowGame; }
//
//	void Start();
//
//	void ConnectGame(CheckersGame* game) { this->game = game; };
//
//private:
//
//	void Events();
//
//	void SelectСheckers();
//	void GameBoardInit();
//	void GameBoardUpdate();
//	void GameBoardTextUpdate();
//	void GameBoardStaticUpdate();
//	void GameBoardAndСheckersUpdate();
//	void LoadTexturesAndText();
//	void StartNewGame();
//
//	void UpdateFromGame();
//
//	WindowGame* windowGame;
//	CheckersGame* game;
//};