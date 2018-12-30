#include "filesystem.h"
#include <stack>
#include <windows.h>
Filesystem::Filesystem() {
	std::stack<std::string> buff;
	buff.push(fmanager.getCurrentDirectory());
	while (fmanager.moveToParent()) {
		buff.push(fmanager.getCurrentDirectory());
		std::cout << buff.top() << std::endl;
	}
	while (!buff.empty()) {
		if (fmanager.changeDirectory(buff.top())) {
			dirs.push(fmanager.listCurrentDirectory());
			buff.pop();
		}
	}
	yanked = DirObject();
}

Filesystem::Filesystem(std::string path) {
	fmanager.changeDirectory(path);
	*this = Filesystem();
}

void Filesystem::yank() {
	yanked = dirs.top().currentlySelected();
	std::cout << "Yanked: " << yanked << std::endl;
}

void Filesystem::paste() {
	if (!yanked.path().empty()) {
		std::string to = currentDir().path();
		fmanager.copy(yanked.path(), to);

		/* The yanked DirObject needs to have it's path changed to fit the new place it's in */
		DirObject to_add(yanked.name(), to + SLASH + yanked.name(), yanked.type());
		currentDir().add(to_add);
	}
}

void Filesystem::remove() {
	if (fmanager.remove(currentDir().currentlySelected().path())) {
		currentDir().remove();
	}
}

const std::string Filesystem::currentDirObjName() {
	return currentDir().currentlySelected().name();
}

void Filesystem::clean() {
	while (dirs.size() > 0) {
		dirs.top().clean();
		dirs.pop();
	}
}

Directory& Filesystem::currentDir() {
	return dirs.top();
}

void Filesystem::addCurrentDir() {
	dirs.push(fmanager.listCurrentDirectory());
}

void Filesystem::back() {
	/*
	 * Since dirs should always contain the root directory
	 * check to see if there is only one thing in the stack
	 */
	if (dirs.size() > 1) {
		dirs.top().clean();
		dirs.pop();
	}
}

void Filesystem::forward() {
	if (currentDir().currentlySelected().isFolder()) {
		currentDir().clean();
		forwardDir();
	}
	else if (currentDir().currentlySelected().isFile()) {
		openFile();
	}
}

void Filesystem::forwardDir() {
	std::string name = currentDir().currentlySelected().name();
	std::string buf;
	if (currentDir().path() != ROOT) {
		buf = currentDir().path() + SLASH + name;
	}
	else {
		buf = currentDir().path() + name;
	}
	if (fmanager.changeDirectory(buf)) {
		addCurrentDir();
	}
}

void Filesystem::openFile() {
	std::string path = currentDir().currentlySelected().path();
	std::string ext = currentDir().currentlySelected().extension();
	std::string command = "";

	/* Look through all of the defined extensions to see what matches */
	for (auto str : TEXT_EXTENSIONS) {
		if (str == ext) {
			command = TEXT_EDITOR;
		}
	}
	for (auto str : VIDEO_EXTENSIONS) {
		if (str == ext) {
			command = VIDEO_PLAYER;
		}
	}
	for (auto str : PDF_EXTENSIONS) {
		if (str == ext) {
			command = PDF_VIEWER;
		}
	}
	for (auto str : IMAGE_EXTENSIONS) {
		if (str == ext) {
			command = IMAGE_VIEWER;
		}
	}

	std::string buff = static_cast<std::string>(" \"") + path + static_cast<std::string>("\"");
	command.append(buff);
	std::cout << command << std::endl;

	/* Windows stuff to execute CreateProcessA */
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	LPSTR s = const_cast<char *>(command.c_str());
	if (!CreateProcessA(NULL, s, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
		printf("CreateProcess failed (%d).\n", GetLastError());
		return;
	}
}

void Filesystem::toggleSortAlphabetically() {
	fmanager.sort_alphabetically = !fmanager.sort_alphabetically;
}
