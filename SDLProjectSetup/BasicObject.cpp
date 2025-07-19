#include "BasicObject.h"
#include "SurfaceUtils.h"
#include <iostream>

using namespace AGPT;

BasicObject::BasicObject(float x, float y, float width, float height, const std::string& imagePath)
    : GameObject(x, y, width, height), textureID(TextureLoader::LoadTexture(imagePath.c_str())) {

    shaderProgram = ShaderUtilities::loadShader("vertex_shader.vert", "fragment_shader.frag");
    InitRenderData();
}

BasicObject::~BasicObject() {
    glDeleteVertexArrays(1, &vaoID);
    glDeleteBuffers(2, vboID);
    glDeleteProgram(shaderProgram);
}

void BasicObject::Update(float deltaTime) {
    GameObject::Update(deltaTime);
}

void BasicObject::InitRenderData() {
    GLfloat vertices[] = {
        // Positions         // Texture Coords
        -0.5f, 0.5f, 0.0f,  0.0f, 1.0f,   // Top-left
        0.5f, 0.5f, 0.0f,   1.0f, 1.0f,   // Top-right
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f,   // Bottom-right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f    // Bottom-left
    };

    glGenVertexArrays(1, &vaoID);
    glGenBuffers(2, vboID);

    glBindVertexArray(vaoID);

    // Vertex positions buffer
    glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);

    // Texture coordinates buffer
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "OpenGL error: " << error << std::endl;
    }
}

void BasicObject::Render() const {
    std::cout << "Rendering BasicObject at (" << x << ", " << y << ")" << std::endl;

    glUseProgram(shaderProgram);

    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    GLint texOffsetLoc = glGetUniformLocation(shaderProgram, "texOffset");
    GLint texScaleLoc = glGetUniformLocation(shaderProgram, "texScale");
    GLint textureLoc = glGetUniformLocation(shaderProgram, "texture1");

    std::cout << "Uniform locations: model=" << modelLoc << ", projection=" << projectionLoc
        << ", texOffset=" << texOffsetLoc << ", texScale=" << texScaleLoc
        << ", texture1=" << textureLoc << std::endl;

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));
    model = glm::scale(model, glm::vec3(width * scale, height * scale, 1.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(640),
        static_cast<float>(480), 0.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    SDL_Rect frame = GetCurrentFrame();
    float texOffsetX = frame.x / static_cast<float>(textureWidth);
    float texOffsetY = frame.y / static_cast<float>(textureHeight);
    float texScaleX = frame.w / static_cast<float>(textureWidth);
    float texScaleY = frame.h / static_cast<float>(textureHeight);

    glUniform2f(texOffsetLoc, texOffsetX, texOffsetY);
    glUniform2f(texScaleLoc, texScaleX, texScaleY);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(textureLoc, 0);

    glBindVertexArray(vaoID);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error in BasicObject::Render: " << error << std::endl;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}


SDL_Rect BasicObject::GetCurrentFrame() const {
    if (frames.empty()) {
        return SDL_Rect{ 0, 0, static_cast<int>(width), static_cast<int>(height) };
    }
    else {
        return frames[currentFrameIndex];
    }
}


void BasicObject::SetupFrames(int frameWidth, int frameHeight, int numFrames, int numRows) {
    frames.clear();
    int framesPerRow = numFrames / numRows;

    if (numFrames % numRows != 0) framesPerRow++;

    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < framesPerRow; ++col) {
            if (frames.size() >= numFrames) break;

            SDL_Rect frame;
            frame.x = col * frameWidth;
            frame.y = row * frameHeight;
            frame.w = frameWidth;
            frame.h = frameHeight;

            frames.push_back(frame);
        }
    }
}
