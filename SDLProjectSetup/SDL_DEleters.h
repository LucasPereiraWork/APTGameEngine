#pragma once

#include <SDL.h>
#include <functional>
#include <memory>
#include "Definitions.h"


struct SDLDeleter {
    void operator()(SDL_Window* ptr) const { if (ptr) SDL_DestroyWindow(ptr); }
    void operator()(SDL_Surface* ptr) const { if (ptr) SDL_FreeSurface(ptr); }
};

using UniqueSDLWindow = std::unique_ptr<SDL_Window, SDLDeleter>;
using UniqueSDLSurface = std::unique_ptr<SDL_Surface, SDLDeleter>;
