#include "Log.h"
#include "CheckersGameAI.h"
#include "CheckersGame.h"
#include "ModelView.h"
#include <thread>
#include <chrono>

using namespace std;

int main()
{
	Log::Init("log.txt");
	CheckersGame game;
	CheckersGameAI AIControllerWhite(&game, 0.01, Team::White);
	CheckersGameAI AIControllerBlack(&game, 0.01, Team::Black);

	game.NewGame();

	thread t = thread([&]()
		{
			ModelView test;
			test.ConnectGame(&game);
			test.Start();
		});

	thread ai = thread([&]()
		{
			while (true)
			{
				this_thread::sleep_for(chrono::milliseconds(500));
				AIControllerWhite.Move();
				this_thread::sleep_for(chrono::milliseconds(500));
				AIControllerBlack.Move();
			}
		});

	ai.join();
	t.join();

	Log::Stop();
}