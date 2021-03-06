#include "filesystem.h"
Filesystem::Filesystem()
{
    std::stack<std::string> buff;
    buff.push(fmanager.getCurrentDirectory());
    while (fmanager.moveToParent())
    {
        buff.push(fmanager.getCurrentDirectory());
        std::cout << buff.top() << std::endl;
    }
    while (!buff.empty())
    {
        if (fmanager.changeDirectory(buff.top()))
        {
            dirs.push(fmanager.listCurrentDirectory());
            buff.pop();
        }
    }
    yanked = std::move(DirObject());
}

Filesystem::Filesystem(std::string path)
{
    fmanager.changeDirectory(path);
    *this = Filesystem();
}

void Filesystem::yank()
{
    dirs.top().last_move = Directory::NONE;
    yanked = dirs.top().currentlySelected();
    std::cout << "Yanked: " << yanked << std::endl;
}

void Filesystem::paste()
{
    if (!yanked.path().empty())
    {
        std::string to = currentDir().path();
        if (yanked.isFolder())
        {
            to += SLASH + yanked.name();
            fmanager.createDirectory(to, yanked.path());
        }
        fmanager.copy(yanked.path(), to);

        /* The yanked DirObject needs to have it's path changed to fit the new place it's in */
        DirObject to_add(yanked.name(), to + SLASH + yanked.name(), yanked.type());
        currentDir().add(std::move(to_add));
    }
}

void Filesystem::remove()
{
    if (fmanager.remove(currentDir().currentlySelected()))
    {
        currentDir().remove();
    }
}

void Filesystem::reloadCurrentDir()
{
    dirs.pop();
    addCurrentDir();
}

const std::string Filesystem::currentDirObjName()
{
    return currentDir().currentlySelected().name();
}

Directory& Filesystem::currentDir()
{
    return dirs.top();
}

void Filesystem::addCurrentDir()
{
    dirs.push(fmanager.listCurrentDirectory());
}

void Filesystem::back()
{
    /*
     * Since dirs should always contain the root directory
     * check to see if there is only one thing in the stack
     */
    if (dirs.size() > 1)
    {
        dirs.pop();
    }
}

void Filesystem::forward()
{
    /* This is so the dir will be redrawn correctly when it is accessed again */
    dirs.top().last_move = Directory::NONE;
    if (currentDir().currentlySelected().isFolder())
    {
        forwardDir();
    }
    else if (currentDir().currentlySelected().isFile())
    {
        openFile();
    }

}

void Filesystem::forwardDir()
{
    std::string name = currentDir().currentlySelected().name();
    std::string buf;
    if (currentDir().path() != ROOT)
    {
        buf = currentDir().path() + SLASH + name;
    }
    else
    {
        buf = currentDir().path() + name;
    }
    if (fmanager.changeDirectory(buf))
    {
        addCurrentDir();
    }
}

void Filesystem::openFile()
{
    std::string path = currentDir().currentlySelected().path();
    Platform::Execute(path);
}

void Filesystem::toggleSortAlphabetically()
{
    fmanager.sort_alphabetically = !fmanager.sort_alphabetically;
}

void Filesystem::createFolder(std::string name)
{
    std::string full_path = currentDir().path() + SLASH + name;
    fmanager.createDirectory(full_path);
}
