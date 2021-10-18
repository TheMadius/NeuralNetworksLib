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
	Coord coord;
	CheckerType type;
	Team team;
};

struct GameInfo
{
	GameInfo() : isEnd(false), winner(Team::Black), allCheckers(0), blackCheckers(0), whiteCheckers(0), countMoves(0) {}
	bool isEnd;
	Team winner;
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

	std::vector<Coord> GetPossibleMoves(const Checker* checker); //доступные клетки для хода шашки

	void Action(const Coord& coord); //нажатие на клетку

	virtual void Init(); //инициализация

protected:

	Checker* chosenChecker;

	std::vector<Checker> checkers;

	GameInfo info;

private:

	Checker* CheckerByCoords(const Coord& coord); //получить шашку по координатам

	void InitializeGame(CheckersGame*);

	std::vector<Coord> GetPossibleMovesRecursive(const Checker& checker, int dir);

	void ChopCheckers(const Checker& checker, const Coord& coord, const std::vector<Coord>& moves);

	bool CoordsInField(const Coord& coord);

	bool CoordIsEmpty(const Coord& coord);

};

