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

int main()
{
	Log::Init("log.txt");
	ChessGame gameChess;
	ChessGameAI gameChessAiW(&gameChess,0.9, ChessGame::Team::White);
	ChessGameAI gameChessAiB(&gameChess,0.9, ChessGame::Team::Black);

	gameChess.NewGame();

	thread t2 = thread([&]()
		{
			ChessMV v;
			v.ConnectGame(&gameChess);
			v.Start();
		});

	while (true)
	{
		this_thread::sleep_for(chrono::milliseconds(400));
		gameChessAiW.Move();
		gameChessAiB.Move(true);
		if (gameChess.GetInfo().isEnd)
		{
			gameChess.NewGame();
		}
	}


	t2.join();
	Log::Stop();
}