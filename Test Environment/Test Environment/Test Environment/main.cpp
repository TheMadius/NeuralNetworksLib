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
	CheckersGameAI AIControllerWhite(&game, 0.01, Team::White);
	CheckersGameAI AIControllerBlack(&game, 0.01, Team::Black);

	game.NewGame();

	thread t = thread([&]()
		{
			ModelView test;
			test.ConnectGame(&game);
			test.Start();
		});

	thread aiWhite = thread([&]()
		{
			while (true)
			{
				m.lock();
				AIControllerWhite.Move(true);
				m.unlock();
			}
		});

	thread aiBlack = thread([&]()
		{
			while (true)
			{
				m.lock();
				AIControllerBlack.Move(true);
				m.unlock();
			}
		});

	aiWhite.join();
	aiBlack.join();
	t.join();

	Log::Stop();
}