#include "filemanager.h"
FileManager::FileManager()
{
    show_hidden_files = false;
    sort_alphabetically = true;
    touched_root = false;
}

const std::string FileManager::getCurrentDirectory()
{
    auto path = fs::current_path();
    return path.string();
}

Directory FileManager::listCurrentDirectory()
{
    std::string path = getCurrentDirectory();
    std::vector<DirObject> current_directory;
    for (const fs::directory_entry &p : fs::directory_iterator(path))
    {
        std::string buff = p.path().string();
        std::string path_str = buff;

        // erasing the full path to just get the name of the file
        buff.erase(0, path.size());
        if (buff.front() == SLASH[0])
        {
            buff.erase(buff.begin());
        }
        DirObject::Type type;

        try
        {
            if (fs::is_directory(p))
            {
                type = DirObject::FOLDER;
            }
            else if (fs::is_regular_file(p))
            {
                type = DirObject::FILE;
            }
            else
            {
                type = DirObject::FILE;
            }
        }
        catch (fs::filesystem_error)
        {
            /*
             * Skips the file/folder that the user
             * has insufficient permissions to query
             */
        }
        DirObject dirobj = DirObject(buff, path_str, type);
        if (!dirobj.isHidden() && !show_hidden_files)
        {
            current_directory.emplace_back(DirObject(buff, path_str, type));
        }
    }
    return Directory(current_directory, getCurrentDirectory());
}

bool FileManager::changeDirectory(std::string path)
{
    /*
     * TODO: This throws a filesystem error
     * maybe try catching it if needed?
     */
    try
    {
        fs::current_path(path);
    }
    catch (fs::filesystem_error)
    {
        return false;
    }
    return true;
}

bool FileManager::moveToParent()
{
    auto path = fs::current_path();
    if (path.string() == ROOT && touched_root)
    {
        return false;
    }
    else if (path.parent_path().string() == ROOT)
    {
        fs::current_path(path.parent_path());
        touched_root = true;
        return true;
    }
    else
    {
        fs::current_path(path.parent_path());
        return true;
    }
}

void FileManager::createDirObject(std::string name)
{
    std::ofstream file(name);
    file.close();
}

void FileManager::copy(std::string from, std::string to)
{
    fs::copy(from, to, fs::copy_options::recursive);
}

bool FileManager::remove(DirObject obj)
{
    std::error_code e;
    if (obj.type() == DirObject::FILE)
    {
        return fs::remove(obj.path(), e);
    }
    else if (obj.type() == DirObject::FOLDER)
    {
        if (fs::remove_all(obj.path(), e) == static_cast<std::uintmax_t>(-1))
        {
            std::cout << e;
            return false;
        }
        else
        {
            return true;
        }
    }
    return false;
}

void FileManager::createDirectory(std::string path, std::string oldpath)
{
    fs::create_directory(path, oldpath);
}

void FileManager::createDirectory(std::string path)
{
    std::cout << "Creating Directory: " << path;
    fs::create_directory(path);
}
