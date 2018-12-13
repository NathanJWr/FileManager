#ifndef FILE_MANAGER_H_
#define FILE_MANAGER_H_
#include "directory.h"
#include <vector>
#include <string>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
class FileManager {
 public:
	FileManager();
  std::string getCurrentDirectory();
  void changeDirectory(std::string path);
  bool moveToParent();

  /* gets the file and folder names in the current directory */
  Directory listCurrentDirectory();
  void createDirObject(std::string name);
 private:
	bool show_hidden_files;
};
#endif // FILE_MANAGER_H_
