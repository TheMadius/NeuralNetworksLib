#include "CheckersGameAI.h"

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