#pragma once

#include <string>
#include "database/sqlite3.h"
class Database
{
public:
	Database();
	sqlite3_stmt *exec(const std::string str);
private:
	char *callback(void *NotUsed, int argc, char **argv, char **azColName);
	sqlite3 *db = 0;
};

