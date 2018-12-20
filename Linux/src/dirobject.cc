#include "dirobject.h"
DirObject::DirObject(std::string n, std::string p, Type t) {
  name = n;
  path = p;
  type = t;
  if (type == FILE) {
    size_t pos = path.find_last_of('.');
    if (pos != std::string::npos) {
      extension = path.substr(pos, path.size() - 1);
    }
  } else {
    extension = "";
  }
  selected = false;
}
bool DirObject::isFolder() const {
  return type == FOLDER;
}

bool DirObject::isFile() const {
  return type == FILE;
}

bool DirObject::isHidden() const {
  return (name[0] == '.');
}

std::ostream& operator << (std::ostream& os, const DirObject& f) {
  os << f.name << " : " << f.type;
  return os;
}
