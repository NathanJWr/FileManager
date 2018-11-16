#ifndef DIR_OBJECT_H_
#define DIR_OBJECT_H_
#include <iostream>
class DirObject {
 public:
  enum Type {FILE, FOLDER};
  DirObject(std::string name, Type type);

  friend std::ostream& operator<<(std::ostream& os, const DirObject& f);

 private:
  std::string name;
  Type type;
};
#endif // DIR_OBJECT_H_
