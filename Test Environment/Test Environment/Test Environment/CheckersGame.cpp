#include "CheckersGame.h"

using namespace std;

CheckersGame::CheckersGame()
{

}

void CheckersGame::NewGame()
{
}

GameInfo CheckersGame::GetInfo() const
{
	return GameInfo();
}

const Checker* CheckersGame::GetChosenChecker() const
{
	return nullptr;
}

//qwerty

std::vector<const Checker*> CheckersGame::GetCheckers() const
{
	return std::vector<const Checker*>();
}

std::vector<Coord> CheckersGame::GetPossibleMoves(const Checker* checker) const
{
	return std::vector<Coord>();
}

void CheckersGame::Action(const Coord& coord)
{
}

void CheckersGame::Init()
{

}
