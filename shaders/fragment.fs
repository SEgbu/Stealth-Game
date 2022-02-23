#version 330 core

out vec4 fragColour;
in vec3 tempColour;

void main(){
    fragColour = vec4(tempColour, 1.0);
}