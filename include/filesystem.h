#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_
#include "directory.h"
#include "filemanager.h"
#include <stack>
class Filesystem {
 public:
  Filesystem();
  Directory& getCurrent();
  void add();
 private:
  std::stack<Directory> dirs;
  FileManager fmanager;
  unsigned int current;
};
#endif // FILESYSTEM_H_
