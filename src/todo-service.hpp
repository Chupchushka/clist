#pragma once

#include "db-service.hpp"
#include "iostream"
#include <cstdio>

class TodoService {

private:
  DbService &db_service;

public:
  TodoService(DbService &dbs) : db_service(dbs) {}

  void createTable() {
    char *sql = "CREATE TABLE IF NOT EXISTS tasks("
                "task_id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "task TEXT NOT NULL );";

    db_service.execSql(sql);
  }

  void addTask(const char* task){
    char sql[256];

    snprintf(sql, sizeof(sql),"INSERT INTO tasks (task)"
                                                 "VALUES( '%s' ); ",task);
    db_service.execSql(sql);
  }

  void removeTask(int task_id){
    char sql[256];

    snprintf(sql, sizeof(sql),"DELETE FROM tasks"
                                                 "WHERE task_id = '%d';",task_id);
  }

};
