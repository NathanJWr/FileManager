#include "display.h"
#include "config.h"
Display::Display(int width, int height) :
	/* These need to be in the initializer list since they don't have default constructors */
	window(SDL2::makeWindow("FileManager", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE)),
	renderer(SDL2::makeRenderer(window, -1, SDL_RENDERER_ACCELERATED)),
	font(SDL2::makeFont("assets/Ubuntu.ttf", 32)),
	folder_icon(SDL2::makeIMGTexture("assets/folder.png", renderer)),
	image_icon(SDL2::makeIMGTexture("assets/image.png", renderer)),
	text_icon(SDL2::makeIMGTexture("assets/text.png", renderer)),
	cpp_icon(SDL2::makeIMGTexture("assets/c++src.png", renderer)),
	script_icon(SDL2::makeIMGTexture("assets/script.png", renderer))
{

	SCREEN_W = width;
	SCREEN_H = height;

	int dir_x = static_cast<int>(width / 5.5);
	int dir_y = static_cast<int>(height / 25);
	int dir_w = SCREEN_W;
	int dir_h = static_cast<int>(height / 1.2);

	/* figure out the height and width of a text_box */
	auto temp_buff = SDL2::makeTextSurface(font, "text", white);
	text_box.w = temp_buff.get()->w;
	text_box.h = temp_buff.get()->h;

	int icon_size = text_box.w / 2;

	folder_icon.pos = {dir_x, 0, icon_size, icon_size};
	image_icon.pos = {dir_x, 0, icon_size, icon_size};
	text_icon.pos = {dir_x, 0, icon_size, icon_size};
	cpp_icon.pos = {dir_x, 0, icon_size, icon_size};
	script_icon.pos = {dir_x, 0, icon_size, icon_size};

	dir_box = {dir_x, dir_y, dir_w, dir_h};
	console_box = { dir_x, dir_y + dir_h, SCREEN_W, SCREEN_H};

	shortcut_box = {0, 0, dir_x, height};
	shell_letter_pos = 0;
}

void Display::update()
{
	SDL_SetRenderDrawColor(renderer.get(), 0, 43, 54, 255);
	SDL_RenderPresent(renderer.get());
	SDL_RenderClear(renderer.get());
	SDL_SetRenderDrawColor(renderer.get(), 0, 43, 54, 255);
}

void Display::resize()
{

	SDL_GetWindowSize(window.get(), &SCREEN_W, &SCREEN_H);

	int dir_x = static_cast<int>(SCREEN_W / 5.5);
	int dir_y = static_cast<int>(SCREEN_H / 25);
	int dir_w = SCREEN_W;
	int dir_h = static_cast<int>(SCREEN_H / 1.2);

	/* figure out the height and width of a text_box */
	auto temp_buff = SDL2::makeTextSurface(font, "text", white);
	text_box.w = temp_buff.get()->w;
	text_box.h = temp_buff.get()->h;

	int icon_size = text_box.w / 2;

	folder_icon.pos = {dir_x, 0, icon_size, icon_size};
	image_icon.pos = {dir_x, 0, icon_size, icon_size};
	text_icon.pos = {dir_x, 0, icon_size, icon_size};
	cpp_icon.pos = {dir_x, 0, icon_size, icon_size};
	script_icon.pos = {dir_x, 0, icon_size, icon_size};

	dir_box = {dir_x, dir_y, dir_w, dir_h};
	console_box = { dir_x, dir_y + dir_h, SCREEN_W, SCREEN_H};

	shortcut_box = {0, 0, dir_x, SCREEN_H};
	shell_letter_pos = 0;
}

SDL_Color Display::determineColor(DirObject obj)
{
	SDL_Color color;
	if (obj.selected)
	{
		color = white;
	}
 	else if (obj.type() == DirObject::FILE)
	{
		color = text;
	}
 	else if (obj.type() == DirObject::FOLDER)
	{
		color = folder;
	}
	return color;
}

void Display::renderIcon(DirObject obj, int y)
{
	if (obj.isFolder())
	{
		folder_icon.pos.y = y;
		renderTexture(folder_icon);
	}
 	else if (obj.extension() == ".png" || obj.extension() == ".jpg")
	{
		image_icon.pos.y = y;
		renderTexture(image_icon);
	}
 	else if (obj.extension() == ".cpp" || obj.extension() == ".cc")
	{
		cpp_icon.pos.y = y;
		renderTexture(cpp_icon);
	}
 	else if (obj.extension() == ".exe" || obj.extension() == ".sh")
	{
		script_icon.pos.y = y;
		renderTexture(script_icon);
	}
	else
	{
		text_icon.pos.y = y;
		renderTexture(text_icon);
	}
}

