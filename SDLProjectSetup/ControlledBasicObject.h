#pragma once

#include "BasicObject.h"
#include "Definitions.h"


namespace AGPT {

    class AGPT_API ControlledBasicObject : public BasicObject {
    public:
        ControlledBasicObject(float x, float y, float width, float height, float velocity, bool isNormalized, const std::string& imagePath);
        virtual ~ControlledBasicObject() override;

        virtual void HandleEvent(const SDL_Event& e); 
        void Update(float deltaTime) override;
        virtual void OnCollision(BasicObject* other) override;
        virtual b2Body* GetBody() const override;

    protected:
        float bodyVelocity;
        float normalVelocityX = 0.0f;
        float normalVelocityY = 0.0f;
        bool isNormalized;
        b2Body* body;
    };

}
