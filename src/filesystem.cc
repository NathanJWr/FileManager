#include "filesystem.h"
#include <stack>
Filesystem::Filesystem() {
  std::stack<std::string> buff;
  buff.push(fmanager.getCurrentDirectory());
  while (fmanager.moveToParent()) {
    buff.push(fmanager.getCurrentDirectory());
  }
  while (!buff.empty()) {
    if( fmanager.changeDirectory(buff.top())) {
      dirs.push(fmanager.listCurrentDirectory());
      buff.pop();
    }
  }
}
Directory& Filesystem::currentDir() {
  return dirs.top();
}

void Filesystem::addCurrentDir() {
  dirs.push(fmanager.listCurrentDirectory());
}

void Filesystem::back() {
  /*
   * Since dirs should always contain the root directory
   * check to see if there is only one thing in the stack
   */
  if (dirs.size() > 1) {
    dirs.pop();
  }
}

void Filesystem::forward() {
  if (currentDir().currentlySelected().isFolder()) {
    std::string name = currentDir().currentlySelected().name;
    std::string buf;
    if (currentDir().path != ROOT) {
      buf = currentDir().path + "/" + name;
    } else {
      buf = currentDir().path + name;
    }
    if (fmanager.changeDirectory(buf)) {
      addCurrentDir();
    }
  }
}
