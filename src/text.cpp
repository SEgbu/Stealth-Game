#include <text.hpp>
#include <string>


Text::Text(glm::vec2 pos, glm::vec2 fSize): position(pos), fontSize(fSize){ 
    std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int alphabetCounter = 0;

    for (int i = 1; i <= 3; i++){
        for (int j = 1; j <= 9; j++){
            if (j == 9 && i == 3){
                break;
            }
            ResourceManager::loadTexture(("assets/copper black font/image"+std::to_string(j)+"x"+std::to_string(i)+".png").c_str(), true, std::to_string(alphabet[alphabetCounter]));
            alphabetCounter++;
        }
    }
}

void Text::draw(SpriteRenderer& renderer, std::string message){
    for (int i = 0; i < (message.length()); i++){  
        if (message[i] != (' ')){
            glm::vec2 currentCharPosition = position + glm::vec2(i*fontSize.x, 0); 
            Texture2D currentMessageCharTexture = ResourceManager::getTexture(std::to_string(message[i]));
            renderer.drawSprite(currentMessageCharTexture, currentCharPosition, 0, fontSize);
        }
        else {
            continue;
        }       
    }
}

