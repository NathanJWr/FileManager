#ifndef FILE_MANAGER_H_
#define FILE_MANAGER_H_
#include "directory.h"
#include "config.h"
#include "dirobject.h"
#include <vector>
#include <string>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
class FileManager
{
public:
    FileManager();
    const std::string getCurrentDirectory();
    bool changeDirectory(std::string path);
    bool moveToParent();

    /* gets the file and folder names in the current directory */
    Directory listCurrentDirectory();
    void createDirObject(std::string name);
    void copy(std::string from, std::string to);
    bool remove(DirObject obj);
    void createDirectory(std::string path, std::string oldpath);
    void createDirectory(std::string path);
    bool sort_alphabetically;
private:
    bool show_hidden_files;
    bool touched_root;
};
#endif // FILE_MANAGER_H_

