#include "CheckersGameAI.h"

CheckersGameAI::CheckersGameAI(CheckersGame* game, double gamma, Team turnTeam)
{
	this->sol = { 64 * 2, 200, 500, 2000, 4096 };

	this->qmod = new QModel(sol);
	this->game = game;
	this->gamma = gamma;
	this->turn = turnTeam;
}

void CheckersGameAI::Move(bool train)
{
	auto input = getInputVector();
	auto legal_move = getLegalVector();

	int moveId = this->qmod->explore(legal_move);

	Coord coordCheck = getCoord(moveId/64);
	Coord coordCheckMove = getCoord(moveId%64);

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
					return;
				}
			}
		}
	}
}

CheckersGameAI::~CheckersGameAI()
{
	delete qmod;
}
