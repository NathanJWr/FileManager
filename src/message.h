#ifndef MESSAGE_H_
#define MESSAGE_H_
class Message
{
public:
    enum Type {REMOVE, QUIT, YANK, PASTE, SHELL, CREATE_FOLDER, CREATE_FILE};
    Message();
    Message(Type t);
    Message(Type t, std::string file_name);
    Message(std::string message);
    const std::string message() const;
    const Type type() const;
private:
    std::string _message;
    Type _type;
};
#endif // MESSAGE_H_
