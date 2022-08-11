#version 330 core

in vec2 texCoords;
out vec4 fragOutput;

uniform sampler2D myTexture;
uniform vec3 spriteColour;

void main(){
    fragOutput = texture(myTexture, texCoords) * vec4(spriteColor, 1.0);
}