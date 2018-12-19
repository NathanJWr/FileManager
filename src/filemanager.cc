#include "filemanager.h"
#include <fstream>
#include <algorithm>
#include <experimental/filesystem>
FileManager::FileManager() {
  show_hidden_files = false;
  sort_alphabetically = true;
}

const std::string FileManager::getCurrentDirectory() {
  auto path = fs::current_path();
  return path.string();
}

bool FileManager::alpha_sort(DirObject i, DirObject j) {
  return (i.name[0] < j.name[0]);
}

bool FileManager::type_sort(DirObject i, DirObject j) {
  if (i.type == DirObject::FOLDER && j.type != DirObject::FOLDER) {
   return true;
  } else return false;
}

Directory FileManager::listCurrentDirectory() {
  std::string path = getCurrentDirectory();
  std::vector<DirObject> current_directory;
  for (const fs::directory_entry &p : fs::directory_iterator(path)) {
    std::string buff = p.path().string();
    std::string path_str = buff;

    // erasing the full path to just get the name of the file
    buff.erase(0, path.size());
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
  if (sort_type == ALPHABETICALLY) {
    std::sort(current_directory.begin(), current_directory.end(), alpha_sort);
  }
  if (sort_type == TYPE) {
    std::sort(current_directory.begin(), current_directory.end(), alpha_sort);
    std::sort(current_directory.begin(), current_directory.end(), type_sort);
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
