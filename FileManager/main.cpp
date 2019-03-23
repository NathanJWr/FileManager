#include "display.h"
#include "filesystem.h"
#include "shortcutbar.h"
#include "message.h"
#include "platformlayer.h"

#include <iostream>
#include <memory>
#include <vector>
#include <list>

class Context {
public:
    bool redraw;
    bool exit;
    bool message;
    bool response;
    bool shell_input;
    Message msg;
    Context() : redraw(0), exit(0), message(0), response(0), shell_input(0), msg() {}
};

enum HandleType {NONE, MESSAGE_CONSOLE, CONSOLE, SHELL};

void drawAll(Display& display,
    Filesystem& dirs,
    ShortcutBar& shortcut_bar,
    Context ctx,
    bool new_message)
{
    display.renderDirectory(dirs.currentDir());
    if (ctx.message)
    {
        dirs.currentDir().last_move = Directory::NONE;
        display.renderUI(shortcut_bar, ctx.msg.message(), new_message);
    }
    else
    {
        display.renderUI(shortcut_bar);
    }

    display.update();
}

char to_lowercase(char c)
{
    if (c >= 'A' && c <= 'Z')
        return c + 32;

    return c;
}

std::string getKeyPress(SDL_Event& e)
{
    const uint8_t *state = SDL_GetKeyboardState(NULL);
    std::string tmp;
    std::string key;
    tmp = SDL_GetKeyName(e.key.keysym.sym);
    if (state[SDL_SCANCODE_LSHIFT] && e.key.keysym.sym != SDLK_LSHIFT)
    {
        key = tmp;
    }
    if (!state[SDL_SCANCODE_LSHIFT]) // Change to lowercase
    {
        for (char& c : tmp)
        {
            c = to_lowercase(c);
        }
        key = tmp;
    }
    if (state[SDL_SCANCODE_SPACE])
    {
        key = " ";
    }
    return key;
}

/* returns 0 on success
    returns 1 on exit/esacpe command */
int handleTypingInput(std::string& command,
    Context& ptx,
    SDL_Event& e,
    Display& display,
    Filesystem& dirs,
    ShortcutBar& bar)
{
    Context ctx;
    while (1)//ctx.msg.message() != "return")
    {
        ctx = Context();
        SDL_WaitEvent(&e);
        ctx.message = true;
        if (e.type == SDL_KEYDOWN)
        {
            ctx.msg = Message(getKeyPress(e));
            if (ctx.msg.message() == "return") return 0;
            std::cout << ctx.msg.message();
            if (!ctx.msg.message().empty()
                && ctx.msg.message() != "shift"
                && ctx.msg.message() != "return")
            {
                if (ctx.msg.message() == "escape")
                {
                    return 1;
                }
                if (ctx.msg.message() == "backspace")
                {
                    display.popShellLetter();
                    if (command.size() > 0) command.pop_back();
                    drawAll(display, dirs, bar, ctx, false);
                    continue;
                }
                drawAll(display, dirs, bar, ctx, false);
                command += ctx.msg.message();
            }
        }
        SDL_PollEvent(&e);
    }
}

HandleType handleKeys(SDL_KeyboardEvent &e, Filesystem &dirs, Message &message, Display &display)
{
    switch (e.keysym.sym)
    {
        case SDLK_y:
            dirs.yank();
            message = Message(Message::YANK, dirs.currentDirObjName());
            return CONSOLE;
            break;
        case SDLK_p:
            dirs.paste();
            message = Message(Message::PASTE, dirs.currentDirObjName());
            return CONSOLE;
            break;
        case SDLK_d:
            message = Message(Message::REMOVE, dirs.currentDirObjName());
            return MESSAGE_CONSOLE;
            break;
        case SDLK_j:
            dirs.currentDir().moveSelectedDown();
            break;
        case SDLK_k:
            dirs.currentDir().moveSelectedUp();
            break;
        case SDLK_l:
            dirs.forward();
            break;
        case SDLK_h:
            dirs.back();
            break;
        case SDLK_a:
            dirs.toggleSortAlphabetically();
            break;
        case SDLK_s:
            return SHELL;
            break;
        case SDLK_f:
            message = Message(Message::CREATE_FOLDER);
            return MESSAGE_CONSOLE;
            break;
        case SDLK_c:
            message = Message(Message::CREATE_FILE);
            return MESSAGE_CONSOLE;
            break;
        case SDLK_q:
            message = Message(Message::QUIT);
            return MESSAGE_CONSOLE;
            break;
        case SDLK_RIGHTBRACKET:
            display.increaseFont();
            break;
        case SDLK_LEFTBRACKET:
            display.decreaseFont();
            break;
        default:
            dirs.currentDir().last_move = Directory::NONE;
            break;
    }
    return NONE;
}

