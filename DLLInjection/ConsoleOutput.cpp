#include "stdafx.h"
#include "ConsoleOutput.h"
#include "ctime"
#include "string"
#include "iostream"

using namespace std;

string ConsoleOutput::Time() {
	time_t timeNow = time(0);
	tm timeInfo;
	localtime_s(&timeInfo, &timeNow);

	string time;

	if (to_string(timeInfo.tm_hour).length() == 1) {
		time += to_string(0);
	}
	time += to_string(timeInfo.tm_hour);
	time += ":";

	if (to_string(timeInfo.tm_min).length() == 1) {
		time += to_string(0);
	}
	time += to_string(timeInfo.tm_min);
	time += ":";

	if (to_string(timeInfo.tm_sec).length() == 1) {
		time += to_string(0);
	}
	time += to_string(timeInfo.tm_sec);
	return time;
}

void ConsoleOutput::Info(string string) {
	cout << "[" << Time() << "] [Info] [DLLInjection]: " << string << "\n";
}

void ConsoleOutput::Warn(string string) {
	cout << "[" << Time() << "] [Warn] [DLLInjection]: " << string << "\n";
}

void ConsoleOutput::Error(string string) {
	cout << "[" << Time() << "] [Error] [DLLInjection]: " << string << "\n";
}