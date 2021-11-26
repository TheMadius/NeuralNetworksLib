#include "Log.h"
//#include "CheckersGameAI.h"
//#include "CheckersGame.h"
#include "ChessGame.h"
#include "ChessMV.h"
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

int main()
{
	Log::Init("log.txt");
<<<<<<< HEAD
	//CheckersGame game;
	ChessGame game;
	//CheckersGameAI AIControllerWhite(&game, 0.01, Team::White);
	//CheckersGameAI AIControllerBlack(&game, 0.01, Team::Black);
=======
	mutex m;
	CheckersGame game;
	CheckersGameAI AIControllerWhite(&game, 0.9, Team::White);
	CheckersGameAI AIControllerBlack(&game, 0.9, Team::Black);
>>>>>>> master

	game.NewGame();

	thread t = thread([&]()
		{
<<<<<<< HEAD
			ChessMV test;
			test.ConnectGame(&game);
			test.Start();
		});

	//thread ai = thread([&]()
	//	{
	//		while (true)
	//		{
	//			this_thread::sleep_for(chrono::milliseconds(500));
	//			AIControllerWhite.Move();
	//			this_thread::sleep_for(chrono::milliseconds(500));
	//			AIControllerBlack.Move();
	//		}
	//	});

	//ai.join();
=======
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
>>>>>>> master
	t.join();

	Log::Stop();
}