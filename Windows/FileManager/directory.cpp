#include "directory.h"
#include <algorithm>
#include <string>
bool Directory::alpha_sort(const DirObject i, const DirObject j) {
	std::string name1 = i.name();
	std::string name2 = j.name();
	transform(name1.begin(), name1.end(), name1.begin(), tolower);
	transform(name2.begin(), name2.end(), name2.begin(), tolower);
	return (name1.compare(name2) < 0);
}

Directory::Directory(std::vector<DirObject> d, std::string p) {
	dir = d;
	_path = p;
	if (!dir.empty()) {
		dir[0].selected = true;
	}
	std::sort(dir.begin(), dir.end(), alpha_sort);
	last_move = NONE;
	max_dir_objs = 0;
	min_dir_objs = 0;
	selected_at = 0;
}

DirObject& Directory::currentlySelected() {
	auto cur = findCurrentlySelected();
	return dir[cur];
}

std::vector<DirObject>& Directory::get() {
	return dir;
}

const std::string Directory::path() const {
	return _path;
}

void Directory::add(DirObject obj) {
	currentlySelected().selected = false;
	obj.selected = true;
	
	dir.push_back(obj);
	std::sort(dir.begin(), dir.end(), alpha_sort);
}

void Directory::remove() {
	unsigned int cur = findCurrentlySelected();
	if (cur > 0) {
		dir[cur - 1].selected = true;
	}
	dir.erase(dir.begin() + cur);
}

void Directory::moveSelectedDown() {
	auto currently_selected = findCurrentlySelected();
	std::cout << "Selected: " << currently_selected;
	if (dir.size() - 1 > currently_selected) {
		dir[currently_selected].selected = false;
		dir[currently_selected + 1].selected = true;
		std::cout << "  Moved to: " << currently_selected + 1 << std::endl;
		last_move = DOWN;
	}
}

void Directory::moveSelectedUp() {
	auto currently_selected = findCurrentlySelected();
	std::cout << "Selected: " << currently_selected;
	if (currently_selected > 0) {
		dir[currently_selected].selected = false;
		dir[currently_selected - 1].selected = true;
		std::cout << "  Moved to: " << currently_selected - 1 << std::endl;
		last_move = UP;
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