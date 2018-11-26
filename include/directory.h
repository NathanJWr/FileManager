#ifndef DIRECTORY_H_
#define DIRECTORY_H_
#include "dirobject.h"
#include <vector>
#include <memory>
class Directory {
 public:
  Directory(std::vector<DirObject>);
  const std::vector<DirObject>& get() const;
  
  void moveSelectedDown();
  void moveSelectedUp();

 private:
  std::vector<DirObject> dir;
  std::unique_ptr<Directory> next;
  unsigned int findCurrentlySelected();

  void moveToDirectoryLeft();
  void setNext(std::string path);
};

#endif // DIRECTORY_H_
