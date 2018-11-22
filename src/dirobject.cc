#include "dirobject.h"
DirObject::DirObject(std::string n, std::string p, Type t) {
  name = n;
  path = p;
  type = t;
  selected = false;
}
std::ostream& operator << (std::ostream& os, const DirObject& f) {
  os << f.name << " : " << f.type;
  return os;
}
