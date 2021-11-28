#include "Log.h"
#include "CheckersGameAI.h"
#include "CheckersGame.h"
#include "ChessGame.h"
#include "ModelView.h"
#include "ChessMV.h"
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

int main()
{
	Log::Init("log.txt");
	ModelView v;
	CheckersGame gameCheckers;
	CheckersGameAI gameCheckersAiW(&gameCheckers,0.9,Team::White);
	CheckersGameAI gameCheckersAiB(&gameCheckers,0.9,Team::Black);

	gameCheckers.NewGame();

	thread t2 = thread([&]()
		{
			while (true)
			{
				gameCheckersAiW.Move();
				gameCheckersAiB.Move(true);
				if (gameCheckers.GetInfo().isEnd)
				{
					gameCheckers.NewGame();
				}
			}
		});

	v.ConnectGame(&gameCheckers);
	v.Start();

	t2.join();
	Log::Stop();
}