#version 330 core
in vec2 vertTexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main() {
	ivec2 coords = ivec2(vertTexCoords.x, vertTexCoords.y);
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, vertTexCoords).r);
    color = vec4(textColor, 1.0) * sampled;
}
