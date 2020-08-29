#pragma once
#include <string>


struct Command {
	virtual const int getSize() { return -1; };
	virtual const char* getWord() { return ""; };
};

struct LoginCommand : Command {
	const int getSize() { return 5; }
	const char* getWord() { return "login"; }
};

struct LogoutCommand : public Command {
	const int getSize() { return 7; }
	const char* getWord() { return "logout"; }
};

struct CalcCommand : public Command {
	const int getSize() { return 5; }
	const char* getWord() { return "calc"; }

};
