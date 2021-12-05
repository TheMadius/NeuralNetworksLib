#pragma once
#include <iostream>
#include <random>
#include <numeric>
#include "CheckersGame.h"
#include "ModelView.h"
#include "Game.h"
#include "RLnet.h"
#define MAX_X 8
#define MAX_Y 8
#define COUNT_TEAM 2
#define AVG_STEP 30

class CheckersGameAI
{
public:
	CheckersGameAI(CheckersGame * game, double gamma, Team turn, string file = "", double ex = 0.3);
	void Move(bool train = false);
	void save(string file_name);
	~CheckersGameAI();
private:
	std::vector<RowVector*> getInputData(int sizeBach, std::vector<int> index);
	std::vector<RowVector*> getOutputData(int sizeBach, std::vector<int> index);
	void updata_history(int limit_count);
	int getIndexForArray(int x, int y);
	int MakeMuve(int indexOfArray);
	RowVector* getInputVector();
	RowVector* getLegalVector();
	Coord getCoord(int indexArr);

	std::vector<uint32_t> sol;
	CheckersGame* game;
	double probRand;
	QModel *qmod;
	double gamma;
	Team turn;

	std::vector<int> history_action;
	std::vector<int> history_reward;
	std::vector<RowVector*> history_state;
	std::vector<RowVector*> history_legal;
	std::vector<RowVector*> history_next_state;
	std::vector<RowVector*> history_next_legal;
};

