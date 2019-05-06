#ifndef FILE_MANAGER_H_
#define FILE_MANAGER_H_
namespace fs = std::experimental::filesystem;
class FileManager
{
public:
    FileManager();
    const std::string getCurrentDirectory();

	/* Sets the current path to arg passed.
	 * Returns true if successfully changed.
	 * Returns false if unsucessful */
    bool changeDirectory(std::string path);

	/* Sets the current path to the parent directory of whatever the
     * current directory is. Returns true if successful. Returns false
	 * if unsucessful */
    bool moveToParent();

    /* Gets the file and folder names in the current directory */
    Directory listCurrentDirectory();

	/* Creates a file in the current directory */
    void createDirObject(std::string name);

	/* Copies a directory or file recursively from one path to another.
	 * Returns true if successful. Returns false otherwise */
    bool copy(std::string from, std::string to);
    bool remove(DirObject obj);
    void createDirectory(std::string path, std::string oldpath);
    void createDirectory(std::string path);
    bool sort_alphabetically;
private:
    bool show_hidden_files;
    bool touched_root;
};
#endif // FILE_MANAGER_H_

