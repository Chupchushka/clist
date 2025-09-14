#pragma once

#include <iostream>
#include <sqlite3.h>

class DbService {

private:
public:
  sqlite3 *pDB;

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
    char *errMessage = nullptr;

    int rc = sqlite3_exec(pDB, sql, nullptr, nullptr, &errMessage);

    if (rc != SQLITE_OK) {
      std::cerr << "Sql couldn't be executed. Error: "
                << (errMessage ? errMessage : "unknown") << std::endl;
      if (errMessage)
        sqlite3_free(errMessage);
    }
  }

  static int callback(void *data, int count, char **argv, char **columnNames) {

    int i;
    for (i = 0; i < count; i++) {
      printf("Col: %s = %s\n", columnNames[i], argv[i]);
    }
    printf("\n");
    return 0;
  }

  void readDataStmt() {

    char *errMsg = 0;
    char *sql = "SELECT task_id, task, done, tasks.tag_id, name, color FROM tasks "
                "LEFT JOIN tags ON tags.tag_id = tasks.tag_id ; ";

    int rc = sqlite3_exec(pDB, sql, callback, 0, &errMsg);
    if (rc != SQLITE_OK) {
      printf("Error in executing SQL: %s \n", errMsg);
      sqlite3_free(errMsg);
    } else {
      printf("read data successfully \n");
    }
  }
};