void Display::renderDirectory(Directory& dir)
{
	cur_path = dir.path();
	int buf_x = dir_box.x + folder_icon.pos.w;

	int size = 0;;
	if (dir.max_dir_objs == 0) dir.max_dir_objs = static_cast<unsigned int>(dir_box.h / text_box.h);
	if (dir.get().size() < dir.min_dir_objs + dir.max_dir_objs)
	{
		size = dir.get().size();
	}
 	else
 	{
		size = dir.min_dir_objs + dir.max_dir_objs;
	}

	if (dir.last_move == Directory::NONE)
	{
		/* New directory, so everything needs to be cleared */
		DirTextures.clear();
		int buf_y = dir_box.y;
		for (int i = dir.min_dir_objs; i < size; ++i)
		{
			auto color = determineColor(dir.get()[i]);
			DirTextures.emplace_back(SDL2::makeTextTexture(font, dir.get()[i].name().c_str(), color, renderer));
			DirTextures.back().pos.x = buf_x;
			DirTextures.back().pos.y = buf_y;

			buf_y += text_box.h;
		}
	}
	else if (dir.last_move == Directory::UP)
	{
		/* The selected DirObject isn't at the top
		 * Redraw old selected item AND the new one
		 */
		if (dir.selected_at > 0 && (dir.selected_at - dir.min_dir_objs > 0))
		{
			unsigned int sel;
			sel = dir.selected_at - dir.min_dir_objs;

			auto redraw1 = std::move(DirTextures[sel]);
			auto redraw2 = std::move(DirTextures[sel - 1]);

			SDL_Color color = determineColor(dir.get()[dir.selected_at]);
			DirTextures[sel] = std::move(SDL2::makeTextTexture(font, redraw1.text.c_str(), color, renderer));
			DirTextures[sel].pos = redraw1.pos;

			DirTextures[--sel] = std::move(SDL2::makeTextTexture(font, redraw2.text.c_str(), white, renderer));
			DirTextures[sel].pos = redraw2.pos;

			for (auto& n : DirTextures)
			{
				renderTextTexture(n);
			}
			--dir.selected_at;
		}

		/* The selected DirObject IS at the top
		 * Add one texture to the TOP/FRONT of the list
		 * Remove one texture from the BOTTOM/END of the list
		 */
		else if (dir.selected_at > 0 && dir.selected_at - dir.min_dir_objs <= 0)
	 	{
			unsigned int sel;
			sel = dir.selected_at - dir.min_dir_objs;

			auto redraw = std::move(DirTextures.front());
			SDL_Color color1 = determineColor(dir.get()[dir.selected_at]);
			DirTextures.front() = std::move(SDL2::makeTextTexture(font, redraw.text.c_str(), color1, renderer));
			DirTextures.front().pos = redraw.pos;

			DirTextures.pop_back();
			auto& tba = dir.get()[--dir.selected_at];
			SDL_Color color2 = determineColor(tba);
			DirTextures.emplace_front(SDL2::makeTextTexture(font, tba.name().c_str(), color2, renderer));
			DirTextures.front().pos.x = redraw.pos.x;
			DirTextures.front().pos.y = redraw.pos.y;
			DirTextures.front().pos.y -= text_box.h;

			for (auto& n : DirTextures)
			{
				n.pos.y += text_box.h;
			}
			dir.min_dir_objs--;
		}
	}
	else if (dir.last_move == Directory::DOWN)
	{
		/* The selected DirObject isn't at the bottom
		 * Redraw the old selected item an new one
		 */
		if (dir.selected_at + 1 < dir.get().size() && (dir.selected_at - dir.min_dir_objs) < dir.max_dir_objs - 1 )
	 	{
			unsigned int sel;
			sel = dir.selected_at - dir.min_dir_objs;

			auto redraw1 = std::move(DirTextures[sel]);
			auto redraw2 = std::move(DirTextures[sel + 1]);

			SDL_Color color = determineColor(dir.get()[dir.selected_at]);
			DirTextures[sel] = std::move(SDL2::makeTextTexture(font, redraw1.text.c_str(), color, renderer));
			DirTextures[sel].pos = redraw1.pos;

			DirTextures[++sel] = std::move(SDL2::makeTextTexture(font, redraw2.text.c_str(), white, renderer));
			DirTextures[sel].pos = redraw2.pos;

			dir.selected_at += 1;
		}
		/* The selected DirObject IS at the bottom
		 * Add one DirObject to the bottom/end of the list
		 * Remove one DirObject to the top/front of the list
		 */
		else if (dir.selected_at + 1 < dir.get().size() && (dir.selected_at - dir.min_dir_objs) >= dir.max_dir_objs - 1)
		{
			dir.min_dir_objs++;

			auto redraw = std::move(DirTextures.back());
			SDL_Color color1 = determineColor(dir.get()[dir.selected_at]);
			DirTextures.back() = std::move(SDL2::makeTextTexture(font, redraw.text.c_str(), color1, renderer));
			DirTextures.back().pos = redraw.pos;

			DirTextures.pop_front();
			auto& tba = dir.get()[++dir.selected_at];
			SDL_Color color2 = determineColor(tba);
			DirTextures.emplace_back(SDL2::makeTextTexture(font, tba.name().c_str(), color2, renderer));
			DirTextures.back().pos.x = redraw.pos.x;
			DirTextures.back().pos.y = redraw.pos.y;
			DirTextures.back().pos.y += text_box.h;

			for (auto& n : DirTextures)
			{
				n.pos.y -= text_box.h;
			}
		}
	}

	/* Render the created/changed textures
	 * AND the icons associated with their file type
	 */
	if (dir.get().size() < dir.min_dir_objs + dir.max_dir_objs)
	{
		size = dir.get().size();
	}
 	else
	{
		size = dir.min_dir_objs + dir.max_dir_objs;
	}
	for (int i = dir.min_dir_objs; i < size; ++i)
	{
		renderIcon(dir.get()[i], DirTextures[i - dir.min_dir_objs].pos.y);
		renderTextTexture(DirTextures[i-dir.min_dir_objs]);
	}

	/*
	std::cout << "Selected at: " << dir.selected_at << std::endl;
	std::cout << "Min_dir_objs: " << dir.min_dir_objs << std::endl;
	std::cout << "Max_dir_objs: " << dir.max_dir_objs << std::endl;
	*/
}

