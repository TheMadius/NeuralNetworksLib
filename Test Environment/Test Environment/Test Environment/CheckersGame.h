#pragma once
#include "Game.h"
#include <vector>
#include <string>

enum class CheckerType
{
	Checker, King
};

enum class Team
{
	Black, White
};

struct Coord
{
	Coord(int x, int y) : x(x), y(y) {}
	Coord operator +(const Coord& other) const { return Coord(x + other.x, y + other.y); }
	Coord operator -(const Coord& other) const { return *this + -other; }
	Coord operator -() const { return Coord(-x, -y); }
	Coord operator *(int k) const { return Coord(x * k, y * k); }
	bool operator ==(const Coord& other) const { return x == other.x && y == other.y; }
	int x, y;
};

struct Checker
{
	Checker(const Coord& coord, CheckerType type, Team team) : coord(coord), type(type), team(team) {}
	bool operator ==(const Checker& other) const { return coord == other.coord && type == other.type && team == other.team;}
	Coord coord;
	CheckerType type;
	Team team;
};

struct Move
{
	Move(const Checker& checker, const Coord& target) : target(target), checker(&checker) {};
	bool operator==(const Move& other) { return checker == other.checker && target == other.target; }
	bool operator!=(const Move& other) { return !(*this == other); }
	const Checker* checker;
	Coord target;
	std::vector<Checker*> chopedCheckers;
};

struct GameInfo
{
	GameInfo() : isEnd(false), winner(Team::Black), turn(Team::White), allCheckers(0), blackCheckers(0), whiteCheckers(0), countMoves(0) {}
	bool isEnd;
	Team winner;
	Team turn;
	int allCheckers, blackCheckers, whiteCheckers;
	int countMoves;
};

class CheckersGame :
	public Game
{
public:
	CheckersGame();

	void NewGame();

	GameInfo GetInfo() const; //получить текущую информацию об игре

	const Checker* GetChosenChecker() const; //получить выбранную шашку

	std::vector<const Checker*> GetCheckers() const; //все шашки на поле

	std::vector<Move> PossibleMoves(const Checker* checker); //доступные клетки для хода шашки

	std::vector<Move> AllPossibleMoves(); //доступные клетки для хода шашек

	void Action(const Coord& coord); //нажатие на клетку

	bool MakeMove(const Checker* checker, const Move& move);

	virtual void Init(); //инициализация

protected:

	Checker* chosenChecker;

	std::vector<Checker*> checkers;

	GameInfo info;

	Team turnTeam;

private:

	Checker* CheckerByCoords(const Coord& coord); //получить шашку по координатам

	void InitializeGame(CheckersGame*);

	std::vector<Move> PossibleMovesRecursive(const Checker* cp, const Checker& checker, const Coord& dir, const std::vector<Checker*>& chopedYet);

	bool CoordsInField(const Coord& coord);

	bool CoordIsEmpty(const Coord& coord);

	bool CheckEndGame();

	void UpdateInfo();

	void ChooseChecker(const Coord& coord);

	void MakeTurn(const Move& move);
};

