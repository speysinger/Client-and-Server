#pragma once
#include "database.h"
class DbFacade
{
public:
	DbFacade();
	bool createUser(const char *login, const char *password, const char *salt);
	char *getPassword(char *login);
private:
	Database *database;

};

