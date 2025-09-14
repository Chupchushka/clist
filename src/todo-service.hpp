#pragma once

#include "db-service.hpp"
#include "iostream"
#include <cstddef>
#include <cstdio>
#include <sqlite3.h>
#include <string>

class TodoService {

private:
  DbService &db_service;

public:
  TodoService(DbService &dbs) : db_service(dbs) {}

  void createTable() {

    // Create table tasks
    char *sql = "CREATE TABLE IF NOT EXISTS tasks("
                "task_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "task TEXT NOT NULL, "
                "done INTEGER NOT NULL, "
                "tag_id INTEGER, "
                "FOREIGN KEY (tag_id) REFERENCES tags(tag_id) ); ";

    db_service.execSql(sql);

    // Create table tags
    char *sqlTags = "CREATE TABLE IF NOT EXISTS tags( "
                    "tag_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "name TEXT NOT NULL, "
                    "color TEXT NOT NULL ); ";

    db_service.execSql(sqlTags);
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

      // Update db
      char sql[256];

      snprintf(sql, sizeof(sql),
               "UPDATE tasks "
               "SET done = %d "
               "WHERE task_id = %d;",
               done, task_id);

      db_service.execSql(sql);
    }
  }

  void addTag(int task_id, const char *tag_name, const char *tag_color) {

    // Adding the tag to tags
    const char *sqlTag = "INSERT INTO tags (name, color )"
                         "VALUES (?, ?) ; ";
    sqlite3_stmt *stmtTag;

    std::cout << sqlTag << std::endl;

    if (sqlite3_prepare_v2(db_service.pDB, sqlTag, -1, &stmtTag, nullptr) !=
        SQLITE_OK) {
      std::cerr << "Prepare failed " << sqlite3_errmsg(db_service.pDB)
                << std::endl;
    }

    sqlite3_bind_text(stmtTag, 1, tag_name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmtTag, 2, tag_color, -1, SQLITE_TRANSIENT);

    sqlite3_step(stmtTag);

    sqlite3_finalize(stmtTag);

    int tag_id = sqlite3_last_insert_rowid(db_service.pDB);

    // Update the tasks table setting the tag_id
    const char *sqlUpdate = "UPDATE tasks SET tag_id = ? WHERE task_id = ?; ";
    sqlite3_stmt *stmtUpdate;

    if (sqlite3_prepare_v2(db_service.pDB, sqlUpdate, -1, &stmtUpdate,
                           nullptr) != SQLITE_OK) {
      std::cerr << "Prepare failed " << sqlite3_errmsg(db_service.pDB)
                << std::endl;
    }

    sqlite3_bind_int(stmtUpdate, 1, tag_id);
    sqlite3_bind_int(stmtUpdate, 2, task_id);

    if (sqlite3_step(stmtUpdate) != SQLITE_DONE) {
      std::cerr << "Execution failed: " << sqlite3_errmsg(db_service.pDB);
      sqlite3_finalize(stmtUpdate);
    }

    sqlite3_finalize(stmtUpdate);

    std::cout << "tag added with id = " << tag_id << "to task " << task_id
              << std::endl;
  }
  
  void removeTag(int tag_id){

    // Set the tag_id to deafault tag
    const char *taskRemove = "UPDATE tasks "
                             "SET tag_id = 0 "
                             "WHERE tag_id = ? ;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db_service.pDB, taskRemove, -1, &stmt, nullptr) !=
        SQLITE_OK) {
        std::cerr << "Prepare failed " << sqlite3_errmsg(db_service.pDB)
                << std::endl;
    }
  
    sqlite3_bind_int(stmt, 1, tag_id);
    sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    // Delete tag from tags table
    const char *tagRemove = "DELETE FROM tags "
                            "Where tag_id = ? ";
    sqlite3_stmt *tag_stmt;

    if (sqlite3_prepare_v2(db_service.pDB, tagRemove, -1, &tag_stmt, nullptr) !=
        SQLITE_OK) {
        std::cerr << "Prepare failed " << sqlite3_errmsg(db_service.pDB)
                << std::endl;
    }
    
    sqlite3_bind_int(stmt, 1, tag_id);
    sqlite3_step(stmt);

    sqlite3_finalize(tag_stmt);

  }

  void addDefaultTag(){
    const char *sql = "INSERT INTO tags (name) "
                      "VALUES ('No tag') ;" ;
    db_service.execSql(sql);

  }

  void printTable() { db_service.readDataStmt(); }

  void printHelp() {
    std::cout << "Clist help message" << std::endl;
    std::cout << "Command list:" << std::endl;
  }
};
