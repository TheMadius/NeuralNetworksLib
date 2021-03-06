#include "CheckersGame.h"
#include "Log.h"
#include <algorithm>
#include <numeric>

using namespace std;

void CheckersGame::InitializeGame(CheckersGame* game) 
{
	for (int i = 0; i < checkers.size(); i++)
		if (checkers[i] != nullptr)
			delete checkers[i];

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
	game->info.turn = Team::White;

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

std::vector<Move> CheckersGame::PossibleMoves(const Checker* checker)
{
	if (checker == nullptr)
		return vector<Move>();
	if (checker->team != turnTeam)
		return vector<Move>();

	return PossibleMovesRecursive(checker, *checker, Coord(0, 0), vector<Checker*>());
}

std::vector<Move> CheckersGame::AllPossibleMoves()
{
	vector<Move> ret;

	for (int i = 0; i < checkers.size(); i++)
	{
		auto t = PossibleMoves(checkers[i]);
		ret.insert(ret.end(), t.begin(), t.end());
	}
	auto r = find_if(ret.begin(), ret.end(), [&](const Move& x) { return !x.chopedCheckers.empty(); });
	if (r != ret.end())
	{
		vector<Move> retn;
		copy_if(ret.begin(), ret.end(), retn.begin(), [&](const Move& x) { return !x.chopedCheckers.empty(); });
		return retn;
	}

	return ret;
}

std::vector<Move> CheckersGame::PossibleMovesRecursive(const Checker* cp, const Checker& checker, const Coord& dir, const vector<Checker*>& chopedYet)
{
	vector<Move> ret;
	int dy = (checker.team == Team::Black) ? (-1) : (1);
	Coord lu = Coord(-1, dy), ru = Coord(1, dy), ld = Coord(-1, -dy), rd = Coord(1, -dy);

	auto tryMove = [&](const Coord& coord, const Coord& dir)
	{
		if (coord == -dir)
			return;
		if (!CoordIsEmpty(checker.coord + coord) && CoordsInField(checker.coord + coord))
		{
			Checker* choped = CheckerByCoords(checker.coord + coord);
			if (choped->team != checker.team && CoordIsEmpty(checker.coord + coord * 2))
			{
				Move t(*cp, checker.coord + coord * 2);
				t.chopedCheckers.insert(t.chopedCheckers.end(), chopedYet.begin(), chopedYet.end());
				t.chopedCheckers.push_back(choped);
				ret.push_back(t);
				vector<Move> tmp = PossibleMovesRecursive(cp,
					Checker(checker.coord + coord * 2, checker.type, checker.team), coord, t.chopedCheckers);
				ret.insert(ret.end(), tmp.begin(), tmp.end());
			}
		}
	};
	auto move = Move(*cp, checker.coord + ru);
	auto moves = &move;
	*moves == move;
	//chop moves
	tryMove(ru, dir);
	tryMove(rd, dir);
	tryMove(ld, dir);
	tryMove(lu, dir);
	//not chop moves
	if (dir == Coord(0, 0) && ret.empty())
	{
		if (CoordIsEmpty(checker.coord + ru))
			ret.push_back(Move(*cp, checker.coord + ru));
		if (CoordIsEmpty(checker.coord + lu))
			ret.push_back(Move(*cp, checker.coord + lu));
	}
	return ret;
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

void CheckersGame::MakeTurn(const Move& move)
{
	for (int i = 0; i < move.chopedCheckers.size(); i++)
	{
		auto t = find(checkers.begin(), checkers.end(), move.chopedCheckers[i]);
		*t = nullptr;
		checkers.erase(t);
		delete move.chopedCheckers[i];
	}
	chosenChecker->coord = move.target;

	chosenChecker = nullptr;
	Log::Write("Checker moved to (" + to_string(move.target.x) + "; " + to_string(move.target.y) + ")");
	turnTeam = (turnTeam == Team::Black) ? (Team::White) : (Team::Black);
	this->info.turn = turnTeam;
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
		vector<Move> possibleMoves = PossibleMoves(chosenChecker);
		auto move = find_if(possibleMoves.begin(), possibleMoves.end(), [&](const Move& m) {return m.target == coord; });
		if (move != possibleMoves.end())
			MakeTurn(*move);
		else
			ChooseChecker(coord);
	}
}

bool CheckersGame::MakeMove(const Checker* checker, const Move& move)
{
	auto possM = PossibleMoves(checker);
	if (find(possM.begin(), possM.end(), move) == possM.end())
		return false;
	ChooseChecker(checker->coord);
	MakeTurn(move);

	return true;
}

void CheckersGame::Init()
{

}
