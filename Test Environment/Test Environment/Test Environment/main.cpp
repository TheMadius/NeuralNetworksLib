#include "Log.h"
#include "CheckersGame.h"

using namespace std;

int main()
{
	Log::Init("log.txt");
	CheckersGame game;
	Log::Write("Game created");

	Log::Stop();
}