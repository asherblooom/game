#include "sprite_renderer.hpp"
#include <iostream>
#include <vector>
#include "../resource_manager.hpp"
#include "vertex.hpp"

SpriteRenderer::SpriteRenderer(unsigned int width, unsigned int height) {
	shader = ResourceManager::LoadShader("sprite", "sprite.vert", "sprite.frag");
	shader.SetMatrix4("projection", glm::ortho(0.0f, (float)(width), (float)(height), 0.0f, -1.0f, 1.0f), true);
	// configure VAO/VBO
	unsigned int VBO;
	std::vector<Vertex> vertices = {
		{0.0f, 1.0f, 0.0f, 1.0f},
		{1.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 0.0f},

		{0.0f, 1.0f, 0.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 0.0f, 1.0f, 0.0f}};

	// initialise VBO buffer and fill it with data from vertices
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	// transfer data from VBO buffer to VAO indices 0 and 1
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, s));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
}

SpriteRenderer::~SpriteRenderer() {
	glDeleteVertexArrays(1, &VAO);
}

void SpriteRenderer::Draw(GameObject* object) {
	// prepare transformations
	shader.Use();
	glm::mat4 model = glm::mat4(1.0f);
	// first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)
	model = glm::translate(model, glm::vec3(object->Position, 0.0f));

	model = glm::translate(model, glm::vec3(0.5f * object->Size.x, 0.5f * object->Size.y, 0.0f));	 // move origin of rotation to center of quad
	model = glm::rotate(model, glm::radians(object->Rotation), glm::vec3(0.0f, 0.0f, 1.0f));		 // then rotate
	model = glm::translate(model, glm::vec3(-0.5f * object->Size.x, -0.5f * object->Size.y, 0.0f));	 // move origin back

	model = glm::scale(model, glm::vec3(object->Size, 1.0f));  // last scale
	shader.SetMatrix4("model", model);

	shader.SetVector3f("spriteColor", object->Color);

	// set uniform variables to the correct texture locations
	shader.SetInteger("image", 0);
	shader.SetInteger("textureArray", 1);
	if (object->Texture) {
		glActiveTexture(GL_TEXTURE0);
		object->Texture->Bind();
		shader.SetInteger("textureIndex", -1);	//-1 means shader will know to use "image" sampler instead of "textureArray"
	} else if (object->TextureArray) {
		glActiveTexture(GL_TEXTURE1);
		object->TextureArray->Bind();
		shader.SetInteger("textureIndex", object->TextureIndex);
	} else {
		std::cerr << "ERROR::RENDERER: cannot render object that has no texture";
		return;
	}

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
