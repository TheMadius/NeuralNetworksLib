#include "CheckersGameAI.h"

CheckersGameAI::CheckersGameAI(CheckersGame* game, double gamma, Team turnTeam)
{
	this->sol = { 64 * 2, 200, 500, 2000, 4096 };

	this->qmod = new QModel(sol);
	this->game = game;
	this->gamma = gamma;
	this->turn = turnTeam;
}

