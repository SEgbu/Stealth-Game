#include <spriterenderer.hpp>

SpriteRenderer::SpriteRenderer(Shader& shader){
    this->shader = shader;
    this->initRenderData();
}

SpriteRenderer::~SpriteRenderer(){
    glDeleteVertexArrays(1, &this->VAO);
}

void SpriteRenderer::initRenderData(){
    // create vertices
    float vertices[] = {
        // pos      // tex
        0.0f, 0.1f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
    };
    
    // create vbo
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // define vao
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindVertexArray(0);
}

void SpriteRenderer::drawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 colour){
    this->shader.use(); // set shader active
    // order of matrix multiplication: scale, rotate and translate (in code, do it in reverse order)
    glm::mat4 model = glm::mat4(1.0f); // a matrix used to transform the object
    
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to the centre of the quad
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

    model = glm::scale(model, glm::vec3(size, 1.0f));

    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, false, glm::value_ptr(model));
    glUniform3fv(glGetUniformLocation(shader.ID, "spriteColour"), 1, glm::value_ptr(colour));



    glBindVertexArray(VAO);

    glActiveTexture(GL_TEXTURE0);
    texture.bind();
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
