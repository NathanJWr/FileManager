#ifndef INPUT_H_
#define INPUT_H_
#include "platformlayer.h"
#include "display.h"
#include "filesystem.h"
#include "shortcutbar.h"
#include "message.h"
#include "context.h"

std::string getKeyPress(SDL_Event& e);
int handleTypingInput(std::string& command,
                      Context& ptx,
                      SDL_Event& e,
                      Display& display,
                      Filesystem& dirs,
                      ShortcutBar& bar);
HandleType handleKeys(SDL_KeyboardEvent &e,
                      Filesystem &dirs,
                      Message &message,
                      Display &display);
Context handleMessageResponse(SDL_Event &e,
                              Display& display,
                              Filesystem &dirs,
                              ShortcutBar& bar,
                              Message msg);
void handleMouse(Filesystem &dirs, ShortcutBar &bar);
Context handleInput(SDL_Event &e,
                    Filesystem &dirs,
                    ShortcutBar &bar,
                    Display &display);
void handleShellInput(SDL_Event& e,
                      Display& display,
                      Filesystem& dirs,
                      ShortcutBar& bar);
char to_lowercase(char c);
#endif
