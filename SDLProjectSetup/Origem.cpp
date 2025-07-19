#include "Origem.h"

namespace AGPT {

    struct Origem::Impl {
        std::vector<std::unique_ptr<GameObject>> gameObjects;
        std::vector<std::unique_ptr<GameObject>> newGameObjects;
        b2World* world = nullptr;
    };

    void Origem::QueueForRemoval(BasicObject* obj) {
        if (obj && obj->IsActive()) {
            m_removalQueue.push_back(obj);
        }
    }

    void Origem::ProcessRemovals() {
        for (auto* obj : m_objectsToRemoveNextFrame) {
            auto it = std::find_if(pImpl->gameObjects.begin(), pImpl->gameObjects.end(),
                [obj](const std::unique_ptr<GameObject>& ptr) { return ptr.get() == obj; });
            if (it != pImpl->gameObjects.end()) {
                pImpl->gameObjects.erase(it);
            }
        }
        m_objectsToRemoveNextFrame.clear();

        for (auto* obj : m_removalQueue) {
            if (obj && obj->IsActive()) {
                obj->SetActive(false);
                if (obj->GetBody()) {
                    obj->GetBody()->GetUserData().pointer = 0;
                    pImpl->world->DestroyBody(obj->GetBody());
                }
                m_objectsToRemoveNextFrame.push_back(obj);
            }
        }
        m_removalQueue.clear();
    }


    b2World* Origem::GetWorld() const {
        if (pImpl && pImpl->world) {
            return pImpl->world;
        }
        return nullptr;
    }

    void Origem::AddGameObject(std::unique_ptr<GameObject> obj) {
        if (pImpl) {
            pImpl->newGameObjects.push_back(std::move(obj));
        }
    }

    void Origem::CleanupObjects() {
        auto& objs = pImpl->gameObjects;
        objs.erase(
            std::remove_if(objs.begin(), objs.end(),
                [](const std::unique_ptr<GameObject>& obj) {
                    return !obj->IsActive();
                }),
            objs.end());
    }


    void Origem::IntegrateNewGameObjects() {
        if (pImpl) {
            for (auto& newObj : pImpl->newGameObjects) {
                pImpl->gameObjects.push_back(std::move(newObj));
            }
            pImpl->newGameObjects.clear();
        }
    }

    Origem::Origem() : pImpl(std::make_unique<Impl>()) {

        b2Vec2 gravity(0.0f, 0.0f);
        pImpl->world = new b2World(gravity);
    }

    Origem::~Origem() {

        if (pImpl->world) {
            pImpl->world->SetContactListener(nullptr);
            delete pImpl->world;
        }
    }

    bool Origem::Init(UniqueSDLWindow& window, UniqueSDLSurface& screenSurface) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
            return false;
        }

        SDL_Window* rawWindow = SDL_CreateWindow("OpenGL Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            AGPT::SCREEN_WIDTH, AGPT::SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
        if (!rawWindow) {
            std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return false;
        }
        window.reset(rawWindow);

        // Create an OpenGL context
        glContext = SDL_GL_CreateContext(rawWindow);
        if (!glContext) {
            std::cout << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(rawWindow);
            SDL_Quit();
            return false;
        }

        // Initialize GLAD
        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
            std::cout << "Failed to initialize GLAD!" << std::endl;
            SDL_GL_DeleteContext(glContext);
            SDL_DestroyWindow(rawWindow);
            SDL_Quit();
            return false;
        }

        glViewport(0, 0, AGPT::SCREEN_WIDTH, AGPT::SCREEN_HEIGHT);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);


        return true;
    }


    SDL_Rect Origem::DefineDrawingRect() {
        SDL_Rect drawingRect = { 0, 0, 800, 600 };
        return drawingRect;
    }

    void Origem::Quit() {
        SDL_GL_DeleteContext(glContext);
        SDL_Quit();
    }

    void Origem::HandleEvents(SDL_Event* inputEvent, bool& isRunning) {


        if (!pImpl) {
            std::cerr << "Error: pImpl is null." << std::endl;
            isRunning = false;
            return;
        }

        if (pImpl->gameObjects.empty()) {
            std::cerr << "Error: gameObjects is empty." << std::endl;
            isRunning = false;
            return;
        }

        if (inputEvent == nullptr) {
            std::cerr << "Error: inputEvent is null." << std::endl;
            isRunning = false;
            return;
        }

        while (SDL_PollEvent(inputEvent) != 0) {

            if (inputEvent->type == SDL_QUIT) {
                isRunning = false;
            }
            else {
                for (auto& obj : pImpl->gameObjects) {
                    if (!obj) {
                        std::cerr << "Error: Found null object in gameObjects." << std::endl;
                        continue;
                    }

                    try {
                        if (auto controlledObj = dynamic_cast<ControlledBasicObject*>(obj.get())) {
                            controlledObj->HandleEvent(*inputEvent);
                        }
                    }
                    catch (const std::bad_cast& e) {
                        std::cerr << "Error: Bad cast exception: " << e.what() << std::endl;
                    }
                }
            }
        }
    }

    void Origem::UpdateObjects(Uint64& currentTicks, Uint64& lastTicks, float& deltaTime) {
        try {
            currentTicks = SDL_GetTicks();
            deltaTime = (currentTicks - lastTicks) / 1000.0f;
            lastTicks = currentTicks;

            try {
                pImpl->world->Step(deltaTime, 6, 2);
            }
            catch (const std::exception& e) {
                std::cerr << "Error in physics step: " << e.what() << std::endl;
            }

            for (size_t i = 0; i < pImpl->gameObjects.size(); ++i) {
                auto& obj = pImpl->gameObjects[i];
                if (obj->IsActive()) {
                    try {
                        obj->Update(deltaTime);
                    }
                    catch (const std::exception& e) {
                        std::cerr << "Error updating object " << i << ": " << e.what() << std::endl;
                    }
                }
            }

            try {
                IntegrateNewGameObjects();
            }
            catch (const std::exception& e) {
                std::cerr << "Error integrating new game objects: " << e.what() << std::endl;
            }

            try {
                ProcessRemovals();
            }
            catch (const std::exception& e) {
                std::cerr << "Error processing removals: " << e.what() << std::endl;
            }

            try {
                CleanupObjects();
            }
            catch (const std::exception& e) {
                std::cerr << "Error cleaning up objects: " << e.what() << std::endl;
            }

        }
        catch (const std::exception& e) {
            std::cerr << "Unhandled exception in UpdateObjects: " << e.what() << std::endl;
        }
        catch (...) {
            std::cerr << "Unknown error occurred in UpdateObjects" << std::endl;
        }
    }

    void Origem::UpdateRender(SDL_Window* window) {
        if (!window) {
            std::cerr << "Error: Null pointer provided to UpdateRender function." << std::endl;
            return;
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(AGPT::SCREEN_WIDTH),
            static_cast<float>(AGPT::SCREEN_HEIGHT), 0.0f, -1.0f, 1.0f);

        for (const auto& gameObject : pImpl->gameObjects) {
            if (gameObject->IsActive()) {
                gameObject->Render();
            }
        }

        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cerr << "OpenGL error in UpdateRender: " << error << std::endl;
        }

        SDL_GL_SwapWindow(window);
    }
}