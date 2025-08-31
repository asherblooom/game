#version 330 core
in vec2 vertTexCoords;
in flat int textureIndex;
out vec4 color;

uniform sampler2D image;
uniform sampler2DArray textureArray;
uniform vec3 spriteColor;

void main()
{
	if (textureIndex < 0){
		color = vec4(spriteColor, 1.0) * texture(image, vertTexCoords);
	} else {
		color = vec4(spriteColor, 1.0) * texture(textureArray, vec3(vertTexCoords.x, verTexCoords.y, textureIndex));
	}
}
