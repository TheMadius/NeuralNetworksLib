#include "Log.h"
#include "CheckersGameAI.h"
#include "CheckersGame.h"
#include "ChessGameAI.h"
#include "ChessGame.h"
#include "ModelView.h"
#include "ChessMV.h"
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;
double LEARNING_RATE = 0.01;

int main()
{
	Log::Init("log.txt");
	CheckersGame gameChess;
	mutex m;
	bool ai = true;

	CheckersGameAI gameChessAiW(&gameChess,0.9, Team::White);
	CheckersGameAI gameChessAiB(&gameChess,0.9, Team::Black);

	gameChess.NewGame();

	thread t2 = thread([&]()
		{
			ModelView v;
			v.ConnectGame(&gameChess);
			v.Start();
		});

	thread t3 = thread([&]()
		{
			while (true)
			{
				if (ai)
					gameChessAiW.Move();
				gameChessAiB.Move(ai);

				if (gameChess.GetInfo().isEnd)
					gameChess.NewGame();
			}
		});

	t2.join();
	t3.join();
	Log::Stop();
}