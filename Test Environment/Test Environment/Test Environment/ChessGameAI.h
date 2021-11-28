#pragma once
#include <iostream>
#include <random>
#include <numeric>
#include "ChessGame.h"
#include "ModelView.h"
#include "Game.h"
#include "RLnet.h"
#define MAX_X 8
#define MAX_Y 8
#define COUNT_TEAM 2
#define COUNT_FIG 6
#define AVG_STEP 30


class ChessGameAI
{
public:
	ChessGameAI(ChessGame* game, double gamma, ChessGame::Team turn, double ex = 0.3);
	void Move(bool train = false);
	~ChessGameAI();
private:
	std::vector<RowVector*> getInputData(int sizeBach, std::vector<int> index);
	std::vector<RowVector*> getOutputData(int sizeBach, std::vector<int> index);
	void updata_history(int limit_count);
	int getIndexForArray(int x, int y);
	int MakeMuve(int indexOfArray);
	RowVector* getInputVector();
	RowVector* getLegalVector();
	ChessGame::Coord getCoord(int indexArr);

	std::vector<uint32_t> sol;
	ChessGame* game;
	double probRand;
	QModel* qmod;
	double gamma;
	ChessGame::Team turn;

	std::vector<int> history_action;
	std::vector<int> history_reward;
	std::vector<RowVector*> history_state;
	std::vector<RowVector*> history_legal;
	std::vector<RowVector*> history_next_state;
	std::vector<RowVector*> history_next_legal;

};

