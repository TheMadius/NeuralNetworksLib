#include "CheckersGameAI.h"

CheckersGameAI::CheckersGameAI(CheckersGame* game, double gamma, Team turnTeam , double ex)
{
	this->sol = { 64 * 2, 200, 500, 2000, 4096 };

	this->qmod = new QModel(sol);
	this->game = game;
	this->gamma = gamma;
	this->turn = turnTeam;
	this->probRand = ex;
}

void CheckersGameAI::Move(bool train)
{
	if (game->GetInfo().isEnd)
		return;

	if (game->GetInfo().turn != this->turn)
		return;

	int moveId;
	auto input = getInputVector();
	auto legal_move = getLegalVector();

	if (train)
	{
		RowVector output(sol.back());

		output.setZero();

		double exp = ((double)(rand()))/RAND_MAX;

		if(exp < this->probRand)
			moveId = this->qmod->explore(legal_move);
		else
			moveId = this->qmod->predict(input, legal_move);

		int reward = MakeMuve(moveId) * 100;

		auto next_input = getInputVector();
		auto next_legal_move = getLegalVector();

		auto best_next_reward = this->qmod->forward(next_input, next_legal_move).maxCoeff();

		output.coeffRef(moveId) = reward + this->gamma * best_next_reward;

		this->qmod->train(input, output);
	}
	else
	{
		moveId = this->qmod->predict(input, legal_move);
		MakeMuve(moveId);
	}
}

CheckersGameAI::~CheckersGameAI()
{
	delete qmod;
}

int CheckersGameAI::MakeMuve(int indexOfArray)
{
	Coord coordCheck = getCoord(indexOfArray / 64);
	Coord coordCheckMove = getCoord(indexOfArray % 64);

	auto checkers = game->GetCheckers();

	for (auto& i : checkers)
	{
		if (i->coord == coordCheck)
		{
			auto moves = this->game->PossibleMoves(i);
			for (auto& move : moves)
			{
				if (move.target == coordCheckMove)
				{
					this->game->MakeMove(i, move);
					return move.chopedCheckers.size();
				}
			}
		}
	}
}

RowVector CheckersGameAI::getInputVector()
{
	auto checkers = game->GetCheckers();
	RowVector input_vextor(sol.front());
	input_vextor.setZero();

	for (auto& i : checkers)
	{
		int x = i->coord.x;
		int y = i->coord.y;
		int turn = (int)(i->team);

		input_vextor.coeffRef(getIndexForArray(x , y) * COUNT_TEAM + turn) = 1.0;
	}

	return input_vextor;
}

RowVector CheckersGameAI::getLegalVector()
{
	auto checkers = game->GetCheckers();
	RowVector legal_vextor(sol.back());
	legal_vextor.setZero();

	for (auto& i : checkers)
	{
		if (i->team == this->turn)
		{
			auto moves = this->game->PossibleMoves(i);
			for (auto& move : moves)
			{
				int x_ch = i->coord.x, x_m = move.target.x;
				int y_ch = i->coord.y, y_m = move.target.y;
				int index_ch = (MAX_X * MAX_Y) * getIndexForArray(x_ch, y_ch);
				int index_pos = getIndexForArray(x_m, y_m);

				legal_vextor.coeffRef(index_ch + index_pos) = 1.0;
			}
		}
	}

	return legal_vextor;
}

int CheckersGameAI::getIndexForArray(int x, int y)
{
	return (y - 1) * MAX_X + x - 1;
}

Coord CheckersGameAI::getCoord(int indexArr)
{
	return Coord(indexArr%MAX_Y + 1, indexArr / MAX_Y + 1);
}
