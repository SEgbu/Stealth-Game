// version of OpenGL (3.3 core profile)
#version 330 core 

in vec2 texCoords; // coordinates for texture mapping
out vec4 fragOutput; // what is going on the screen

uniform sampler2D myTexture; // current texture unit
uniform vec3 spriteColour; // colour for colour modulation 

// when rendering...
void main(){
    // ...output the texture and colour modulate it with "spriteColour"
    fragOutput = vec4(spriteColour, 1.0) * texture(myTexture, texCoords);
}