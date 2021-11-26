#include "ChessGame.h"
#include "Log.h"
#include <algorithm>

using namespace std;

ChessGame::ChessGame()
{
	NewGame();
}

void ChessGame::NewGame()
{
	InitializeGame();
}

ChessGame::GameInfo ChessGame::GetInfo() const
{
	return info;
}

const ChessGame::Figure* ChessGame::GetChosenFigure() const
{
	return chosenFigure;
}

std::vector<const ChessGame::Figure*> ChessGame::GetFigures() const
{
	vector<const Figure*> ret;
	for (int i = 0; i < figures.size(); i++)
		ret.push_back(figures[i]);
	return ret;
}

std::vector<ChessGame::Move> ChessGame::PossibleMoves(const Figure* figure)
{
	if (figure == nullptr)
		return vector<Move>();
	vector<Move> ret;
	auto tryMove = [&](const Coord& dc)
	{
		Coord to = figure->coord + dc;
		Move tmp = Move(figure->coord, to);
		if (!IsMovePossible(figure->coord, to))
			return;
		if (!CoordsInField(to))
			return;
		if (CoordIsEmpty(to))
			ret.push_back(tmp);
		else if (FigureByCoords(to)->team != figure->team)
			ret.push_back(tmp);
	};
	auto tryMovePawn = [&](const Coord& dc)
	{
		Coord to = figure->coord + dc;
		if (!IsMovePossible(figure->coord, to))
			return;
		if (CoordIsEmpty(to))
			return;
		if (FigureByCoords(to)->team != figure->team)
			ret.push_back(Move(figure->coord, to));
	};
	auto lineMoves = [&](const Coord& dir)
	{
		for (int i = 1; i < 8; i++)
		{
			tryMove(dir * i);
			if (!CoordsInField(figure->coord + dir * i))
				break;
			if (FigureByCoords(figure->coord + dir * i) != nullptr)
				break;
		}
	};
	auto moveFromPosition = [&](const Coord& pos)
	{
		return find_if(history.begin(), history.end(), [&](const Move& x) { return x.figureCoord == pos; }) != history.end();
	};

	int dy = (figure->team == Team::Black) ? (-1) : (1);
	Coord lu = Coord(-1, dy), ru = Coord(1, dy), ld = Coord(-1, -dy), rd = Coord(1, -dy);
	Coord u = Coord(0, dy), d = Coord(0, -dy), l = Coord(-1, 0), r = Coord(1, 0);

	switch (figure->type)
	{
	case FigureType::Pawn:
		if (!figure->moved)
			tryMove(u * 2);
		tryMove(u);
		tryMovePawn(lu);
		tryMovePawn(ru);
		break;
	case FigureType::Knight:
		tryMove(u + ru);
		tryMove(u + lu);
		tryMove(d + ld);
		tryMove(d + rd);
		tryMove(l + lu);
		tryMove(l + ld);
		tryMove(r + ru);
		tryMove(r + rd);
		break;
	case FigureType::Bishop:
		lineMoves(ru);
		lineMoves(rd);
		lineMoves(ld);
		lineMoves(lu);
		break;
	case FigureType::Rook:
		lineMoves(u);
		lineMoves(r);
		lineMoves(d);
		lineMoves(l);
		break;
	case FigureType::Queen:
		lineMoves(ru);
		lineMoves(rd);
		lineMoves(ld);
		lineMoves(lu);
		lineMoves(u);
		lineMoves(r);
		lineMoves(d);
		lineMoves(l);
		break;
	case FigureType::King:
		if (!figure->moved)
		{
			if (moveFromPosition((figure->team == ChessGame::Team::White) ? (Coord(1, 1)) : (Coord(1, 8))))
				tryMove(l * 2);
			if (moveFromPosition((figure->team == ChessGame::Team::White) ? (Coord(8, 1)) : (Coord(8, 8))))
				tryMove(r * 2);
		}
		tryMove(u);
		tryMove(ru);
		tryMove(r);
		tryMove(rd);
		tryMove(d);
		tryMove(ld);
		tryMove(l);
		tryMove(lu);
		break;
	default:
		break;
	}
	return ret;
}

void ChessGame::Action(const Coord& coord)
{
	Log::Write("Pressed to (" + to_string(coord.x) + ";" + to_string(coord.y) + ")");
	if (chosenFigure == nullptr)
	{
		ChooseFigure(coord);
	}
	else
	{
		vector<Move> possibleMoves = PossibleMoves(chosenFigure);
		auto move = find_if(possibleMoves.begin(), possibleMoves.end(), [&](const Move& m) {return m.target == coord; });
		if (move != possibleMoves.end())
			MakeTurn(*move);
		else
			ChooseFigure(coord);
	}
}

bool ChessGame::MakeMove(const Figure* figure, const Move& move)
{
	return false;
}

ChessGame::Figure* ChessGame::FigureByCoords(const Coord& coord)
{
	for (int i = 0; i < figures.size(); i++)
		if (figures[i]->coord == coord)
			return figures[i];
	return nullptr;
}

