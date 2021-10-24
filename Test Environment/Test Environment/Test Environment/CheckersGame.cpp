#include "CheckersGame.h"
#include "Log.h"
#include <algorithm>
#include <numeric>

using namespace std;

void CheckersGame::InitializeGame(CheckersGame* game) 
{
	game->turnTeam = Team::White;
	game->chosenChecker = nullptr;

	game->checkers.clear();
	//Add white checkers
	for (int i = 1; i <= 8; i++)
		for (int j = 1; j <= 3; j++)
			if ((i + j) % 2 != 0)
				game->checkers.push_back(new Checker(Coord(i, j), CheckerType::Checker, Team::White));

	//Logging part
	Log::Write("White checkers was added\tCheckers total: " + std::to_string(checkers.size()));
	//Logging part end


	//Add black checkers
	for (int i = 1; i <= 8; i++)
		for (int j = 6; j <= 8; j++)
			if ((i + j) % 2 != 0)
				game->checkers.push_back(new Checker(Coord(i, j), CheckerType::Checker, Team::Black));

	//logging part
	Log::Write("Black checkers was added\tCheckers total: " + std::to_string(checkers.size()));
	//Logging part end

	//Add info about game
	game->info.isEnd = false;
	game->info.allCheckers = 24;
	game->info.blackCheckers = 12;
	game->info.whiteCheckers = 12;
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
	return info;
}

const Checker* CheckersGame::GetChosenChecker() const
{
	return chosenChecker;
}

Checker* CheckersGame::CheckerByCoords(const Coord& coord)
{
	bool isFound = false;

	//Logging part
	//Log::Write("Called checker getter");
	//Log::Write("Position: x=" + std::to_string(coord.x) + "; y=" + std::to_string(coord.y));
	//Logging part end

	for (int i = 0; i < this->checkers.size(); i++) 
	{
		if (checkers[i]->coord.x == coord.x && checkers[i]->coord.y == coord.y) 
		{

			//Logging part
			string team = (this->checkers[i]->team == Team::Black) ? "Black" : "White";
			//Log::Write("Checker was found. Team: " + team);
			//Logging part end

			return checkers[i];
		}
	}

	//Logging part
	//Log::Write("Checker not found");
	//Logging part end

	return nullptr;
}

std::vector<const Checker*> CheckersGame::GetCheckers() const
{
	vector<const Checker*> ret;
	for (int i = 0; i < checkers.size(); i++)
		ret.push_back(checkers[i]);
	return ret;
}

std::vector<Coord> CheckersGame::PossibleMoves(const Checker* checker)
{
	if (checker == nullptr)
		return vector<Coord>();

	return GetPossibleMovesRecursive(*checker, Coord(0, 0));
}

std::vector<Coord> CheckersGame::GetPossibleMovesRecursive(const Checker& checker, const Coord& dir)
{
	vector<Coord> ret;
	int dy = (checker.team == Team::Black) ? (-1) : (1);
	Coord lu = Coord(-1, dy), ru = Coord(1, dy), ld = Coord(-1, -dy), rd = Coord(1, -dy);

	auto tryMove = [&](const Coord& coord, const Coord& dir)
	{
		if (coord == -dir)
			return;
		if (!CoordIsEmpty(checker.coord + coord) && CoordsInField(checker.coord + coord))
			if (CheckerByCoords(checker.coord + coord)->team != checker.team && CoordIsEmpty(checker.coord + coord * 2))
			{
				ret.push_back(checker.coord + coord * 2);
				vector<Coord> tmp = GetPossibleMovesRecursive(
					Checker(checker.coord + coord * 2, checker.type, checker.team), coord);
				ret.insert(ret.end(), tmp.begin(), tmp.end());
			}
	};

	//not chop moves
	if (dir == Coord(0, 0))
	{
		if (CoordIsEmpty(checker.coord + Coord(1, dy)))
			ret.push_back(checker.coord + Coord(1, dy));
		if (CoordIsEmpty(checker.coord + Coord(-1, dy)))
			ret.push_back(checker.coord + Coord(-1, dy));
	}
	//chop moves
	tryMove(ru, dir);
	tryMove(rd, dir);
	tryMove(ld, dir);
	tryMove(lu, dir);
	return ret;
}

