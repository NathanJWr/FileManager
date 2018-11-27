#include "filesystem.h"
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
Directory& Filesystem::currentDir() {
  return dirs.top();
}

void Filesystem::addCurrentDir() {
  dirs.push(fmanager.listCurrentDirectory());
}

void Filesystem::back() {
	dirs.pop();
}

void Filesystem::forward() {
	if (currentDir().currentlySelected().isFolder()) {
		std::string name = currentDir().currentlySelected().name;
		std::string buf = currentDir().path + "/" + name;
		fmanager.changeDirectory(buf);
		addCurrentDir();
	}
}
