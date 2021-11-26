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
	mutex m;
	CheckersGame gameCheckers;
	ChessGame gameChess;

	gameCheckers.NewGame();
	gameChess.NewGame();

	thread t = thread([&]()
		{
			ModelView test;
			test.ConnectGame(&gameCheckers);
			test.Start();
		});

	thread t2 = thread([&]()
		{
			ChessMV test;
			test.ConnectGame(&gameChess);
			test.Start();
		});

	t.join();
	t2.join();

	Log::Stop();
}