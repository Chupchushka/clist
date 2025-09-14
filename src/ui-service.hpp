#pragma once

#include "todo-service.hpp"
#include <string>

class UI {
private:
  TodoService &todo_service;

public:
  UI(TodoService &tds) : todo_service(tds) {}

  void parseArgs(int argc, char *argv[]) {

    for (int i = 1; i < argc; i++) {

      if ((std::string(argv[i]) == "add" || std::string(argv[i]) == "-a") &&
          i + 1 < argc) {
        todo_service.addTask(argv[i + 1]);
        i++;
      } else if ((std::string(argv[i]) == "remove" ||
                  std::string(argv[i]) == "-r") &&
                 i + 1 < argc) {
        int id = std::stoi(argv[i + 1]);
        todo_service.removeTask(id);
        i++;
      } else if (std::string(argv[i]) == "list" ||
                 std::string(argv[i]) == "-l") {
        todo_service.printTable();
      } else if ((std::string(argv[i]) == "mark" ||
                  std::string(argv[i]) == "-m") &&
                 i + 2 < argc) {
        int task_id = std::stoi(argv[i + 1]);
        bool done_flag = (std::string(argv[i + 2]) == "done");

        todo_service.markDone(done_flag ? 1 : 0, task_id);
        i += 2;
      } else if (std::string(argv[i]) == "addTag" ||
                 std::string(argv[i]) == "-t" && i + 3 < argc) {
        int task_id = std::stoi(argv[i + 1]);
        std::string tag_name = argv[i + 2];
        std::string tag_color = argv[i + 3];

        todo_service.addTag(task_id, tag_name.c_str(), tag_color.c_str());
        i += 3;
      } else if (std::string(argv[i]) == "help" ||
                 std::string(argv[i]) == "-h") {
        todo_service.printHelp();
      }

      else {
        todo_service.printHelp();
      }
    }
  }
};
