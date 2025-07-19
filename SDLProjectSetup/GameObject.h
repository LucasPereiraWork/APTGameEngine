#pragma once

#include <SDL.h>
#include <iostream>
#include <string>
#include "Definitions.h"


namespace AGPT {

    class AGPT_API GameObject {
    public:
        GameObject(float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f);
        virtual ~GameObject();

        virtual void Update(float deltaTime);
        virtual void Render() const = 0;
        virtual void SetPosition(float newX, float newY);
        virtual const std::string& GetTag() const { return tag; };
        virtual void SetTag(const std::string& newTag) { tag = newTag; }

        float GetX() const;
        float GetY() const;

        void SetVelocity(float vx, float vy);
        bool IsActive() const;
        void SetActive(bool state);

    protected:
        float x, y;          // Position
        float width, height; // Dimensions
        float velocityX, velocityY; // Velocity
        bool active;         // Active state of the GameObject
        std::string tag;
    };

}

