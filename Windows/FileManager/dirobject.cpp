#include "dirobject.h"
#include <iostream>
DirObject::DirObject(std::string n, std::string p, Type t) {
	_name = n;
	_path = p;
	_type = t;
	if (_type == FILE) {
		size_t pos = _path.find_last_of('.');
		if (pos != std::string::npos) {
			_extension = _path.substr(pos, _path.size() - 1);
		}
	} else {
		_extension = "";
	}
	selected = false;
	texture = NULL;
}

DirObject::DirObject() {

}

void DirObject::clean() {
	if (texture != NULL) {
		SDL_DestroyTexture(texture);
		texture = NULL;
	}
}
bool DirObject::isFolder() const {
	return _type == FOLDER;
}

bool DirObject::isFile() const {
	return _type == FILE;
}

bool DirObject::isHidden() const {
	return (_name[0] == '.');
}

const std::string DirObject::name() const {
	return _name;
}

const std::string DirObject::path() const {
	return _path;
}

const std::string DirObject::extension() const {
	return _extension;
}

const DirObject::Type DirObject::type() const {
	return _type;
}

std::ostream& operator << (std::ostream& os, const DirObject& f) {
	return os << f.name() << " : " << f.type();
}
