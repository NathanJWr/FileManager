#include "directory.h"
#include "filemanager.h"
Directory::Directory(std::vector<DirObject> d) {
  dir = d;
  dir[0].selected = true;
}

const std::vector<DirObject>& Directory::get() const {
  return dir;
}

void Directory::moveSelectedDown() {
  auto currently_selected = findCurrentlySelected();
  if (dir.size()-1 > currently_selected) {
    dir[currently_selected].selected = false;
    dir[currently_selected+1].selected = true;
  }
}

void Directory::moveSelectedUp() {
  auto currently_selected = findCurrentlySelected();
  if (currently_selected > 0) {
    dir[currently_selected].selected = false;
    dir[currently_selected-1].selected = true;
  }
}
unsigned int Directory::findCurrentlySelected() {
  unsigned int currently_selected = -1;
  for (unsigned int i = 0; i < dir.size(); i++) {
    if (dir[i].selected) {
      currently_selected = i;
    }
  }
  return currently_selected;
}
