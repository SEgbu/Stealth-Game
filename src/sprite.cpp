#include <sprite.hpp>

SpriteRenderer::SpriteRenderer(Shader& shader){
    this->shader = shader; // set its own shader to shader passed in
    this->initRenderData(); // declare the render data
}

// deallocate vao
SpriteRenderer::~SpriteRenderer(){
    glDeleteVertexArrays(1, &this->VAO);
}

void SpriteRenderer::initRenderData(){
    // create vertices
    float vertices[]{
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        
        0.0f, 1.0f, 0.0f, 1.0f, 
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    // define vao
    glGenVertexArrays(1, &this->VAO);
    
    // create vbo
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // bind vao
    glBindVertexArray(this->VAO);

    // create vertex attribute pointers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    // unbind vao and vbo
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void SpriteRenderer::drawSprite(Texture2D& texture, glm::vec2 position, int zIndex, glm::vec2 size, float rotate, glm::vec3 colour){
    this->shader.use(); // set shader active

    // set model matrix to an identity matrix
    glm::mat4 model = glm::mat4(1.0f);

    // the model matrix is being multiplied by other matrices for transformation 
    // the order in matrix multiplicaiton operations (this is all in reverse): translate, rotate then scale

    model = glm::translate(model, glm::vec3(position, float(zIndex))); // set sprite to position

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to centre of sprite
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // rotate 
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move back

    model = glm::scale(model, glm::vec3(size, 1.0f)); // scale sprite

    // set some uniforms 
    this->shader.setMatrix4("model", model);
    this->shader.setVector3f("spriteColour", colour);

    // bind texture and set texture unit active
    glActiveTexture(GL_TEXTURE0);
    texture.bind();
    
    // draw triangles (the sprite) on the screen
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}