void CheckersGame::ChopCheckers(const Checker& checker, const Coord& coord, const std::vector<Coord>& moves)
{
	auto findMove = [](const vector<Coord>& vect, Coord coord)
	{
		return find(vect.begin(), vect.end(), coord) != vect.end();
	};

	int dy = (checker.team == Team::Black) ? (-1) : (1);
	Coord lastDir = Coord(0, 0);
	Coord tmp = coord, lu = Coord(-1, dy), ru = Coord(1, dy), ld = Coord(-1, -dy), rd = Coord(1, -dy);
	vector<Checker*> delCheckers;

	auto backStep = [&](const Coord& coord)
	{
		if (coord == -lastDir)
			return false;
		Checker* midChecker = CheckerByCoords(tmp + coord);
		bool isMidEnemy = midChecker != nullptr && midChecker->team != checker.team;
		if (isMidEnemy && (findMove(moves, tmp + coord * 2) || tmp + coord * 2 == checker.coord))
		{
			delCheckers.push_back(midChecker);
			tmp = tmp + coord * 2;
			lastDir = coord;
			return true;
		}
		return false;
	};

	if (abs((coord - checker.coord).x) == 1)
		return;

	while (true)
	{
		if (tmp == checker.coord)
			break;
		if (backStep(ru))
			continue;
		if (backStep(rd))
			continue;
		if (backStep(ld))
			continue;
		if (backStep(lu))
			continue;
		throw exception("Can't build chop checkers way");
	}
	for (int i = 0; i < delCheckers.size(); i++)
	{
		auto t = find(checkers.begin(), checkers.end(), delCheckers[i]);
		*t = nullptr;
		checkers.erase(t);
		delete delCheckers[i];
	}
}

bool CheckersGame::CoordsInField(const Coord& coord)
{
	return coord.x <= 8 && coord.x >= 1 && coord.y <= 8 && coord.y >= 1;
}

bool CheckersGame::CoordIsEmpty(const Coord& coord)
{
	if (!CoordsInField(coord))
		return false;
	return CheckerByCoords(coord) == nullptr;
}

bool CheckersGame::CheckEndGame()
{
	auto CanMove = [&](Team team)
	{
		int sum = 0;
		for (int i = 0; i < checkers.size(); i++)
			if (checkers[i]->team == team)
				sum += PossibleMoves(checkers[i]).size();
		return sum != 0;
	};
	return !CanMove(turnTeam);
}

void CheckersGame::UpdateInfo()
{
	info.allCheckers = checkers.size();
	info.blackCheckers = count_if(checkers.begin(), checkers.end(), [](Checker* t) {return t->team == Team::Black; });
	info.whiteCheckers = info.allCheckers - info.blackCheckers;
	info.isEnd = CheckEndGame();
	info.winner = (turnTeam == Team::Black) ? (Team::White) : (Team::Black);
}

void CheckersGame::ChooseChecker(const Coord& coord)
{
	chosenChecker = CheckerByCoords(coord);
	if (chosenChecker == nullptr)
	{
		Log::Write("Empty");
		return;
	}
	if (chosenChecker->team != turnTeam)
	{
		chosenChecker = nullptr;
		return;
	}
	Log::Write("Checker chosen (" + to_string(chosenChecker->coord.x) + ";" + to_string(chosenChecker->coord.y) + ")");
}

void CheckersGame::MakeTurn(const Coord& coord, const std::vector<Coord>& moves)
{
	ChopCheckers(*chosenChecker, coord, moves);
	chosenChecker->coord = coord;

	chosenChecker = nullptr;
	Log::Write("Checker moved to (" + to_string(coord.x) + "; " + to_string(coord.y) + ")");
	turnTeam = (turnTeam == Team::Black) ? (Team::White) : (Team::Black);
	info.countMoves++;
	UpdateInfo();
}

void CheckersGame::Action(const Coord& coord)
{
	Log::Write("Pressed to (" + to_string(coord.x) + ";" + to_string(coord.y) + ")");
	if (chosenChecker == nullptr)
	{
		ChooseChecker(coord);
	}
	else
	{
		vector<Coord> possibleMoves = PossibleMoves(chosenChecker);
		if (find(possibleMoves.begin(), possibleMoves.end(), coord) != possibleMoves.end())
			MakeTurn(coord, possibleMoves);
		else
			ChooseChecker(coord);
	}
}

void CheckersGame::Init()
{

}
