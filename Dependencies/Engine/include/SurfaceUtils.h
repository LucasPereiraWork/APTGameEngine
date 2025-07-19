#pragma once


#ifndef SURFACE_UTILS_H
#define SURFACE_UTILS_H

#include <SDL.h>
#include <string> 
#include "Definitions.h"


/// - FUNCTION DECLARATIONS - ///



SDL_Surface* OptimizedSurface(std::string filePath, SDL_Surface* windowSurface);

#endif
