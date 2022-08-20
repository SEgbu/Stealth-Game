#pragma once 

#include <texture2D.hpp>
#include <shader.hpp>

#include <glad/glad.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>


class SpriteRenderer{
    public:
        SpriteRenderer(Shader& shader);
        ~SpriteRenderer();

        void drawSprite(Texture2D& texture, glm::vec2 position,
                        glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
                        glm::vec3 colour = glm::vec3(1.0f));
    private:
        Shader shader;
        unsigned int VAO;

        void initRenderData(); // configures the VAO
};