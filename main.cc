#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
class DirObject {
 public:
  enum Type {FILE, FOLDER};
  DirObject(std::string name, Type type);

  friend std::ostream& operator<<(std::ostream& os, const DirObject& f);

 private:
  std::string name;
  Type type;
};

DirObject::DirObject(std::string name, Type type) : 
  name{name},
  type{type} {
}

std::ostream& operator << (std::ostream& os, const DirObject& f) {
  os << f.name << " : " << f.type;
}

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

std::string FileManager::getCurrentDirectory() {
  auto path = fs::current_path();
  return path.string();
}

std::vector<DirObject>& FileManager::listCurrentDirectory() {
  std::string path = getCurrentDirectory();
  for (const fs::directory_entry &p : fs::directory_iterator(path)) {
    std::string buff = p.path().string();
    buff.erase(buff.begin(), buff.begin() + path.size() + 1);
    DirObject::Type type;
    if (fs::is_directory(p)) {
      type = DirObject::FOLDER;
    } else if (fs::is_regular_file(p)) {
      type = DirObject::FILE;
    }
    DirObject f(buff, type);
    current_directory.push_back(f);
  }
  return current_directory;
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

int main() {
  FileManager fmanager;
  //std::cout << fmanager.getCurrentDirectory() << std::endl;
 // fmanager.changeDirectory("/home/njw/");
  auto list = fmanager.listCurrentDirectory();
  for (auto n : list) {
    std::cout << n << std::endl;
  }
  //std::cout << fmanager.getCurrentDirectory() << std::endl;
  return 0;
}
