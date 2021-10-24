#include "Log.h"
#include "CheckersGame.h"
#include "ModelView.h"
#include <thread>

using namespace std;

int main()
{
	Log::Init("log.txt");
	CheckersGame game, game1;
	game.NewGame();
	game1.NewGame();

	thread t = thread([&]()
		{
			ModelView test;
			test.ConnectGame(&game);
			test.Start();
		});

	thread t1 = thread([&]()
		{
			ModelView test;
			test.ConnectGame(&game1);
			test.Start();
		});

	t.join();
	t1.join();
	//t.join();
	//t1.join();
	Log::Stop();
}