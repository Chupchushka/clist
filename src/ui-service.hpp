#pragma once

#include "db-service.hpp"
#include "todo-service.hpp"
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <string>
#include <fmt/core.h>
#include <fmt/color.h>


class UI{
  private:
    TodoService &todo_service;
    DbService &db_service;

  public:
    UI(TodoService &tds, DbService &dbs) : todo_service(tds),db_service(dbs) {}

    void parseArgs(int argc, char* argv[]){

    for (int i = 1; i < argc; i++)
    {

        if ((std::string(argv[i]) == "add" || std::string(argv[i]) == "-a") && i + 1 < argc)
        {
            todo_service.addTask(argv[i + 1]);
            i++;
        }
        else if ((std::string(argv[i]) == "remove" || std::string(argv[i]) == "-r") && i + 1 < argc)
        {
            int id = std::stoi(argv[i + 1]);
            todo_service.removeTask(id);
            i++;
        }
        else if (std::string(argv[i]) == "list" || std::string(argv[i]) == "-l")
        {
            printTableColor();
        }
        else if ((std::string(argv[i]) == "mark" || std::string(argv[i]) == "-m") && i + 2 < argc)
        {
          int task_id = std::stoi(argv[i + 1]);
          bool done_flag = (std::string(argv[i + 2]) == "done");

          todo_service.markDone(done_flag ? 1 : 0, task_id);
          i += 2;
        }
        else if (std::string(argv[i]) == "addTag" || std::string(argv[i]) == "-t" && i + 3 < argc) {
          int task_id = std::stoi(argv[i + 1]);
          std::string tag_name = argv[i + 2];
          std::string tag_color = argv[i + 3];

          todo_service.addTag(task_id, tag_name.c_str(), tag_color.c_str());
          i += 3;
        }
        else if (std::string(argv[i]) == "removeTag" || std::string(argv[i]) == "-rt" && i + 1 < argc) {
          int tag_id = std::stoi(argv[ i + 1]);
          
          todo_service.removeTag(tag_id);
        }
        else if (std::string(argv[i]) == "help" || std::string(argv[i]) == "-h")
        {
            todo_service.printHelp();
        }
        
        else
        {
          std::cerr << "Incorrect command: " << argv[i] << " Please use clist help or clist -h to see list pf commands. " << std::endl;
        }
    }

  }
  
  fmt::color parseColors(std::string color_name){
    if (color_name == "white" || color_name == "White"){
      return fmt::color::white;
    } else if (color_name == "red" || color_name == "Red"){
      return fmt::color::red;
    } else if (color_name == "blue" || color_name == "Blue"){
      return fmt::color::blue;
    } else if (color_name == "green" || color_name == "Green"){
      return fmt::color::green;
    } else if (color_name == "yellow" || color_name == "Yellow"){
      return fmt::color::yellow;
    } else if (color_name == "purple" || color_name == "Purple"){
      return fmt::color::purple;
    } else if (color_name == "pink" || color_name == "Pink"){
      return fmt::color::pink;
    } else {
      return fmt::color::white;
    }
  }

  
static int callback(void *data, int count, char **argv, char **columnNames) {
    UI *ui = static_cast<UI*>(data);

    // Find the color column for this row
    fmt::color parsed = fmt::color::white; // default
    for (int i = 0; i < count; i++) {
        if (std::string(columnNames[i]) == "color" && argv[i] != nullptr) {
            parsed = ui->parseColors(argv[i]);
            break;  
      }
    }

    // Print the whole row with that color
    for (int i = 0; i < count; i++) {
        fmt::print(fg(parsed) | fmt::emphasis::bold,
                   "{}: {}\n", columnNames[i], argv[i] ? argv[i] : "NULL");
    }

    fmt::print("\n");  
    return 0;
}



  void printTableColor(){
    char *errMsg = 0;
    char *sql = "SELECT task_id, task, done, tasks.tag_id, name, color FROM tasks "
                "LEFT JOIN tags ON tags.tag_id = tasks.tag_id ; ";

    int rc = sqlite3_exec(db_service.pDB, sql, callback, this, &errMsg);
    if (rc != SQLITE_OK) {
      printf("Error in executing SQL: %s \n", errMsg);
      sqlite3_free(errMsg);
    } else {
      printf("read data successfully \n");
    }
  }

};
