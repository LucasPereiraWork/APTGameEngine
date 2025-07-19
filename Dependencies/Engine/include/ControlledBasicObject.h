#pragma once

#include "BasicObject.h"
#include "Definitions.h"


namespace AGPT {

    class AGPT_API ControlledBasicObject : public BasicObject {
    public:
        ControlledBasicObject(float x, float y, float width, float height, float velocity, bool isNormalized, const std::string& imagePath);
        virtual ~ControlledBasicObject() override;

        void HandleEvent(const SDL_Event& e);  // New method to handle events
        void Update(float deltaTime) override; // Override to implement controlled movement

    private:
        float bodyVelocity;
        float normalVelocityX = 0.0f;
        float normalVelocityY = 0.0f;
        bool isNormalized;
    };

}
