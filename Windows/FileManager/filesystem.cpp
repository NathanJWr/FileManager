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
		dirs.pop();
	}
}

void Filesystem::forward() {
	if (currentDir().currentlySelected().isFolder()) {
		forwardDir();
	}
	else if (currentDir().currentlySelected().isFile()) {
		openFile();
	}
}

void Filesystem::forwardDir() {
	std::string name = currentDir().currentlySelected().name;
	std::string buf;
	if (currentDir().path != ROOT) {
		buf = currentDir().path + "/" + name;
	}
	else {
		buf = currentDir().path + name;
	}
	if (fmanager.changeDirectory(buf)) {
		addCurrentDir();
	}
}

void Filesystem::openFile() {
	std::string path = currentDir().currentlySelected().path;
	std::string ext = currentDir().currentlySelected().extension;
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

	std::string buff = static_cast<std::string>(" ") + "\"" + path + "\"";
	command.append(buff);
	std::cout << command << std::endl;

	//system(command.c_str());
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
