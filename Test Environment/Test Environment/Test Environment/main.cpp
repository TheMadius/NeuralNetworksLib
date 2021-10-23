#include "Log.h"
#include "CheckersGame.h"
#include "ModelView.h"

using namespace std;

int main()
{
	Log::Init("log.txt");
	CheckersGame game;
	game.NewGame();

	ModelView test;
	test.ConnectGame(&game);
	test.Start();


	Log::Stop();
}