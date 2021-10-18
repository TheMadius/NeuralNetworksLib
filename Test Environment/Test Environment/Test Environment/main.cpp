#include "Log.h"
#include "CheckersGame.h"

using namespace std;

int main()
{
	Log::Init("log.txt");
	CheckersGame game;

	game.CheckerByCoords(Coord(1, 1));
	game.CheckerByCoords(Coord(7, 3));
	game.CheckerByCoords(Coord(5, 5));

	game.NewGame();

	Log::Stop();
}