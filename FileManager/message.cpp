#include "message.h"
Message::Message() {}
Message::Message(Type t)
{
    _type = t;
    switch (t)
    {
    case QUIT:
        _message = "Are you sure you want to quit? Y/N";
        break;
    case CREATE_FOLDER:
        _message = "Create Folder: ";
        break;
    case CREATE_FILE:
        _message = "Create File: ";
        break;
    }
}
Message::Message(Type t, std::string file_name)
{
    _type = t;
    if (t == REMOVE)
    {
        _message = "Are you sure you want to delete " + file_name + "? Y/N";
    }
    else if (t == YANK)
    {
        _message = "Yanked " + file_name + "!";
    }
    else if (t == PASTE)
    {
        _message = "Pasted " + file_name + "!";
    }
}
Message::Message(std::string message)
{
    _type = SHELL;
    _message = message;
}
const std::string Message::message() const
{
    return _message;
}

const Message::Type Message::type() const
{
    return _type;
}