void Display::renderUI(ShortcutBar &bar)
{
	SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer.get(), &dir_box);
	SDL_RenderDrawRect(renderer.get(), &console_box);
	renderShortcuts(bar);
	renderCurrentPath();
}

void Display::renderUI(ShortcutBar &bar, std::string console_message, bool new_message)
{
	renderUI(bar);
	renderConsoleMessage(console_message, new_message);
}

void Display::renderConsoleMessage(std::string message, bool new_message)
{
	SDL2::TextTexture text = SDL2::makeTextTexture(font, message.c_str(), white, renderer);

	if (new_message)
	{
		shell_letters.clear();
		shell_letter_pos = 0;
	}
	if (message != "backspace" && message != "return")
	{
		shell_letters.emplace_back(SDL2::makeTextTexture(font, message.c_str(), white, renderer));
		shell_letters.back().pos.x = console_box.x + shell_letter_pos;
		shell_letter_pos += shell_letters.back().pos.w;
		shell_letters.back().pos.y = console_box.y;
	}

	for (SDL2::TextTexture &n : shell_letters)
	{
		renderTextTexture(n);
	}
}

void Display::renderShortcuts(ShortcutBar &bar)
{
	SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer.get(), &shortcut_box);
	int buf = 0;
	auto &list = bar.get_s();
	for (auto& n : list)
 	{
		SDL_Color color;
		if (n.highlighted)
	 	{
			color = yellow;
		}
		else
	 	{
			color = shortcut;
		}
		SDL2::TextTexture text = SDL2::makeTextTexture(font, n.name().c_str(), color, renderer);
		text.pos.y = buf;
		buf += text_box.h;
		n.pos = text.pos;
		renderTextTexture(text);
	}
}

void Display::renderCurrentPath()
{
	int count = 0;
	for (unsigned int i = 0; i < cur_path.size(); i++)
 	{
		if (cur_path.at(i) == SLASH[0])
	 	{
			count++;
		}
	}
	std::string buff;
	if (count > 2) buff = ".." + SLASH;
	else buff = "";
	while (count > 2)
	{
		size_t pos = cur_path.find_first_of(SLASH);
		cur_path.erase(cur_path.begin(), cur_path.begin() + pos + 1);
		count--;
	}
	std::string buf = buff + cur_path;
	auto p = SDL2::makeTextTexture(font, buf.c_str(), white, renderer);
	p.pos.x = dir_box.x;
	p.pos.y = dir_box.y - text_box.h;
	renderTextTexture(p);
}

void Display::renderTextTexture(SDL2::TextTexture& t)
{
	SDL_RenderCopy(renderer.get(), t.tex.get(), NULL, &t.pos);
}

void Display::renderTexture(SDL2::ImgTexture& t)
{
	SDL_RenderCopy(renderer.get(), t.tex.get(), NULL, &t.pos);
}

void Display::popShellLetter()
{
	if (!shell_letters.empty())
	{
		shell_letter_pos -= shell_letters.back().pos.w;
		shell_letters.pop_back();
	}
}

void Display::clearShellLetters()
{
	shell_letters.clear();
	shell_letter_pos = 0;
}
