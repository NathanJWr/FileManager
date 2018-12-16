#include "filemanager.h"
#include <fstream>
#include <experimental/filesystem>
FileManager::FileManager() {
  show_hidden_files = false;
}

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

    // erasing the full path to just get the name of the file
    buff.erase(buff.begin(), buff.begin() + path.size());
    if (buff.front() == '/') {
      buff.erase(buff.begin());
    }
    DirObject::Type type;

    try {
      if (fs::is_directory(p)) {
        type = DirObject::FOLDER;
      } else if (fs::is_regular_file(p)) {
        type = DirObject::FILE;
      }
    } catch (fs::filesystem_error &e) {
      /*
       * Skips the file/folder that the user
       * has insufficient permissions to query
       */
    }
    DirObject dirobj = DirObject(buff, path_str, type);
    if (!dirobj.isHidden() && !show_hidden_files) {
      current_directory.emplace_back(DirObject(buff, path_str, type));
    }
  }
  return Directory(current_directory, getCurrentDirectory());
}

bool FileManager::changeDirectory(std::string path) {
  /*
   * TODO: This throws a filesystem error
   * maybe try catching it if needed?
   */
  try {
    fs::current_path(path);
  } catch (fs::filesystem_error& e) {
    return false;
  }
  return true;
}

bool FileManager::moveToParent() {
  auto path = fs::current_path();
  if (path.parent_path().string() == "") {
    return false;
  } else {
    fs::current_path(path.parent_path());
    return true;
  }
}


void FileManager::createDirObject(std::string name) {
  std::ofstream file(name);
  file.close();
}
