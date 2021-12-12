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
	int num = 0;
	Log::Init("log.txt");
	ChessGame gameChess;
	
	ChessGameAI gameChessAiW(&gameChess,0.9, ChessGame::Team::White);
	ChessGameAI gameChessAiB(&gameChess,0.9, ChessGame::Team::Black);

	gameChessAiB.setOpponent(&gameChessAiW);

	gameChess.NewGame();

	thread t2 = thread([&]()
		{
			ChessMV v;
			v.ConnectGame(&gameChess);
			v.Start();
		});

	while (true)
	{
		gameChessAiW.Move();
		gameChessAiB.Move(true);
		if (gameChess.GetInfo().isEnd)
		{
			num++;
			gameChess.NewGame();
			gameChessAiB.save("modelBChess.mo");
			if(num % 5 == 0)
				gameChessAiB.save("modelBChess"+to_string(num)+".mo");
		}
	}

	t2.join();
	//t3.join();
	Log::Stop();
}