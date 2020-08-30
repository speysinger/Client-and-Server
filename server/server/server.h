#pragma once
#include <winsqlite/winsqlite3.h>

#include "commands.h"
#include "dbFacade.h"

class Server
{
public:
	Server();
	void start();
private:
	DbFacade *DB;
	static const int stringLength = 140;
	enum ServerCommands { LOGIN, LOGOUT, CALC, WRONG };

	bool validateUser(char *login, char *password);
	bool processLoginCommand(char *str);
	bool processLogoutCommand(char *str);
	bool processCalcCommand(char *str);

	bool checkCommand(char *str, Command *command);
	char *deleteSubString(char *str, int numberCharactersToRemove);
	ServerCommands processCommand(char *str);
};

