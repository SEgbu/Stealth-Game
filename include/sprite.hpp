#pragma once

// local libraries
#include <texture2D.hpp>
#include <shader.hpp>

// glad library
#include <glad/glad.h>

// opengl maths libraries
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

class SpriteRenderer {
    public:
        // constructor and destructor
        SpriteRenderer(Shader& shader);
        ~SpriteRenderer();
        
        // rendering sprite
        void drawSprite(Texture2D& texture, glm::vec2 position, int zIndex,
                        glm::vec2 size = glm::vec2(15.0f, 15.0f), 
                        float rotate = 0.0f, glm::vec3 colour = glm::vec3(1.0f));

    private:
        // personal shader and vertex array object
        Shader shader;
        unsigned int VAO;

        void initRenderData(); // configures the VAO
};