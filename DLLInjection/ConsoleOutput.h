#include "iostream"

class ConsoleOutput {

private:
	static std::string Time();

public:
	static void Info(std::string string);
	static void Warn(std::string string);
	static void Error(std::string string);
};