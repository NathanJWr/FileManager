#ifndef MESSAGE_H_
#define MESSAGE_H_
#include <string>
class Message {
public:
	enum Type {REMOVE, QUIT, YANK, PASTE};	
	Message();
	Message(Type t);
	Message(Type t, std::string file_name);
	const std::string message() const;
	const Type type() const;
private:
	std::string _message;
	Type _type;
};
#endif // MESSAGE_H_