Context handleMessageResponse(SDL_Event &e, Display& display, Filesystem &dirs, ShortcutBar& bar, Message msg) {
    Context ctx;
    if (msg.type() == Message::CREATE_FOLDER)
    {
        std::string folder_name;
        if (handleTypingInput(folder_name, ctx, e, display, dirs, bar) == 0)
        {
            std::cout << "Creating folder: " << folder_name << std::endl;
            dirs.createFolder(folder_name);
            dirs.reloadCurrentDir();
        }
        display.clearShellLetters();
        ctx.redraw = true;
    }
    if (msg.type() == Message::CREATE_FILE)
    {
        std::string file_name;
        if (handleTypingInput(file_name, ctx, e, display, dirs, bar) == 0)
        {
            std::cout << "Creating file: " << file_name << std::endl;
#ifdef _WIN32
            CreateFileA(file_name.c_str(), 0, 0, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
#endif
            dirs.reloadCurrentDir();
        }
        display.clearShellLetters();
        ctx.redraw = true;
    }
    else
    {
        switch (e.key.keysym.sym) {
        case SDLK_y:
            switch (msg.type()) {
                case Message::REMOVE:
                    dirs.remove();
                    ctx.redraw = true;
                    break;
                case Message::QUIT:
                    ctx.exit = true;
                    break;
            }
            break;
        case SDLK_n:
            ctx.redraw = true;
            break;
        }
    }
    return ctx;
}

void handleMouse(Filesystem &dirs, ShortcutBar &bar)
{
    std::string path;
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    if (bar.checkClicks(mouse_x, mouse_y, path))
    {
        DirObject yanked = std::move(dirs.yanked);
        dirs = Filesystem(path);
        dirs.yanked = std::move(yanked);
    }
}

Context handleInput(SDL_Event &e, Filesystem &dirs, ShortcutBar &bar, Display &display)
{
    Context ctx;
    switch (e.type)
    {
        case SDL_QUIT:
        {
            ctx.exit = true;
            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        {
            ctx.redraw = true;
            dirs.currentDir().last_move = Directory::NONE;
            handleMouse(dirs, bar);
            break;
        }
        case SDL_KEYDOWN:
        {
            ctx.redraw = true;
            HandleType val = handleKeys(e.key, dirs, ctx.msg, display);
            if (val == MESSAGE_CONSOLE)
            {
                ctx.message = true;
                ctx.response = true;
                dirs.currentDir().last_move = Directory::NONE;
            }
            else if (val == CONSOLE)
            {
                ctx.message = true;
                dirs.currentDir().last_move = Directory::NONE;
            }
            else if (val == SHELL)
            {
                ctx.shell_input = true;
            }
            break;
        }
        case SDL_WINDOWEVENT:
            switch(e.window.event)
            {
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                case SDL_WINDOWEVENT_RESIZED:
                {
                    dirs.currentDir().last_move = Directory::NONE;
                    display.resize();
                    drawAll(display, dirs, bar, ctx, false);
                    break;
                }
                case SDL_WINDOWEVENT_FOCUS_GAINED:
                case SDL_WINDOWEVENT_EXPOSED:
                {
                    dirs.currentDir().last_move = Directory::NONE;
                    std::cout << "Focused Gained, redrawing window!\n";
                    drawAll(display, dirs, bar, ctx, false);
                    break;
                }
            }
    }
    return ctx;
}

void handleShellInput(SDL_Event& e,
        Display& display,
        Filesystem& dirs,
        ShortcutBar& bar)
{
    std::string command;
    Context shell;
    if (handleTypingInput(command, shell, e, display, dirs, bar) == 0)
    {
        std::cout << "Executing: " << command << std::endl;
        Platform::CMDExecute(command);
        //FILE* file = popen(command.c_str(), "r");
        //pclose(file);
        //ShellExecute(NULL, "open", "cmd.exe", "/C del /P test", NULL, SW_SHOWNORMAL);
    }
    display.clearShellLetters();
    dirs.reloadCurrentDir();
    drawAll(display, dirs, bar, shell, false);
}

int MAIN()
{
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        success = false;
    }
    if (TTF_Init() == -1)
    {
        std::cerr << "Failed to initialize TTF: " << TTF_GetError() << std::endl;
        success = false;
    }
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        std::cerr << "Failed to initialize IMG: " << IMG_GetError() << std::endl;
        success = false;
    }
    if (success)
    {
        Filesystem dirs;
        ShortcutBar shortcut_bar;
        Display display(1280, 800, 32);

        /* initial render */
        display.update();
        display.renderDirectory(dirs.currentDir());
        display.renderUI(shortcut_bar);
        display.update();

        SDL_Event e;
        /* There are no messages on the console that need to be handled */
        bool handle_message = false;
        Message msg; // The message sent to messageHandler

        while (1)
        {
            Context ctx;
            SDL_WaitEvent(&e);

            if (handle_message)
            {
                ctx = handleMessageResponse(e, display, dirs, shortcut_bar, msg);
            }
            else
            {
                ctx = handleInput(e, dirs, shortcut_bar, display);
            }

            if (ctx.shell_input)
            {
                handleShellInput(e, display, dirs, shortcut_bar);
            }

            if (ctx.redraw)
            {
                if (ctx.message) msg = ctx.msg; // This msg is sent to handleMessageRespnse
                if (ctx.response) handle_message = true; // Need to be set to handle message response
                else handle_message = false;
                if (!msg.message().empty())
                {
                    std::cout << "Message: " << msg.message() << std::endl;
                }
                drawAll(display, dirs, shortcut_bar, ctx, true);
            }

            if (ctx.exit)
            {
                break;
            }
            SDL_PollEvent(&e);
        }
    }
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return 0;
}
