// version of OpenGL (3.3 core profile)
#version 330 core
// vertex attribute 
layout (location = 0) in vec4 vertex; // vec2 pos, vec2 tex

// texture mapping coordinates that is going to the vertex shader
out vec2 texCoords;

uniform mat4 model; // uniform matrix for transforming objects 
uniform mat4 projection; // uniform matrix for displaying the game

void main(){
    texCoords = vertex.zw; // set the tex coords to the last two floats of each row in vertices (in the sprite renderer, initRenderData)
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0); // sets position and size of the object which is warped by the projection and model matrix
}