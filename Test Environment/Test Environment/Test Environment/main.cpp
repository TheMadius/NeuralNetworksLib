#include "Log.h"
#include "CheckersGameAI.h"
#include "CheckersGame.h"
#include "ModelView.h"
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

int main()
{
	Log::Init("log.txt");
	mutex m;
	CheckersGame game;
	CheckersGameAI AIControllerWhite(&game, 0.9, Team::White);
	CheckersGameAI AIControllerBlack(&game, 0.9, Team::Black);

	game.NewGame();

	thread t = thread([&]()
		{
			//ModelView test;
			//test.ConnectGame(&game);
			//test.Start();
		});

	thread aiWhite = thread([&]()
		{
			while (true)
			{
				AIControllerWhite.Move();
				AIControllerBlack.Move(true);
				if (game.GetInfo().isEnd)
				{
					game.NewGame();
				}
			}
		});

	aiWhite.join();
	t.join();

	Log::Stop();
}