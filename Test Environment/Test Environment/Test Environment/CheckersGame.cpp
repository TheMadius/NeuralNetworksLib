#include "CheckersGame.h"
#include "Log.h"

using namespace std;

void CheckersGame::InitializeGame(CheckersGame* game) {
	game->chosenChecker = nullptr;

	game->checkers.clear();
	//Add white checkers
	for (int i = 0; i < 16; i++) {
		game->checkers.push_back(Checker(Coord(i / 8 + 1, i % 8 + 1), CheckerType::Checker, Team::White));
	}

	//Logging part
	Log::Write("White checkers was added\n\tCheckers total: " + std::to_string(checkers.size()));
	//Logging part end


	//Add black checkers
	for (int i = 0; i < 16; i++) {
		game->checkers.push_back(Checker(Coord(8 - i / 8, i % 8 + 1), CheckerType::Checker, Team::Black));
	}

	//logging part
	Log::Write("Black checkers was added\n\tCheckers total: " + std::to_string(checkers.size()));
	//Logging part end

	//Add info about game
	game->info.isEnd = false;
	game->info.allCheckers = 32;
	game->info.blackCheckers = 16;
	game->info.whiteCheckers = 16;
	game->info.winner = Team::Black;
	game->info.countMoves = 0;

	//Loging part
	Log::Write("Initialized game info");
	Log::Write("Game created");
	//Logging part end
}

CheckersGame::CheckersGame()
{
	InitializeGame(this);
}

void CheckersGame::NewGame()
{
	InitializeGame(this);
}

GameInfo CheckersGame::GetInfo() const
{
	return GameInfo();
}

const Checker* CheckersGame::GetChosenChecker(int x, int y) const
{
	bool isFound = false;

	//Logging part
	Log::Write("Called checker getter");
	Log::Write("Position: x=" + std::to_string(x) + "; y=" + std::to_string(y));
	//Logging part end

	for (int i = 0; i < this->checkers.size(); i++) {
		if (this->checkers[i].coord.x == x && this->checkers[i].coord.y == y) {

			//Logging part
			string team = (this->checkers[i].team == Team::Black) ? "Black" : "White";
			Log::Write("Checker was found. Team: " + team);
			//Logging part end

			return &(this->checkers[i]);
		}
	}

	//Logging part
	Log::Write("Checker not found");
	//Logging part end

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
