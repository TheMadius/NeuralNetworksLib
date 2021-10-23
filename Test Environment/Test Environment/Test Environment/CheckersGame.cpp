#include "CheckersGame.h"
#include "Log.h"

using namespace std;

void CheckersGame::InitializeGame(CheckersGame* game) {
	game->turn = Team::White;
	game->chosenChecker = nullptr;

	game->checkers.clear();
	//Add white checkers
	for (int i = 1; i <= 8; i++)
		for (int j = 1; j <= 3; j++)
			if ((i + j) % 2 != 0)
				game->checkers.push_back(new Checker(Coord(i, j), CheckerType::Checker, Team::White));

	//Logging part
	Log::Write("White checkers was added\n\tCheckers total: " + std::to_string(checkers.size()));
	//Logging part end


	//Add black checkers
	for (int i = 1; i <= 8; i++)
		for (int j = 6; j <= 8; j++)
			if ((i + j) % 2 != 0)
				game->checkers.push_back(new Checker(Coord(i, j), CheckerType::Checker, Team::Black));

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
		if (checkers[i] == nullptr)
			continue;
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

//qwerty

std::vector<const Checker*> CheckersGame::GetCheckers() const
{
	vector<const Checker*> ret;
	for (int i = 0; i < checkers.size(); i++)
		ret.push_back(checkers[i]);
	return ret;
}

std::vector<Coord> CheckersGame::GetPossibleMoves(const Checker* checker)
{
	if (chosenChecker == nullptr)
		return vector<Coord>();

	return GetPossibleMovesRecursive(*chosenChecker, 0);
}

std::vector<Coord> CheckersGame::GetPossibleMovesRecursive(const Checker& checker, int dir) //dir: 1-ru, 2-rd, 3-ld, 4-lu, 0-none
{
	vector<Coord> ret;
	int dy = (checker.team == Team::Black) ? (-1) : (1);
	Coord left = Coord(-1, dy), right = Coord(1, dy);
	//not chop moves
	if (dir == 0)
	{
		if (CoordIsEmpty(checker.coord + Coord(1, dy)))
			ret.push_back(checker.coord + Coord(1, dy));
		if (CoordIsEmpty(checker.coord + Coord(-1, dy)))
			ret.push_back(checker.coord + Coord(-1, dy));
	}
	//chop moves
	if (!CoordIsEmpty(checker.coord + right) && CoordsInField(checker.coord + right))
		if (CheckerByCoords(checker.coord + right)->team != checker.team && CoordIsEmpty(checker.coord + right * 2))
		{
			ret.push_back(checker.coord + right * 2);
			vector<Coord> tmp = GetPossibleMovesRecursive(
				Checker(checker.coord + right * 2, checker.type, checker.team), (dy == 1) ? (1) : (2));
			ret.insert(ret.end(), tmp.begin(), tmp.end());
		}
	if (!CoordIsEmpty(checker.coord + left) && CoordsInField(checker.coord + left))
		if (CheckerByCoords(checker.coord + left)->team != checker.team && CoordIsEmpty(checker.coord + left * 2))
		{
			ret.push_back(checker.coord + left * 2);
			vector<Coord> tmp = GetPossibleMovesRecursive(
				Checker(checker.coord + left * 2, checker.type, checker.team), (dy == 1) ? (4) : (3));
			ret.insert(ret.end(), tmp.begin(), tmp.end());
		}
	return ret;
}

void CheckersGame::ChopCheckers(const Checker& checker, const Coord& coord, const std::vector<Coord>& moves)
{
	auto findMove = [](const vector<Coord>& vect, Coord coord)
	{
		return find(vect.begin(), vect.end(), coord) != vect.end();
	};

	int dy = (checker.team == Team::Black) ? (-1) : (1);
	Coord tmp = coord, left = Coord(-1, dy), right = Coord(1, dy);
	vector<Checker*> delCheckers;

	if (abs((coord - checker.coord).x) == 1)
		return;

	while (true)
	{
		if (tmp == checker.coord)
			break;
		Checker* midChecker = CheckerByCoords(tmp - right);
		bool isMidEnemy = midChecker != nullptr && midChecker->team != checker.team;
		if (isMidEnemy && (findMove(moves, tmp - right * 2) || tmp - right * 2 == checker.coord))
		{
			delCheckers.push_back(midChecker);
			tmp = tmp - right * 2;
			continue;
		}
		midChecker = CheckerByCoords(tmp - left);
		isMidEnemy = midChecker != nullptr && midChecker->team != checker.team;
		if (isMidEnemy && (findMove(moves, tmp - left * 2) || tmp - left * 2 == checker.coord))
		{
			delCheckers.push_back(midChecker);
			tmp = tmp - left * 2;
			continue;
		}
		throw exception("Can't build chop checkers way");
	}
	for (int i = 0; i < delCheckers.size(); i++)
	{
		*find(checkers.begin(), checkers.end(), delCheckers[i]) = nullptr;
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

void CheckersGame::Action(const Coord& coord)
{
	Log::Write("Pressed to (" + to_string(coord.x) + ";" + to_string(coord.y) + ")");
	if (chosenChecker == nullptr)
	{
		chosenChecker = CheckerByCoords(coord);
		if (chosenChecker == nullptr)
		{
			Log::Write("Empty");
			return;
		}
		if (chosenChecker->team != turn)
		{
			chosenChecker = nullptr;
			return;
		}
		Log::Write("Checker chosen (" + to_string(chosenChecker->coord.x) + ";" + to_string(chosenChecker->coord.y) + ")");
	}
	else
	{
		vector<Coord> possibleMoves = GetPossibleMoves(chosenChecker);
		if (find(possibleMoves.begin(), possibleMoves.end(), coord) != possibleMoves.end())
		{
			ChopCheckers(*chosenChecker, coord, possibleMoves);
			chosenChecker->coord = coord;
			chosenChecker = nullptr;
			Log::Write("Checker moved to (" + to_string(coord.x) + "; " + to_string(coord.y) + ")");
			turn = (turn == Team::Black) ? (Team::White) : (Team::Black);
		}
		else
		{
			chosenChecker = CheckerByCoords(coord);
			if (chosenChecker == nullptr)
			{
				Log::Write("Empty");
				return;
			}
			Log::Write("Checker chosen (" + to_string(chosenChecker->coord.x) + ";" + to_string(chosenChecker->coord.y) + ")");
		}
	}
}

void CheckersGame::Init()
{

}
