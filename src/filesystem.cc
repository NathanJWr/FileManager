#include "filesystem.h"
#include <queue>
#include <stack>
Filesystem::Filesystem() {
  std::stack<std::string> buff;
  buff.push(fmanager.getCurrentDirectory());
  while (fmanager.moveToParent()) {
    buff.push(fmanager.getCurrentDirectory());
  }
  while (!buff.empty()) {
    fmanager.changeDirectory(buff.top());
    dirs.push(fmanager.listCurrentDirectory());
    buff.pop();
  }
}
Directory& Filesystem::getCurrent() {
  return dirs.top();
}

void Filesystem::add() {
  dirs.push(fmanager.listCurrentDirectory());
}
