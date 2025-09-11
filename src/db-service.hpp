#pragma once

#include <iostream>
#include <sqlite3.h>

class DbService {

private:
  sqlite3 *pDB;

public:
  void openDb(const char *filename) {
    int exit = 0;
    exit = sqlite3_open(filename, &pDB);

    // Logging out the information
    if (exit) {
      std::cerr << "Error occured while opening the database. Please try again."
                << std::endl;
    } else {
      std::cout << "Opened database succesfully!" << std::endl;
    }
  }

  void closeDB() { sqlite3_close(pDB); }

  void execSql(const char *sql) {
    int exit = 0;
    char *errMessage;
    exit = sqlite3_exec(pDB, sql, NULL, 0, &errMessage);

    if (exit != SQLITE_OK) {
      std::cerr << "Sql couldn't be executed sql error " << std::endl;
      sqlite3_free(errMessage);
    }
  }
};
