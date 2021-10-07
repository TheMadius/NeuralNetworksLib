#include "CheckersGame.h"
#include "Log.h"

using namespace std;

CheckersGame::CheckersGame()
{
	chosenChecker = nullptr;

	//Добавляем белые шашки
	for (int i = 0; i < 16; i++) {
		checkers.push_back(Checker(Coord(i/8 + 1, i % 8 + 1), CheckerType::Checker, Team::White));
		Log::Write("Added white checker with coords: x=" + std::to_string(checkers[i].coord.x)
			+ ", y=" + std::to_string(checkers[i].coord.y));
	}

	Log::Write("White checkers was added\n\tCheckers total: " + std::to_string(checkers.size()));
	

	//Добавляем черные шашки
	for (int i = 0; i < 16; i++) {
		checkers.push_back(Checker(Coord(8 - i / 8, i % 8 + 1), CheckerType::Checker, Team::Black));
		Log::Write("Added black checker with coords: x=" + std::to_string(checkers[i + 16].coord.x)
			+ ", y=" + std::to_string(checkers[i + 16].coord.y));
	}

	Log::Write("Black checkers was added\n\tCheckers total: " + std::to_string(checkers.size()));
	

	//Добавляем информацию об игре
	info.isEnd = false;
	info.allCheckers = 32;
	info.blackCheckers = 16;
	info.whiteCheckers = 16;
	info.winner = Team::Black;
	info.countMoves = 0;
}

void CheckersGame::NewGame()
{
	chosenChecker = nullptr;

	//Добавляем белые шашки
	for (int i = 0; i < 16; i++) {
		checkers.push_back(Checker(Coord(i / 8 + 1, i % 8 + 1), CheckerType::Checker, Team::White));
	}

	//Добавляем черные шашки
	for (int i = 0; i < 16; i++) {
		checkers.push_back(Checker(Coord(8 - i / 8, i % 8 + 1), CheckerType::Checker, Team::Black));
	}

	//Добавляем информацию об игре
	info.isEnd = false;
	info.allCheckers = 32;
	info.blackCheckers = 16;
	info.whiteCheckers = 16;
	info.winner = Team::Black;
	info.countMoves = 0;
}

GameInfo CheckersGame::GetInfo() const
{
	return GameInfo();
}

const Checker* CheckersGame::GetChosenChecker(int x, int y) const
{
	bool isFound = false;

	for (int i = 0; i < this->checkers.size(); i++) {
		if (this->checkers[i].coord.x == x && this->checkers[i].coord.y == y) {
			return &(this->checkers[i]);
		}
	}

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
