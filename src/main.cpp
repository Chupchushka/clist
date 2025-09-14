#include "db-service.hpp"
#include "todo-service.hpp"
#include "ui-service.hpp"

#include <iostream>
#include <memory>

auto db_service = std::unique_ptr<DbService>(new DbService());
auto todo_service = std::unique_ptr<TodoService>(new TodoService(*db_service));
auto ui_service = std::unique_ptr<UI>(new UI(*todo_service, *db_service));

int main(int argc, char *argv[]) {

  db_service->openDb("db.sqlite3");
  todo_service->createTable();
  ui_service->parseArgs(argc, argv);
  db_service->closeDB();
  return 0;
}
