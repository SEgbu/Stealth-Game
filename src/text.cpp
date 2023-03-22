#include <text.hpp>
#include <string>


Text::Text(glm::vec2 pos, glm::vec2 fSize): position(pos), fontSize(fSize){ 
    // loading letter images
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

    // loading numbers
    std::string numbers0to8 = "012345678";

    for (int i = 1; i < 10; i++){
        std::string temp(1, numbers0to8[i]);
        ResourceManager::loadTexture(("assets/copper black font/image"+std::to_string(i)+"x7.png").c_str(), true, temp);
    }

    ResourceManager::loadTexture("assets/copper black font/image1x8.png", true, "9");

    // punctuation: :,/#'!?.()
    ResourceManager::loadTexture("assets/copper black font/image5x8.png", true, ":");
    ResourceManager::loadTexture("assets/copper black font/image3x8.png", true, ",");
    ResourceManager::loadTexture("assets/copper black font/image2x9.png", true, "/");
    ResourceManager::loadTexture("assets/copper black font/image7x8.png", true, "#");
    ResourceManager::loadTexture("assets/copper black font/image8x8.png", true, "'");
    ResourceManager::loadTexture("assets/copper black font/image9x8.png", true, "!");
    ResourceManager::loadTexture("assets/copper black font/image3x9.png", true, "?");
    ResourceManager::loadTexture("assets/copper black font/image2x8.png", true, ".");
    ResourceManager::loadTexture("assets/copper black font/image6x9.png", true, "(");
    ResourceManager::loadTexture("assets/copper black font/image7x9.png", true, ")");
}

std::string symbol = ":,/#'!?.()";
std::string numbers = "1234567890";

void Text::draw(SpriteRenderer& renderer, std::string message){
    for (int i = 0; i < (message.length()); i++){  
        if (message[i] != (' ')){
            glm::vec2 currentCharPosition = position + glm::vec2(i*fontSize.x, 0); 

            if (symbol.find(message[i]) != std::string::npos){
                std::string temp(1, message[i]);
                Texture2D currentMessageCharTexture = ResourceManager::getTexture(temp);
                renderer.drawSprite(currentMessageCharTexture, currentCharPosition, 0, fontSize);   
            }
            else if (numbers.find(message[i]) != std::string::npos){
                if (message[i] != '9')
                    message[i] += 1;
                std::string temp(1, message[i]);
                Texture2D currentMessageCharTexture = ResourceManager::getTexture(temp);
                renderer.drawSprite(currentMessageCharTexture, currentCharPosition, 0, fontSize);   
            }
            else {    
                Texture2D currentMessageCharTexture = ResourceManager::getTexture(std::to_string(message[i]));
                renderer.drawSprite(currentMessageCharTexture, currentCharPosition, 0, fontSize);    
            }
        }
        else {
            continue;
        }       
    }
}

