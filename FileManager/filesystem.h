#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_
#include "directory.h"
#include "dirobject.h"
#include "filemanager.h"
#include "config.h"
#include <stack>
class Filesystem
{
public:
	/* makes a list of dirs starting from current and moving back to root */
	Filesystem();
	/* changes the directory and THEN creates the Filesystem */
	Filesystem(std::string path);
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
	/* Yank the current DirObject */
	void yank();
	/* Paste the yanked DirObject in the Current Directory */
	void paste();
	/* Delete the current DirObject */
	void remove();
	/* Create a Folder in the current path */
	void createFolder(std::string name);

	void reloadCurrentDir();
	const std::string currentDirObjName();

	DirObject yanked;
private:
	void forwardDir();
	void openFile();
	std::stack<Directory> dirs;
	FileManager fmanager;
};
#endif // FILESYSTEM_H_

