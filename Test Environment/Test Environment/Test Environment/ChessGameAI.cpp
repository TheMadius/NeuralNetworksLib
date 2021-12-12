#include "ChessGameAI.h"

static std::vector<int> getRandomVec(int size)
{
	std::vector<int> x(size);
	std::iota(x.begin(), x.end(), 0);

	std::random_shuffle(std::begin(x), std::end(x));

	return x;
}

void ChessGameAI::setOpponent(const ChessGameAI* op)
{
	this->_op = op;
}

ChessGameAI::ChessGameAI(ChessGame* game, double gamma, ChessGame::Team turnTeam, string file, double ex)
{
	this->sol = { 64 * COUNT_TEAM * COUNT_FIG, 1000, 2000, 4096 };

	this->qmod = new QModel(sol, 0.01, file);
	this->game = game;
	this->gamma = gamma;
	this->turn = turnTeam;
	this->probRand = ex;
	this->_op = nullptr;
}

void ChessGameAI::Move(bool train)
{
	if (game->GetInfo().isEnd)
		return;

	if (game->GetInfo().turn != this->turn)
		return;

	auto input = getInputVector();
	auto legal_move = getLegalVector();

	int moveId;
	this->history_legal.push_back(legal_move);
	this->history_state.push_back(input);

	bool isRand = (((double)(rand())) / RAND_MAX) < this->probRand;

	if (isRand && train)
		moveId = this->qmod->explore(*legal_move);
	else
		moveId = this->qmod->predict(*input, *legal_move);

	this->history_action.push_back(moveId);
	this->history_reward.push_back(MakeMuve(moveId));
	this->history_next_state.push_back(getInputVector());
	this->history_next_legal.push_back(getLegalVector());

	updata_history(AVG_STEP);

	if (train)
	{
		_train(this);

		if (this->_op != nullptr)
			_train(this->_op);
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
	auto rewards = [](ChessGame::FigureType type){
		switch (type){
		case ChessGame::FigureType::Pawn:return 100;case ChessGame::FigureType::Knight:return 300;
		case ChessGame::FigureType::Bishop:return 300;case ChessGame::FigureType::Rook:return 500;
		case ChessGame::FigureType::Queen:return 1000;case ChessGame::FigureType::King:return 10000;default:return 0;}
	};

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
					int reward = 0;
					if (move.chopedFigure != nullptr)
						reward = rewards(move.chopedFigure->type);

					this->game->MakeMove(move);

					if (this->game->GetInfo().isEnd)
						reward += 10000;

					return reward;
				}
			}
		}
	}
}

vector<double>* ChessGameAI::getInputVector()
{
	auto checkers = game->GetFigures();
	vector<double>* legal_vector = new vector<double>(sol.front(), 0);

	for (auto& i : checkers)
	{
		int x = i->coord.x;
		int y = i->coord.y;
		int turn = (int)(i->team);
		int type = (int)(i->type);

		(*legal_vector)[getIndexForArray(x, y) * COUNT_TEAM * COUNT_FIG + COUNT_FIG * turn + type] = 1.0;
	}

	return legal_vector;
}

vector<double>* ChessGameAI::getLegalVector()
{
	auto checkers = game->GetFigures();
	vector<double>* legal_vector = new vector<double>(sol.back(), 0);

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

				(*legal_vector)[index_ch + index_pos] = 1.0;
			}
		}
	}

	return legal_vector;
}


std::vector<const vector<double>*> ChessGameAI::getInputData(int sizeBach, std::vector<int> index, const ChessGameAI* ai)
{
	std::vector<const vector<double>*> input;
	if (index.size() < sizeBach)
		sizeBach = index.size();

	for (int i = 0; i < sizeBach; i++)
		input.push_back(ai->history_state[index[i]]);

	return input;
}

std::vector<const vector<double>*> ChessGameAI::getOutputData(int sizeBach, std::vector<int> index, const ChessGameAI* ai)
{
	int reward = (ai == this) ? 1 : -1;

	std::vector<const vector<double>*> output;

	if (index.size() < sizeBach)
		sizeBach = index.size();

	for (int i = 0; i < sizeBach; i++)
	{
		int id = index[i];
		vector<double>* updated_q_values = new vector<double>(sol.back(), 0);
		auto best_next_rewards = ai->qmod->forward(ai->history_next_state[id], ai->history_next_legal[id]);
		double best_next_reward = *(std::max_element(best_next_rewards.begin(), best_next_rewards.end()));

		(*updated_q_values)[ai->history_action[id]] = reward * ai->history_reward[id] + ai->gamma * best_next_reward;

		output.push_back(updated_q_values);
	}

	return output;
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

void ChessGameAI::_train(const ChessGameAI* ai)
{
	auto index = getRandomVec(ai->history_state.size());
	auto inputData = getInputData(7, index, ai);
	auto outputData = getOutputData(7, index, ai);

	this->qmod->train(inputData, outputData);

	for (auto i : outputData)
		delete i;
}

int ChessGameAI::getIndexForArray(int x, int y)
{
	return (y - 1) * MAX_X + x - 1;
}

ChessGame::Coord ChessGameAI::getCoord(int indexArr)
{
	return ChessGame::Coord(indexArr % MAX_Y + 1, indexArr / MAX_Y + 1);
}