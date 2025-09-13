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
  void readDataStmt() {
    char *sql = "SELECT * FROM 'tasks'; ";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(pDB, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
      printf("error occurred: %s", sqlite3_errmsg(pDB));
    } else {

      int NoOfCols =
          sqlite3_column_count(stmt); // get the number of columns in the table
      bool done = false;
      while (!done) { // while we are still reading rows from the table
        switch (sqlite3_step(stmt)) {
        case SQLITE_ROW: // case sqlite3_step has another row ready
          for (int i = 0; i < NoOfCols; i++) { // iterate through the columns
                                               // and get data for each column

            const char *colName =
                sqlite3_column_name(stmt, i); // get the column name
            const unsigned char *text = sqlite3_column_text(
                stmt, i); // get the value from at that column as text
            printf("Col: %s = %s \n", colName, text);
          }
          printf("\n");

          break;
        case SQLITE_DONE: // case sqlite3_step() has finished executing
          printf("done reading all rows \n");
          sqlite3_finalize(stmt); // destroy the prepared statement object
          done = true;
          break;
        }
      }
    }
  }
};
