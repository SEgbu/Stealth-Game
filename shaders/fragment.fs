#version 330 core

in vec2 texCoords;
out vec4 fragOutput;

uniform sampler2D myTexture;

void main(){
    fragOutput = texture(myTexture, texCoords);
}