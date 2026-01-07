#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D objectTexture; // takes as a postfix of the current assinged texture
uniform float opacity;

void main()
{
   // A value of 0.2 will return 80% of the first input color and 20% of the second input color, resulting in a mixture of both our textures. 
   FragColor = texture(objectTexture, TexCoord);
};