#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_
#include "directory.h"
#include "filemanager.h"
#include "config.h"
#include <stack>
class Filesystem {
public:
	/* makes a list of dirs starting from current and moving back to root */
	Filesystem();
	/* get the current directory */
	Directory& currentDir();
	/* add the current directory to the list of dirs */
	void addCurrentDir();
	/* go back a directory by popping the current */
	void back();
	/* move into a directory or open a file */
	void forward();
	/* change the FileManager settings to sort alphabetically */
	void toggleSortAlphabetically();
private:
	void forwardDir();
	void openFile();
	std::stack<Directory> dirs;
	FileManager fmanager;
};
#endif // FILESYSTEM_H_

