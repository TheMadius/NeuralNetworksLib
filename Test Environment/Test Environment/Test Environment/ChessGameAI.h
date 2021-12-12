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
#define AVG_STEP 50

class ChessGameAI
{
public:
	ChessGameAI(ChessGame* game, double gamma, ChessGame::Team turn, string file = "", double ex = 0.3);
	void Move(bool train = false);
	void save(string file_name);
	void setOpponent(const ChessGameAI* op);
	~ChessGameAI();
private:
	std::vector<const vector<double>*> getInputData(int sizeBach, std::vector<int> index, const ChessGameAI* ai);
	std::vector<const vector<double>*> getOutputData(int sizeBach, std::vector<int> index, const ChessGameAI* ai);

	ChessGame::Coord getCoord(int indexArr);
	void updata_history(int limit_count);
	void _train(const ChessGameAI* ai);
	int getIndexForArray(int x, int y);
	vector<double>* getInputVector();
	vector<double>* getLegalVector();
	int MakeMuve(int indexOfArray);

	std::vector<uint32_t> sol;
	ChessGame::Team turn;
	ChessGame* game;
	double probRand;
	QModel* qmod;
	double gamma;
	const ChessGameAI* _op;

	std::vector<int> history_action;
	std::vector<int> history_reward;
	std::vector<const vector<double>*> history_state;
	std::vector<const vector<double>*> history_legal;
	std::vector<const vector<double>*> history_next_state;
	std::vector<const vector<double>*> history_next_legal;
};

