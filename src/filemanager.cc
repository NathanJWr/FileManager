#include "filemanager.h"
#include "dirobject.h"
#include <fstream>
#include <experimental/filesystem>
std::string FileManager::getCurrentDirectory() {
  auto path = fs::current_path();
  return path.string();
}

Directory FileManager::listCurrentDirectory() {
  std::string path = getCurrentDirectory();
  std::vector<DirObject> current_directory;
  for (const fs::directory_entry &p : fs::directory_iterator(path)) {
    std::string buff = p.path().string();
    std::string path_str = buff;

    //erasing the full path to just get the name of the file
    buff.erase(buff.begin(), buff.begin() + path.size() + 1);
    DirObject::Type type;

    if (fs::is_directory(p)) {
      type = DirObject::FOLDER;
    } else if (fs::is_regular_file(p)) {
      type = DirObject::FILE;
    }
    DirObject f(buff, path_str, type);
    current_directory.push_back(f);
  }
  return Directory(current_directory);
}

void FileManager::changeDirectory(std::string path) {
  /*
   * TODO: This throws a filesystem error
   * maybe try catching it if needed?
   */
  fs::current_path(path);
}

void FileManager::createDirObject(std::string name) {
  std::ofstream file(name);
  file.close();
}
