#pragma once


#ifndef SURFACE_UTILS_H
#define SURFACE_UTILS_H

#include <SDL.h>
#include <string> 
#include <memory>
#include "Definitions.h"


/// - FUNCTION DECLARATIONS - ///



std::unique_ptr<SDL_Surface, void (*)(SDL_Surface*)> OptimizedSurface(const std::string& filePath, SDL_Surface* windowSurface);

#endif
