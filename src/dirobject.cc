#include "dirobject.h"
DirObject::DirObject(std::string n, std::string p, Type t) {
  name = n;
  path = p;
  type = t;
  selected = false;
}
bool DirObject::isFolder() const {
	return type == FOLDER;
}

bool DirObject::isHidden() const {
	return (name.front() == '.');
}

std::ostream& operator << (std::ostream& os, const DirObject& f) {
  os << f.name << " : " << f.type;
  return os;
}
