#include "ControlledBasicObject.h"

using namespace AGPT;

ControlledBasicObject::ControlledBasicObject(float x, float y, float width, float height, float bodyVelocity, bool isNormalized, const std::string& imagePath)
    : BasicObject(x, y, width, height, imagePath), bodyVelocity(bodyVelocity), isNormalized(isNormalized) {}

ControlledBasicObject::~ControlledBasicObject() {}

void ControlledBasicObject::HandleEvent(const SDL_Event& e) {
}

void ControlledBasicObject::OnCollision(BasicObject* other) {
  
}

b2Body* ControlledBasicObject::GetBody() const {
    return body; 
}

void ControlledBasicObject::Update(float deltaTime) {

    if (isNormalized)
    {
        float magnitude = sqrt(velocityX * velocityX + velocityY * velocityY);
        if (magnitude > 0) {
            normalVelocityX = (velocityX / magnitude) * bodyVelocity;
            normalVelocityY = (velocityY / magnitude) * bodyVelocity;
        }
        else
        {
            normalVelocityX = velocityX;
            normalVelocityY = velocityY;
        }
    }
    else
    {
        normalVelocityX = velocityX * bodyVelocity;
        normalVelocityY = velocityY * bodyVelocity;
    }

    x += normalVelocityX * deltaTime;
    y += normalVelocityY * deltaTime;
    std::cout << "Rendering BasicObject at (" << velocityX << ", " << velocityY << ") without an image." << std::endl;
    BasicObject::Update(deltaTime); 
}