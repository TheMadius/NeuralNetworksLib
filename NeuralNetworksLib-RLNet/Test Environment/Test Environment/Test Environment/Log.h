#pragma once
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <vector>
#include <string>

class Log
{
public:

	static void Init(std::string fileName);

	static void Write(std::string data);

	static void Stop();

private:

	static void StartLog();

	static std::ofstream logFile;
	static std::thread logThread;
	static std::mutex logMutex;
	static std::vector<std::string> logVector;
	static bool logging;

};

