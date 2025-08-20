#include <ft2build.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include FT_FREETYPE_H

#include "../../resource_manager.hpp"
#include "../vertex.hpp"
#include "font.hpp"
#include "text_renderer.hpp"

TextRenderer::TextRenderer(unsigned int width, unsigned int height) {
	// load and configure shader
	textShader = ResourceManager::LoadShader("text", "text.vert", "text.frag");
	textShader.SetMatrix4("projection", glm::ortho(0.0f, (float)(width), (float)(height), 0.0f, -1.0f, 1.0f), true);
	textShader.SetInteger("text", 0);
	// configure VAO/VBO for texture quads
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 6, NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, x));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, s));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TextRenderer::RenderText(std::string text, float x, float y, float scale, Font font, glm::vec3 color) {
	// activate corresponding render state
	textShader.Use();
	textShader.SetVector3f("textColor", color);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, font.TextureAtlas);
	glBindVertexArray(VAO);

	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		CharacterData ch = font.Characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y + (font.Characters['H'].Bearing.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// update VBO for each character
		// std::vector<Vertex> vertices = {
		// 	{xpos, ypos + h, 0.0f, 1.0f},
		// 	{xpos + w, ypos, 1.0f, 0.0f},
		// 	{xpos, ypos, 0.0f, 0.0f},
		//
		// 	{xpos, ypos + h, 0.0f, 1.0f},
		// 	{xpos + w, ypos + h, 1.0f, 1.0f},
		// 	{xpos + w, ypos, 1.0f, 0.0f}};
		//
		// std::vector<Vertex> vertices = {
		// 	{xpos, ypos + h, 0.0f, (float)font.AtlasSize.y},
		// 	{xpos + w, ypos, (float)font.AtlasSize.x, 0.0f},
		// 	{xpos, ypos, 0.0f, 0.0f},
		//
		// 	{xpos, ypos + h, 0.0f, (float)font.AtlasSize.y},
		// 	{xpos + w, ypos + h, (float)font.AtlasSize.x, (float)font.AtlasSize.y},
		// 	{xpos + w, ypos, (float)font.AtlasSize.x, 0.0f}};
		//
		std::vector<Vertex> vertices = {
			{xpos, ypos + h, ch.TexPos.x / font.AtlasSize.x, (ch.TexPos.y + ch.Size.y) / font.AtlasSize.y},
			{xpos + w, ypos, (ch.TexPos.x + ch.Size.x) / font.AtlasSize.x, ch.TexPos.y / font.AtlasSize.y},
			{xpos, ypos, ch.TexPos.x / font.AtlasSize.x, ch.TexPos.y / font.AtlasSize.y},

			{xpos, ypos + h, ch.TexPos.x / font.AtlasSize.x, (ch.TexPos.y + ch.Size.y) / font.AtlasSize.y},
			{xpos + w, ypos + h, (ch.TexPos.x + ch.Size.x) / font.AtlasSize.x, (ch.TexPos.y + ch.Size.y) / font.AtlasSize.y},
			{xpos + w, ypos, (ch.TexPos.x + ch.Size.x) / font.AtlasSize.x, ch.TexPos.y / font.AtlasSize.y}};
		//
		// std::vector<Vertex> vertices = {
		// 	{xpos, ypos + h, (float)ch.TexPos.x, (float)(ch.TexPos.y + ch.Size.y)},
		// 	{xpos + w, ypos, (float)(ch.TexPos.x + ch.Size.x), (float)ch.TexPos.y},
		// 	{xpos, ypos, (float)ch.TexPos.x, (float)ch.TexPos.y},
		//
		// 	{xpos, ypos + h, (float)ch.TexPos.x, (float)(ch.TexPos.y + ch.Size.y)},
		// 	{xpos + w, ypos + h, (float)(ch.TexPos.x + ch.Size.x), (float)(ch.TexPos.y + ch.Size.y)},
		// 	{xpos + w, ypos, (float)(ch.TexPos.x + ch.Size.x), (float)ch.TexPos.y}};

		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// be sure to use glBufferSubData and not glBufferData
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * vertices.size(), vertices.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph
		x += (ch.Advance >> 6) * scale;	 // bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
