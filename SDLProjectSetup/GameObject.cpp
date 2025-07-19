#include "GameObject.h"

using namespace AGPT;

GameObject::GameObject(float x, float y, float width, float height)
    : x(x), y(y), width(width), height(height), velocityX(0), velocityY(0), active(true) {}

GameObject::~GameObject() {}

void GameObject::Update(float deltaTime) {
    x += velocityX * deltaTime;
    y += velocityY * deltaTime;
}


float GameObject::GetX() const {
    return x;
}

float GameObject::GetY() const {
    return y;
}

void GameObject::SetVelocity(float vx, float vy) {
    velocityX = vx;
    velocityY = vy;
}

bool GameObject::IsActive() const {
    return active;
}

void GameObject::SetActive(bool state) {
    active = state;
}

void GameObject::SetPosition(float newX, float newY) {
    x = newX;
    y = newY;
}
