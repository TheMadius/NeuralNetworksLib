#include "Log.h"
#include "CheckersGame.h"

using namespace std;

int main()
{
	Log::Init("log.txt");
	CheckersGame game;

	game.GetChosenChecker(1, 1);
	game.GetChosenChecker(7, 3);
	game.GetChosenChecker(5, 5);

	game.NewGame();

	Log::Stop();
}