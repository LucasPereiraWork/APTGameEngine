#include "SurfaceUtils.h"
#include <iostream>
#include <memory>

std::unique_ptr<SDL_Surface, void (*)(SDL_Surface*)> OptimizedSurface(const std::string& filePath, SDL_Surface* windowSurface) {
    SDL_Surface* loadedSurface = SDL_LoadBMP(filePath.c_str());
    if (!loadedSurface) {
        std::cout << "Error loading surface: " << SDL_GetError() << std::endl;
        return std::unique_ptr<SDL_Surface, void (*)(SDL_Surface*)>(nullptr, SDL_FreeSurface);
    }

    std::unique_ptr<SDL_Surface, void (*)(SDL_Surface*)> optimizedSurface(
        SDL_ConvertSurface(loadedSurface, windowSurface->format, 0), SDL_FreeSurface);

    SDL_FreeSurface(loadedSurface);

    if (!optimizedSurface) {
        std::cout << "Error optimizing surface: " << SDL_GetError() << std::endl;
        return std::unique_ptr<SDL_Surface, void (*)(SDL_Surface*)>(nullptr, SDL_FreeSurface);
    }

    return optimizedSurface;
}
