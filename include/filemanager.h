#ifndef FILE_MANAGER_H_
#define FILE_MANAGER_H_
#include "dirobject.h"
#include <vector>
#include <string>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
class FileManager {
 public:
  std::string getCurrentDirectory();
  void changeDirectory(std::string path);

  /* gets the file and folder names in the current directory */
  std::vector<DirObject>& listCurrentDirectory();
  void createDirObject(std::string name);
 private:
  std::vector<DirObject> current_directory;
};
#endif // FILE_MANAGER_H_
