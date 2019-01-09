#ifdef _WIN32
#include <SDL.h>
#include <SDL_ttf.h>
#endif

#ifdef __unix__
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#endif

#ifndef SDLWRAP_H_
#define SDLWRAP_H_
#include <utility>
#include <system_error>
#include <memory>

#include <string>
#include <iostream>

namespace SDL2 {

	inline SDL_Texture* surfaceToTexture(SDL_Surface* surf, SDL_Renderer* renderer) {
		SDL_Texture* text = nullptr;
		text = SDL_CreateTextureFromSurface(renderer, surf);
		//SDL_FreeSurface(surf);
		if (!text) {
			throw std::system_error(errno, std::generic_category());
		}
		return text;
	}

	template<typename Creator, typename Destructor, typename... Arguments>
	inline auto make_resource(Creator c, Destructor d, Arguments&&... args) {
		auto r = c(std::forward<Arguments>(args)...);
		if (!r) { throw std::system_error(errno, std::generic_category()); }
		return std::unique_ptr<std::decay_t<decltype(*r)>, decltype(d)>(r, d);
	}

	using window_ptr = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>;
	using renderer_ptr = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>;
	using font_ptr = std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)>;
	using text_surface = std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>;
	using text_texture = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>;
	struct Screen {
		window_ptr window;
		renderer_ptr renderer;
	};
	struct TextTexture {
		text_texture texture;
		SDL_Rect pos;
		std::string text;
	};
	inline renderer_ptr makeRenderer(window_ptr& w, int index, Uint32 flags) {
		return make_resource(SDL_CreateRenderer, SDL_DestroyRenderer, w.get(), index, flags);
	}
	inline window_ptr makeWindow(const char* title, int x, int y, int w, int h, Uint32 flags) {
		return  make_resource(SDL_CreateWindow, SDL_DestroyWindow, title, x, y, w, h, flags);
	}
	inline Screen makeScreen(const char* title, int x, int y, int w, int h, Uint32 flags1, int index, Uint32 flags2) {
		window_ptr win = make_resource(SDL_CreateWindow, SDL_DestroyWindow, title, x, y, w, h, flags1);
		renderer_ptr r = makeRenderer(win, index, flags2);
		return Screen{ std::move(win), std::move(r) };
	}
	inline font_ptr makeFont(const char* path, const int size) {
		return make_resource(TTF_OpenFont, TTF_CloseFont, path, size);
	}
	inline text_surface makeTextSurface(font_ptr& f, const char* t, SDL_Color c) {
		return make_resource(TTF_RenderText_Solid, SDL_FreeSurface, f.get(), t, c);
	}

	inline TextTexture makeTextTexture(font_ptr& f, const char* t, SDL_Color c, renderer_ptr& r) {
		auto s = make_resource(TTF_RenderText_Solid, SDL_FreeSurface, f.get(), t, c);
		return { make_resource(surfaceToTexture, SDL_DestroyTexture, s.get(), r.get()), {0, 0, s->w, s->h}, t };
	}
	inline TextTexture makeTextTexture(std::string text, text_surface& s, renderer_ptr& r) {
		SDL_Rect pos = { 0, 0, s->w, s->h };
		return { make_resource(surfaceToTexture, SDL_DestroyTexture, s.get(), r.get()), pos, text };
	}
}
#endif // SDLWRAP_H_
