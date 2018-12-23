#include "directory.h"
#include "filemanager.h"
Directory::Directory(std::vector<DirObject> d, std::string p) {
	dir = d;
	_path = p;
	if (!dir.empty()) {
		dir[0].selected = true;
	}
}

const DirObject& Directory::currentlySelected() const {
	auto cur = findCurrentlySelected();
	return dir[cur];
}

const std::vector<DirObject>& Directory::get() const {
	return dir;
}

const std::string Directory::path() const {
	return _path;
}

void Directory::moveSelectedDown() {
	auto currently_selected = findCurrentlySelected();
	if (dir.size() - 1 > currently_selected) {
		dir[currently_selected].selected = false;
		dir[currently_selected + 1].selected = true;
	}
}

void Directory::moveSelectedUp() {
	auto currently_selected = findCurrentlySelected();
	if (currently_selected > 0) {
		dir[currently_selected].selected = false;
		dir[currently_selected - 1].selected = true;
	}
}

unsigned int Directory::findCurrentlySelected() const {
	unsigned int currently_selected = 0;
	for (unsigned int i = 0; i < dir.size(); i++) {
		if (dir[i].selected) {
			currently_selected = i;
		}
	}
	return currently_selected;
}