#ifndef DIRECTORY_H_
#define DIRECTORY_H_
#include "dirobject.h"
#include <vector>
#include <memory>
class Directory
{
public:
    enum LastMove {UP, DOWN, NONE};
    LastMove last_move;
    Directory(std::vector<DirObject>, std::string path);

    std::vector<DirObject>& get();
    DirObject& currentlySelected();
    const std::string path() const;

    /* selects the item in the array that is +1 from currentlySelected() */
    void moveSelectedDown();
    /* selects the item in the array that is -1 from currentlySelected() */
    void moveSelectedUp();
    /* adds a DirObject to the vector of directories */
    void add(DirObject obj);
    /* removes the currently selected DirObject */
    void remove();

    /* To keep track of what is displayed on screen */
    unsigned int max_dir_objs;
    unsigned int min_dir_objs;
    unsigned int selected_at;
private:
    std::vector<DirObject> dir;
    /* looks through dir to find the currently selected DirObject */
    unsigned int findCurrentlySelected() const;

    std::string _path;
    static bool alpha_sort(DirObject, DirObject);
};

#endif // DIRECTORY_H_
