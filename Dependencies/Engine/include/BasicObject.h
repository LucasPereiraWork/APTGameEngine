#pragma once

#include <string>
#include <memory>
#include "GameObject.h"

namespace AGPT {

    class AGPT_API BasicObject : public GameObject {
    public:
        BasicObject(float x, float y, float width, float height, const std::string& imagePath);
        virtual ~BasicObject() override;

        void Update(float deltaTime) override;
        void Render(SDL_Surface* targetSurface) const override;
        void OnCollision(GameObject* other) override;

    private:
        std::unique_ptr<SDL_Surface, void (*)(SDL_Surface*)> image;  // Smart pointer for SDL_Surface
    };

}
