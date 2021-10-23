#include "Log.h"
#include "CheckersGame.h"
#include "ModelView.h"

using namespace std;

int main()
{
	
	//ModelView test;
	//test.Start();
	Log::Init("log.txt");
	CheckersGame game;

	game.NewGame();

	Log::Stop();
}