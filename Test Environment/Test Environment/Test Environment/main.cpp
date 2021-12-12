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
	ChessGame gameChess;
	//CheckersGame gameChess;
	ChessGameAI gameChessAiW(&gameChess,0.9, ChessGame::Team::White);
	ChessGameAI gameChessAiB(&gameChess,0.9, ChessGame::Team::Black);

	//CheckersGameAI gameChessAiW(&gameChess,0.9, Team::White);
	//CheckersGameAI gameChessAiB(&gameChess,0.9, Team::Black);

	gameChess.NewGame();

	thread t2 = thread([&]()
		{
			ChessMV v;
			v.ConnectGame(&gameChess);
			v.Start();
		});

	while (true)
	{
		//this_thread::sleep_for(chrono::milliseconds(1000));
		gameChessAiW.Move();
		//this_thread::sleep_for(chrono::milliseconds(1000));
		gameChessAiB.Move();
	}


	t2.join();
	//t3.join();
	Log::Stop();
}