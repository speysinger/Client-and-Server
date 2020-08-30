#include "dbFacade.h"
#include "database.h"
#include "database/sqlite3.h"

#include <winsqlite/winsqlite3.h>

DbFacade::DbFacade() {
	database = new Database();
	//exception from db
}

bool DbFacade::createUser(const char *login, const char *password, const char *salt) {

	std::string request = std::string("INSERT INTO Users (login, password, salt) VALUES (")
		+ std::string(login) + ", "
		+ std::string(password) + ", "
		+ std::string(salt) + ");"
		;

	try {
		database->exec(request);
	}
	catch (std::string exception) {
		return false;
	}
	return true;
}
