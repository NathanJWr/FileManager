#ifndef DIR_OBJECT_H_
#define DIR_OBJECT_H_
#include <iostream>
class DirObject {
 public:
  enum Type {FILE, FOLDER};
	bool isFolder() const;
  Type type;
  std::string name;
  std::string path;
  bool selected;

  DirObject(std::string name, std::string path, Type type);

  friend std::ostream& operator<<(std::ostream& os, const DirObject& f);

};
#endif // DIR_OBJECT_H_
