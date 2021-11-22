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