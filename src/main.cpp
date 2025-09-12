#include "db-service.hpp"
#include "todo-service.hpp"
#include <iostream>
#include <memory>

auto db_service = std::unique_ptr<DbService>(new DbService());

auto todo_service = std::unique_ptr<TodoService>(new TodoService(*db_service));
int main() {
  
  db_service->openDb("clist-db.db");
  todo_service -> createTable();
  todo_service->addTask("Example task");
  todo_service->addTask("Example task2");
  todo_service->addTask("Example task3");

  todo_service->markDone(true, 2);
  todo_service->removeTask(1);
  db_service->readDataStmt();
  db_service->closeDB();

  return 0;
}
