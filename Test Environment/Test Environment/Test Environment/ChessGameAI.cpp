#include "ChessGameAI.h"

static std::vector<int> getRandomVec(int size)
{
	std::vector<int> x(size);
	std::iota(x.begin(), x.end(), 0);

	std::random_shuffle(std::begin(x), std::end(x));

	return x;
}

ChessGameAI::ChessGameAI(ChessGame* game, double gamma, ChessGame::Team turnTeam, string file, double ex)
{
	this->sol = { 64 * COUNT_TEAM * COUNT_FIG, 1000, 2000, 4096 };

	this->qmod = new QModel(sol, 0.01, file);
	this->game = game;
	this->gamma = gamma;
	this->turn = turnTeam;
	this->probRand = ex;
}

void ChessGameAI::Move(bool train)
{
	if (game->GetInfo().isEnd)
		return;

	if (game->GetInfo().turn != this->turn)
		return;

	auto input = getInputVector();
	auto legal_move = getLegalVector();

	if (train)
	{
		int moveId;
		this->history_legal.push_back(legal_move);
		this->history_state.push_back(input);

		if ((((double)(rand())) / RAND_MAX) < this->probRand)
			moveId = this->qmod->explore(*legal_move);
		else
			moveId = this->qmod->predict(*input, *legal_move);

		this->history_action.push_back(moveId);
		this->history_reward.push_back(MakeMuve(moveId) * 10000);
		this->history_next_state.push_back(getInputVector());
		this->history_next_legal.push_back(getLegalVector());

		updata_history(AVG_STEP);

		auto index = getRandomVec(this->history_state.size());
		auto inputData = getInputData(6, index);
		auto outputData = getOutputData(6, index);

		this->qmod->train(inputData, outputData);

		for (auto i : outputData)
			delete i;
	}
	else
	{
		int moveId;
		moveId = this->qmod->predict(*input, *legal_move);
		MakeMuve(moveId);

		delete input;
		delete legal_move;
	}
}

void ChessGameAI::save(string file_name)
{
	this->qmod->saveModel(file_name);
}

ChessGameAI::~ChessGameAI()
{
	delete qmod;
	for (auto item : history_legal)
		delete item;
	for (auto item : history_next_legal)
		delete item;
	for (auto item : history_next_state)
		delete item;
	for (auto item : history_state)
		delete item;
}

int ChessGameAI::MakeMuve(int indexOfArray)
{
	ChessGame::Coord coordCheck = getCoord(indexOfArray / 64);
	ChessGame::Coord coordCheckMove = getCoord(indexOfArray % 64);

	auto checkers = game->GetFigures();

	for (auto& i : checkers)
	{
		if (i->coord == coordCheck)
		{
			auto moves = this->game->PossibleMoves(i);
			for (auto& move : moves)
			{
				if (move.target == coordCheckMove)
				{
					this->game->MakeMove(move);
					return 0;
				}
			}
		}
	}
}

RowVector* ChessGameAI::getInputVector()
{
	auto checkers = game->GetFigures();
	RowVector* input_vextor = new RowVector(sol.front());
	input_vextor->setZero();

	for (auto& i : checkers)
	{
		int x = i->coord.x;
		int y = i->coord.y;
		int turn = (int)(i->team);
		int type = (int)(i->type);

		input_vextor->coeffRef(getIndexForArray(x, y) * COUNT_TEAM * COUNT_FIG + COUNT_FIG * turn + type) = 1.0;
	}

	return input_vextor;
}

RowVector* ChessGameAI::getLegalVector()
{
	auto checkers = game->GetFigures();
	RowVector* legal_vextor = new RowVector(sol.back());
	legal_vextor->setZero();

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

				legal_vextor->coeffRef(index_ch + index_pos) = 1.0;
			}
		}
	}

	return legal_vextor;
}


std::vector<RowVector*> ChessGameAI::getInputData(int sizeBach, std::vector<int> index)
{
	std::vector<RowVector*> input;
	if (index.size() < sizeBach)
		sizeBach = index.size();

	for (int i = 0; i < sizeBach; i++)
		input.push_back(this->history_state[index[i]]);

	return input;
}

std::vector<RowVector*> ChessGameAI::getOutputData(int sizeBach, std::vector<int> index)
{
	std::vector<RowVector*> output;

	if (index.size() < sizeBach)
		sizeBach = index.size();

	for (int i = 0; i < sizeBach; i++)
	{
		RowVector* updated_q_values = new RowVector(sol.back());
		updated_q_values->setZero();
		int id = index[i];

		auto best_next_reward = this->qmod->forward(this->history_next_state[id], this->history_next_legal[id]).maxCoeff();
		updated_q_values->coeffRef(this->history_action[id]) = this->history_reward[id] + this->gamma * best_next_reward;

		output.push_back(updated_q_values);
	}

	return output;
}

int ChessGameAI::getIndexForArray(int x, int y)
{
	return (y - 1) * MAX_X + x - 1;
}

ChessGame::Coord ChessGameAI::getCoord(int indexArr)
{
	return ChessGame::Coord(indexArr % MAX_Y + 1, indexArr / MAX_Y + 1);
}

void ChessGameAI::updata_history(int limit_count)
{
	if (this->history_action.size() > limit_count)
	{
		history_action.erase(history_action.begin());
		history_reward.erase(history_reward.begin());

		delete history_legal.front();
		delete history_next_legal.front();
		delete history_next_state.front();
		delete history_state.front();

		history_legal.erase(history_legal.begin());
		history_next_legal.erase(history_next_legal.begin());
		history_next_state.erase(history_next_state.begin());
		history_state.erase(history_state.begin());
	}
}

