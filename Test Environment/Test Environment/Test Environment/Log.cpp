#include "Log.h"

using namespace std;

ofstream Log::logFile;
thread Log::logThread;
mutex Log::logMutex;
vector<string> Log::logVector;
bool Log::logging = false;

void Log::Init(std::string fileName)
{
	logFile = ofstream(fileName, ios::out);
	logThread = thread(StartLog);
	logging = true;
}

void Log::Write(std::string data)
{
	logMutex.lock();
	logVector.insert(logVector.begin(), data);
	logMutex.unlock();
}

void Log::Stop()
{
	while (!logVector.empty())
		this_thread::sleep_for(chrono::milliseconds(10));
	logging = false;
	logThread.join();
}

void Log::StartLog()
{
	while (logging)
	{
		this_thread::sleep_for(chrono::milliseconds(10));
		logMutex.lock();
		while (!logVector.empty())
		{
			clock_t t = clock();
			cout << t / 60000 << ":" << (t % 60000) / 1000 << ":" << t % 1000 << ":\t" << logVector.back() << endl;
			logFile << t / 60000 << ":" << (t % 60000) / 1000 << ":" << t % 1000 << ":\t" << logVector.back() << endl;
			logVector.pop_back();
		}
		logMutex.unlock();
	}
	return;
}
