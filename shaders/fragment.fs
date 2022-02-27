#version 330 core

out vec4 fragColour;

in vec3 tempColour;
in vec2 texCoord;

uniform sampler2D myTexture;

void main(){
    fragColour = texture(myTexture, texCoord) * vec4(tempColour, 1.0f);
}