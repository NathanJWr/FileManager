#include "dirobject.h"
#include "filemanager.h"
#include <iostream>
int main() {
  FileManager fmanager;
  auto list = fmanager.listCurrentDirectory();
  for (auto n : list) {
    std::cout << n << std::endl;
  }
  return 0;
}
