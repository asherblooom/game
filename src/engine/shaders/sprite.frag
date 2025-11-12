#version 330 core
in vec2 vertTexCoords;
out vec4 color;

uniform sampler2D image;
uniform sampler2DArray textureArray;
uniform int textureIndex;
uniform vec3 spriteColor;

void main()
{
	//set as -1 in sprite_renderer.cpp if we want to use "image" sampler 
	if (textureIndex < 0){
		color = vec4(spriteColor, 1.0) * texture(image, vertTexCoords);
	} else {
		color = vec4(spriteColor, 1.0) * texture(textureArray, vec3(vertTexCoords, float(textureIndex)));
	}
}
