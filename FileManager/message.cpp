#include "message.h"
Message::Message() {}
Message::Message(Type t) {
	_type = t;
	if (t == QUIT) {
		_message = "Are you sure you want to quit? Y/N";
	}
}
Message::Message(Type t, std::string file_name) {
	_type = t;
	if (t == REMOVE) {
		_message = "Are you sure you want to delete " + file_name + "? Y/N";
	} else if (t == YANK) {
		_message = "Yanked " + file_name + "!";
	} else if (t == PASTE) {
		_message = "Pasted " + file_name + "!";
	}
}
Message::Message(std::string message)
{
	_type = SHELL;
	_message = message;
}
const std::string Message::message() const {
	return _message;
}

const Message::Type Message::type() const {
	return _type;
}
