#ifndef DIRECTORY_H_
#define DIRECTORY_H_
#include "dirobject.h"
#include <vector>
class Directory {
 public:
  Directory(std::vector<DirObject>);
  const std::vector<DirObject>& get() const;
  
  void moveSelectedDown();
  void moveSelectedUp();

 private:
  std::vector<DirObject> dir;
  unsigned int findCurrentlySelected();
};

#endif // DIRECTORY_H_
