#include "Log.h"
#include "CheckersGameAI.h"
#include "CheckersGame.h"
#include "ChessGame.h"
#include "ChessMV.h"
#include <thread>
#include <chrono>

using namespace std;

int main()
{
	Log::Init("log.txt");
	//CheckersGame game;
	ChessGame gameA;
	//CheckersGameAI AIControllerWhite(&game, 0.01, Team::White);
	//CheckersGameAI AIControllerBlack(&game, 0.01, Team::Black);

	thread t = thread([&]()
		{
			ChessMV mv;
			mv.ConnectGame(&gameA);
			mv.Start();
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
	t.join();

	Log::Stop();
}