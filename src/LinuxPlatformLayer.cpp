#ifdef __unix__
#include "platformlayer.h"
bool Platform::IsFileHidden(std::string Path, std::string Name)
{
    return Name.at(0) == '.';
}

void Platform::Execute(std::string Path)
{
    std::string tmp = "xdg-open \"" + Path + "\"";
    FILE* file = popen(tmp.c_str(), "r");
    pclose(file);
}

void Platform::CMDExecute(std::string Path)
{
    FILE* file = popen(Path.c_str(), "r");
    pclose(file);
}

std::string Platform::Username()
{
    std::string username;
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    if (pw)
        username = pw->pw_name;
    return username;
}
#endif
