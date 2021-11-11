#include "Log.h"
#include "CheckersGame.h"
#include "ModelView.h"
#include <thread>

using namespace std;

int main()
{
	Log::Init("log.txt");
	CheckersGame game;
	game.NewGame();

	thread t = thread([&]()
		{
			ModelView test;
			test.ConnectGame(&game);
			test.Start();
		});


	t.join();
	Log::Stop();
}