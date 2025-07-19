#pragma once

#include <string>
#include <memory>
#include <vector>
#include "TextureLoader.h"
#include "ShaderUtilities.h"
#include "GameObject.h"
#include "box2d/box2d.h"


namespace AGPT {

    class AGPT_API BasicObject : public GameObject {
    public:
        BasicObject(float x, float y, float width, float height, const std::string& imagePath);
        virtual ~BasicObject() override;

        void Update(float deltaTime) override;
        void Render() const override;
        virtual void SetupFrames(int frameWidth, int frameHeight, int numFrames, int numRows);
        virtual SDL_Rect GetCurrentFrame() const;
		virtual void InitRenderData();
        virtual b2Body* GetBody() const = 0;
        virtual void OnCollision(BasicObject* other) = 0;
        

    protected:
        GLuint textureID;  // OpenGL texture ID
        int currentFrameIndex = 0;
        std::vector<SDL_Rect> frames;
        int texWidth, texHeight;
        GLuint vaoID;    // ID for Vertex Array Object
		GLuint vboID[2]; // IDs for Vertex Buffer Objects
        GLuint shaderProgram;
		int textureWidth, textureHeight;
		float scale = 1.0f;
    };

}
