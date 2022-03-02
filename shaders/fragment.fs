#version 330 core

out vec4 fragColour;

in vec3 tempColour;
in vec2 texCoord;

uniform sampler2D myTexture;
uniform sampler2D myTexture2;

void main(){
    fragColour = mix(texture(myTexture, texCoord), texture(myTexture2, texCoord), 0.45);
}