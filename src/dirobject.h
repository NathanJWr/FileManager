#ifndef DIR_OBJECT_H_
#define DIR_OBJECT_H_
class DirObject
{
public:
    bool changed;
    enum Type { FILE, FOLDER };
    DirObject();
    DirObject(std::string name, std::string path, Type type);
    bool isFolder() const;
    bool isFile() const;
    bool isHidden() const;

    const std::string name() const;
    const std::string path() const;
    const std::string extension() const;
    const Type type() const;

    bool selected; // left public because it's changed often

    friend std::ostream& operator<<(std::ostream& os, const DirObject& f);
private:
    Type _type;
    std::string _name;
    std::string _path;
    std::string _extension;
};
#endif // DIR_OBJECT_H_
