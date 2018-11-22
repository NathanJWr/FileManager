#include "dirobject.h"
DirObject::DirObject(std::string name, Type type) : 
  name{name},
  type{type} {
}
std::ostream& operator << (std::ostream& os, const DirObject& f) {
  os << f.name << " : " << f.type;
  return os;
}
