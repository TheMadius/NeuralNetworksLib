//#pragma once
//#include <iostream>
//#include "CheckersGame.h"
//#include "ModelView.h"
//#include "Game.h"
//#include "RLnet.h"
//#define MAX_X 8
//#define MAX_Y 8
//#define COUNT_TEAM 2
//
//class CheckersGameAI
//{
//public:
//	CheckersGameAI(CheckersGame * game, double gamma, Team turn, double ex = 0.1);
//	void Move(bool train = false);
//
//	~CheckersGameAI();
//private:
//	void MakeMuve(int indexOfArray);
//	RowVector getInputVector();
//	RowVector getLegalVector();
//	int getIndexForArray(int x, int y);
//	Coord getCoord(int indexArr);
//	std::vector<uint32_t> sol;
//	QModel *qmod;
//	CheckersGame* game;
//	double gamma;
//	Team turn;
//};
//
