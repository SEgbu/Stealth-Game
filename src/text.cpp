#include "text.hpp"

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    long int Advance;    // Offset to advance to next glyph
};

std::map<char, Character> Characters;

Text::Text(char* ffp){
    fontFilePath = ffp;
}

bool Text::Init(float scrwidth, float scrheight){
    // set success to true
    bool success = true;

    // if free type doesn't init then print a statement and return unsuccessful
    if (FT_Init_FreeType(&ftLibrary)){
        std::cout << "FreeType Error: Could not init FreeType Library" << std::endl;
        success = false;
    }

    // if face doesn't load then print a statement and return unsuccessful
    if (FT_New_Face(ftLibrary, fontFilePath, 0, &face)){
        std::cout << "FreeType Error: Failed to load font" << std::endl;
        success = false;
    }

    // sets the font's width and height parameters
    // (width is set to 0 so that is can dynamically calculate the width based on the given height)
    FT_Set_Pixel_Sizes(face, 0, 48);

    // loading the glyphs
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
  
    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (!FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "FreeType Error: Failed to load Glyph" << std::endl;
            continue;
        }

        else {
            // creating texture
            Texture2D texture(face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_RED, GL_RED, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, 
                              GL_LINEAR, GL_LINEAR);

            // store character for later use
            Character character = {
                texture.ID, 
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };
            Characters.insert(std::pair<char, Character>(c, character));

            texture.bind();
        }

    }
    
    // initialize render data
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW); // the data is null until we update it in the render function
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);   
    

    // create shader
    shader.compile("shaders/text.vs", "shaders/text.fs");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(scrwidth), 0.0f, static_cast<float>(scrheight));
    shader.use();
    shader.setMatrix4("projection", projection, true);

    return success; 
}

void Text::Render(std::string text, float x, float y, float scale, glm::vec3 colour){
    
    // activate corresponding render state	
    shader.use();
    shader.setVector3f("textColor", colour, true);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++){
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        // update VBO for each character
        float vertices[6][4] = {
            {xpos,     ypos + h,   0.0f, 0.0f},            
            {xpos,     ypos,       0.0f, 1.0f},
            {xpos + w, ypos,       1.0f, 1.0f},

            {xpos,     ypos + h,   0.0f, 0.0f},
            {xpos + w, ypos,       1.0f, 1.0f},
            {xpos + w, ypos + h,   1.0f, 0.0f}           
        };
        
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0); 

        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Text::~Text(){
    // deinitialize face and the freetype library
    FT_Done_Face(face);
    FT_Done_FreeType(ftLibrary);
}