void ChessGame::InitializeGame()
{
	turnTeam = Team::White;
	chosenFigure = nullptr;

	for (int i = 0; i < figures.size(); i++)
		if (figures[i] != nullptr)
			delete figures[i];

	figures.clear();
	//Add white figures
	figures.push_back(new Figure(Coord(1, 1), FigureType::Rook, Team::White));
	figures.push_back(new Figure(Coord(8, 1), FigureType::Rook, Team::White));
	figures.push_back(new Figure(Coord(2, 1), FigureType::Knight, Team::White));
	figures.push_back(new Figure(Coord(7, 1), FigureType::Knight, Team::White));
	figures.push_back(new Figure(Coord(3, 1), FigureType::Bishop, Team::White));
	figures.push_back(new Figure(Coord(6, 1), FigureType::Bishop, Team::White));
	figures.push_back(new Figure(Coord(4, 1), FigureType::Queen, Team::White));
	figures.push_back(new Figure(Coord(5, 1), FigureType::King, Team::White));

	for (int i = 1; i <= 8; i++)
		figures.push_back(new Figure(Coord(i, 2), FigureType::Pawn, Team::White));

	//Logging part
	Log::Write("White figures was added\tFigures total: " + std::to_string(figures.size()));
	//Logging part end


	//Add black figures
	figures.push_back(new Figure(Coord(1, 8), FigureType::Rook, Team::Black));
	figures.push_back(new Figure(Coord(8, 8), FigureType::Rook, Team::Black));
	figures.push_back(new Figure(Coord(2, 8), FigureType::Knight, Team::Black));
	figures.push_back(new Figure(Coord(7, 8), FigureType::Knight, Team::Black));
	figures.push_back(new Figure(Coord(3, 8), FigureType::Bishop, Team::Black));
	figures.push_back(new Figure(Coord(6, 8), FigureType::Bishop, Team::Black));
	figures.push_back(new Figure(Coord(4, 8), FigureType::Queen, Team::Black));
	figures.push_back(new Figure(Coord(5, 8), FigureType::King, Team::Black));

	for (int i = 1; i <= 8; i++)
		figures.push_back(new Figure(Coord(i, 7), FigureType::Pawn, Team::Black));

	//logging part
	Log::Write("Black figures was added\tFigures total: " + std::to_string(figures.size()));
	//Logging part end

	//Add info about game
	info.isEnd = false;
	info.allFigures = figures.size();
	info.blackFigures = figures.size() / 2;
	info.whiteFigures = figures.size() / 2;
	info.winner = Team::Black;
	info.countMoves = 0;
	info.turn = Team::White;

	//Loging part
	Log::Write("Initialized game info");
	Log::Write("Game created");
	//Logging part end
}

bool ChessGame::CoordsInField(const Coord& coord)
{
	return coord.x <= 8 && coord.x >= 1 && coord.y <= 8 && coord.y >= 1;
}

bool ChessGame::CoordIsEmpty(const Coord& coord)
{
	if (!CoordsInField(coord))
		return false;
	return FigureByCoords(coord) == nullptr;
}

bool ChessGame::CheckEndGame()
{
	return false;
}

void ChessGame::UpdateInfo()
{
	info.allFigures = figures.size();
	info.blackFigures = count_if(figures.begin(), figures.end(), [](Figure* t) {return t->team == Team::Black; });
	info.whiteFigures = info.allFigures - info.blackFigures;
	info.isEnd = CheckEndGame();
	info.winner = (turnTeam == Team::Black) ? (Team::White) : (Team::Black);
}

void ChessGame::ChooseFigure(const Coord& coord)
{
	chosenFigure = FigureByCoords(coord);
	if (chosenFigure == nullptr)
	{
		Log::Write("Empty");
		return;
	}
	if (chosenFigure->team != turnTeam)
	{
		chosenFigure = nullptr;
		return;
	}
	Log::Write("Figure chosen (" + to_string(chosenFigure->coord.x) + ";" + to_string(chosenFigure->coord.y) + ")");
}

void ChessGame::MakeTurn(const Move& move)
{
	auto choped = FigureByCoords(move.target);
	if (choped != nullptr)
	{
		auto t = find(figures.begin(), figures.end(), choped);
		*t = nullptr;
		figures.erase(t);
		delete choped;
	}
	if (chosenFigure->type == FigureType::King && abs((move.target - move.figureCoord).x) == 2)
	{
		Log::Write("Rokirovka!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	}
	chosenFigure->coord = move.target;
	chosenFigure->moved = true;

	chosenFigure = nullptr;
	history.push_back(move);
	Log::Write("Figure moved to (" + to_string(move.target.x) + "; " + to_string(move.target.y) + ")");
	turnTeam = (turnTeam == Team::Black) ? (Team::White) : (Team::Black);
	this->info.turn = turnTeam;
	info.countMoves++;
	UpdateInfo();
}

bool ChessGame::IsMovePossible(const Coord& from, const Coord& to) const
{
	return true;
}
