#pragma once

#include "db-service.hpp"
#include "iostream"
#include <cstdio>
#include <string>

class TodoService {

private:
  DbService &db_service;

public:
  TodoService(DbService &dbs) : db_service(dbs) {}

  void createTable() {
    char *sql = "CREATE TABLE IF NOT EXISTS tasks("
                "task_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "task TEXT NOT NULL, "
                "done INTEGER NOT NULL); ";

    db_service.execSql(sql);
  }

  void addTask(const char *task) {
    char sql[256];

    snprintf(sql, sizeof(sql),
             "INSERT INTO tasks (task,done)"
             "VALUES( '%s', 0); ",
             task);
    db_service.execSql(sql);
  }

  void removeTask(int task_id) {
    char sql[256];

    snprintf(sql, sizeof(sql),
             "DELETE FROM tasks "
             "WHERE task_id = %d; ",
             task_id);
    db_service.execSql(sql);
  }

  void markDone(int done, int task_id) {
    if (done != 1 && done != 0) {
      std::cerr << "Incorrect value " << done << std::endl;
    } else {
      char sql[256];

      snprintf(sql, sizeof(sql),
               "UPDATE tasks "
               "SET done = %d "
               "WHERE task_id = %d;",
               done, task_id);

      db_service.execSql(sql);
    }
  }
};
