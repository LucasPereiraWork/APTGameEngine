#pragma once

#ifndef MAIN_H 
#define MAIN_H

#include <functional>
#include <memory>
#include <vector>
#include <iostream>
#include <stdexcept>
#include "GameObject.h"
#include "BasicObject.h"
#include "ControlledBasicObject.h"
#include "Config.h"
#include "SurfaceUtils.h"
#include "Definitions.h"
#include "SDL_DEleters.h"
#include "box2d\box2d.h"

namespace AGPT {

    class OrigemImpl; // Forward declaration of the implementation class

    class AGPT_API Origem {
    public:
        Origem();
        ~Origem();

        bool Init(UniqueSDLWindow& window, UniqueSDLSurface& screenSurface);
        void Quit();
        SDL_Rect DefineDrawingRect();
        void HandleEvents(SDL_Event* inputEvent, bool& isRunning);
        void UpdateObjects(Uint64& currentTicks, Uint64& lastTicks, float& deltaTime);
        void UpdateRender(SDL_Window* window);
        void AddGameObject(std::unique_ptr<GameObject> gameObject);
        void IntegrateNewGameObjects();
        void CleanupObjects();
        void QueueForRemoval(BasicObject* obj);
        void ProcessRemovals();

        b2World* GetWorld() const;
       
        struct Impl;
        std::unique_ptr<Impl> pImpl;  // Pointer to the implementation class
        SDL_GLContext glContext;
        std::vector<BasicObject*> m_removalQueue;
        std::vector<BasicObject*> m_objectsToRemoveNextFrame;



    };

}

#endif
