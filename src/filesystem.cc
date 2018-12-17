#include "filesystem.h"
#include "config.h"
#include <stack>
#include <stdlib.h>
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
    forwardDir();
  } else if (currentDir().currentlySelected().isFile()) {
    openFile();
  }
}

void Filesystem::forwardDir() {
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

void Filesystem::openFile() {
  std::string path = currentDir().currentlySelected().path;
  std::string ext = currentDir().currentlySelected().extension;
  std::string command = "";

  /* Look through all of the defined extensions to see what matches */
  for (auto str : TEXT_EXTENSIONS) {
    if (str == ext) {
      command = (std::string) TEXT_EDITOR;
    }
  }
  for (auto str : VIDEO_EXTENSIONS) {
    if (str == ext) {
      command = (std::string) VIDEO_PLAYER;
    }
  }
  std::string buff = (std::string) " " + "\"" + path + "\"";
  command.append(buff);

  popen(command.c_str(), "r");
}
