#ifndef DIR_OBJECT_H_
#define DIR_OBJECT_H_
#include <iostream>
#include <string>
class DirObject {
public:
	enum Type { FILE, FOLDER };
	bool isFolder() const;
	bool isFile() const;
	bool isHidden() const;
	Type type;
	std::string name;
	std::string path;
	std::string extension;
	bool selected;

	DirObject(std::string name, std::string path, Type type);

	friend std::ostream& operator<<(std::ostream& os, const DirObject& f);

};
#endif // DIR_OBJECT_H_
