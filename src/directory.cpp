#include "directory.h"
bool Directory::alpha_sort(const DirObject i, const DirObject j)
{
    std::string name1 = i.name();
    std::string name2 = j.name();
    transform(name1.begin(), name1.end(), name1.begin(), tolower);
    transform(name2.begin(), name2.end(), name2.begin(), tolower);
    return (name1.compare(name2) < 0);
}

Directory::Directory(std::vector<DirObject> d, std::string p)
{
    dir = d;
    _path = p;
    if (!dir.empty()) {
        std::sort(dir.begin(), dir.end(), alpha_sort);
        dir[0].selected = true;
    }
    last_move = NONE;
    max_dir_objs = 0;
    min_dir_objs = 0;
    selected_at = 0;
}

DirObject& Directory::currentlySelected()
{
    return dir[selected_at];
}

std::vector<DirObject>& Directory::get()
{
    return dir;
}

const std::string Directory::path() const
{
    return _path;
}

void Directory::add(DirObject obj)
{
    if (!dir.empty())
    {
        currentlySelected().selected = false;
    }
    obj.selected = true;
    dir.push_back(obj);
    std::sort(dir.begin(), dir.end(), alpha_sort);
    selected_at = findCurrentlySelected();
}

void Directory::remove()
{
    if (selected_at > 0)
    {
        dir[selected_at - 1].selected = true;
        dir.erase(dir.begin() + selected_at);
        selected_at--;
    }
    else if (selected_at == 0)
    {
        dir.erase(dir.begin() + selected_at);
        if (!dir.empty())
        {
            dir[0].selected = true;
        }
        selected_at = 0;
    }
}

void Directory::moveSelectedDown()
{
    auto currently_selected = findCurrentlySelected();
    std::cout << "Selected: " << currently_selected;
    if (dir.size() - 1 > currently_selected)
    {
        dir[currently_selected].selected = false;
        dir[currently_selected + 1].selected = true;
        std::cout << "  Moved to: " << currently_selected + 1 << std::endl;
        last_move = DOWN;
    }
}

void Directory::moveSelectedUp()
{
    auto currently_selected = findCurrentlySelected();
    std::cout << "Selected: " << currently_selected;
    if (currently_selected > 0)
    {
        dir[currently_selected].selected = false;
        dir[currently_selected - 1].selected = true;
        std::cout << "  Moved to: " << currently_selected - 1 << std::endl;
        last_move = UP;
    }
}

unsigned int Directory::findCurrentlySelected() const
{
    unsigned int currently_selected = 0;
    for (unsigned int i = 0; i < dir.size(); i++)
    {
        if (dir[i].selected)
        {
            currently_selected = i;
        }
    }
    return currently_selected;
}
