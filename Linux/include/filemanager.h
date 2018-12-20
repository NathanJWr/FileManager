#ifndef FILE_MANAGER_H_
#define FILE_MANAGER_H_
#include "directory.h"
#include "config.h"
#include <vector>
#include <string>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
class FileManager {
 public:
  FileManager();
  const std::string getCurrentDirectory();
  bool changeDirectory(std::string path);
  bool moveToParent();

  /* gets the file and folder names in the current directory */
  Directory listCurrentDirectory();
  void createDirObject(std::string name);
  bool sort_alphabetically;
 private:
  bool show_hidden_files;
  static bool alpha_sort(DirObject, DirObject);
  static bool type_sort(DirObject, DirObject);
};
#endif // FILE_MANAGER_H_
