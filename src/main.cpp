#include "db-service.hpp"
#include <iostream>
#include <memory>

auto db_service = std::unique_ptr<DbService>(new DbService());

int main() {

  db_service->openDb("clist-db.db");
  db_service->closeDB();

  return 0;
}
