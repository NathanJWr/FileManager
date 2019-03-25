#ifndef CONTEXT_H_
#define CONTEXT_H_
#include "message.h"
class Context {
public:
    bool redraw;
    bool exit;
    bool message;
    bool response;
    bool shell_input;
    Message msg;
    Context() : redraw(0), exit(0),
                message(0), response(0),
                shell_input(0), msg() {}
};

enum HandleType {NONE, MESSAGE_CONSOLE, CONSOLE, SHELL};
#endif
