#include "display.h"
#include "filesystem.h"
#include "shortcutbar.h"
#include "message.h"
#include "platformlayer.h"
#include "input.h"
#include "context.h"

#include <iostream>
#include <memory>
#include <vector>
#include <list>

int MAIN()
{
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr <<
            "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        success = false;
    }
    if (TTF_Init() == -1)
    {
        std::cerr <<
            "Failed to initialize TTF: " << TTF_GetError() << std::endl;
        success = false;
    }
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        std::cerr <<
            "Failed to initialize IMG: " << IMG_GetError() << std::endl;
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
                ctx = handleMessageResponse(e, display,
                                            dirs, shortcut_bar, msg);
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
                display.drawAll(dirs, shortcut_bar, ctx, true);
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
