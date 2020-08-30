#include "database.h"

Database::Database() {
	if (sqlite3_open("users.db", &db)) {
		throw "Database not available";
	}
	const std::string databaseCreation = "CREATE TABLE IF NOT EXISTS Users(login TEXT PRIMARY KEY, password TEXT, salt TEXT);";
	this->exec(databaseCreation);
}

sqlite3_stmt *Database::exec(const std::string str) {
	sqlite3_stmt *stmt;
	int error = sqlite3_prepare_v2(db, str.c_str(), -1, &stmt, NULL);

	std::string errorMessage = sqlite3_errmsg(db);

	if (error != SQLITE_OK) {
		throw "Database error: " + errorMessage;
	}

	return stmt;
}
