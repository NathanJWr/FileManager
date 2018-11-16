#include "dirobject.h"
#include "filemanager.h"
#include <iostream>
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
