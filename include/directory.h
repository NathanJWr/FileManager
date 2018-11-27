#ifndef DIRECTORY_H_
#define DIRECTORY_H_
#include "dirobject.h"
#include <vector>
#include <memory>
class Directory {
 public:
  Directory(std::vector<DirObject>, std::string path);
  const std::vector<DirObject>& get() const;
	const DirObject& currentlySelected() const;
  
  void moveSelectedDown();
  void moveSelectedUp();

	std::string path;
 private:
  std::vector<DirObject> dir;
  unsigned int findCurrentlySelected() const;
};

#endif // DIRECTORY_H_
