#ifndef DISPLAY_H_
#define DISPLAY_H_
#include "sdl2wrapper.h"
#include "directory.h"
#include "shortcutbar.h"
#include "shortcut.h"
#include <string>
#include <deque>
class Display
{
public:
    Display(int width, int height, int font_size);
    //~Display();
    /* put the Directory contents on screen */
    void renderDirectory(Directory&);
    /* put all of the UI elements on the screen */
    void renderUI(ShortcutBar &bar);
    /* message will be displayed in the console box */
    void renderUI(ShortcutBar &bar, std::string message, bool new_message);
    /* display the screen */
    void update();

    void resize();

    void increaseFont();
    void decreaseFont();

    void popShellLetter();
    void clearShellLetters();

private:
    void renderConsoleMessage(std::string message, bool new_message);
    void renderTextTexture(SDL2::TextTexture& tex);
    void renderTexture(SDL2::ImgTexture& tex);
    void renderIcon(DirObject obj, int y);
    void renderCurrentPath();
    void renderShortcuts(ShortcutBar &bar);

    void createNewDirectoryTextures(Directory& dir, int size);
    void repositionTexturesOnResize(Directory& dir, int size);

    void replaceTexturesUp(Directory& dir, int size);
    void popBackAndMakeNewFront(Directory& dir, int size);
    void remakeOldSelectedAndNewUp(Directory& dir, int size); 

    void replaceTexturesDown(Directory& dir, int size);
    void popFrontAndMakeNewBack(Directory& dir, int size);
    void remakeOldSelectedAndNewDown(Directory& dir, int size); 

    SDL_Color determineColor(DirObject obj);

    SDL2::window_ptr window;
    SDL2::renderer_ptr renderer;
    SDL2::font_ptr font;

    SDL_Color white{ 255, 255, 255, 255 };
    SDL_Color yellow{ 181, 137, 0, 255 };
    SDL_Color green = { 0, 255, 0, 255 };
    SDL_Color text = { 133, 153, 0, 255 };
    SDL_Color shortcut = { 42, 161, 152, 255 };
    SDL_Color folder = { 38, 139, 210, 255 };

    std::deque<SDL2::TextTexture> DirTextures;
    std::vector<SDL2::TextTexture> shortcuts;
    std::vector<SDL2::TextTexture> shell_letters;
    int shell_letter_pos;

    int SCREEN_W;
    int SCREEN_H;
    int FontSize;
    bool resized;

    SDL_Rect dir_box;
    SDL_Rect shortcut_box;
    SDL_Rect text_box;
    SDL_Rect console_box;

    SDL2::ImgTexture folder_icon;
    SDL2::ImgTexture image_icon;
    SDL2::ImgTexture text_icon;
    SDL2::ImgTexture script_icon;
    SDL2::ImgTexture cpp_icon;

    std::string cur_path;
};
#endif // DISPLAY_H_


