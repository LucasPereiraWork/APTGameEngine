#pragma once

#ifndef MAIN_H 
#define MAIN_H

#include <functional>
#include <vector>
#include "GameObject.h"
#include "BasicObject.h"
#include "ControlledBasicObject.h"
#include "Config.h"
#include "SurfaceUtils.h"
#include "Definitions.h"
#include "SDL_DEleters.h"

/// - FUNCTION DECLARATIONS - ///
namespace AGPT {

		//int main();

		// Declare the Init function with smart pointers

	class AGPT_API Origem
	{
	public:
		 Origem();
		~Origem();

		bool Init(UniqueSDLWindow& window, UniqueSDLSurface& screenSurface);

		void Quit();

		SDL_Rect DefineDrawingRect();

		void HandleEvents(SDL_Event* inputEvent, bool& isRunning, std::vector<std::unique_ptr<AGPT::GameObject>>& gameObjects);

		void UpdateObjects(Uint64& currentTicks, Uint64& lastTicks, float& deltaTime, std::vector<std::unique_ptr<AGPT::GameObject>>& gameObjects);

		void UpdateRender(SDL_Surface* screenSurface, SDL_Window* window, SDL_Rect& drawingRect, std::vector<std::unique_ptr<AGPT::GameObject>>& gameObjects);

		//void Quit(SDL_Surface** currentImage, SDL_Surface** screenSurface, SDL_Window** window);

	private:

	};
	}

#endif
