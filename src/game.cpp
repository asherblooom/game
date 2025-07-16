#include "game.hpp"

#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>
#include <memory>
#include "engine/objects/game_object.hpp"
#include "engine/resource_manager.hpp"

Game::Game(unsigned int width, unsigned int height)
	: width(width), height(height) {
}

void Game::Init() {
	// load shaders
	ResourceManager::LoadShader("sprite", "src/shaders/sprite.vs", "src/shaders/sprite.frag");
	// configure shaders
	glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
	Shader& spriteShader = ResourceManager::GetShader("sprite");
	spriteShader.Use();
	spriteShader.SetInteger("image", 0);
	spriteShader.SetMatrix4("projection", projection);
	// set render-specific controls
	renderer = new SpriteRenderer();
	// load textures
	ResourceManager::LoadDDSTexture("ball", "textures/circle.dds");
}

void Game::ProcessInput(float dt) {
	// if there is a selected ball and the mouse button is no longer held, make it react to forces
	// if the mouse pointer is no longer over it, deselect it
	if (selectedBall && !MouseButtons[GLFW_MOUSE_BUTTON_LEFT]) {
		selectedBall->Physics->Static = false;
		if (!(selectedBall->BoundingVolume->DetectMouseOver(MousePos))) {
			selectedBall->Render->Color -= glm::vec3(0.1);
			selectedBall = nullptr;
		}
	}
	// if no ball currently selected, select a ball which is over the mouse pointer
	// loop through balls in reverse order, so as to pick the one on top (drawn last) if any overlap
	if (!selectedBall) {
		for (int i = balls.size() - 1; i >= 0; i--) {
			auto& ball = balls[i];
			if (ball.BoundingVolume->DetectMouseOver(MousePos)) {
				selectedBall = &ball;
				// make selected ball brighter
				ball.Render->Color += glm::vec3(0.1);
				// only select one ball
				break;
			}
		}
	}
}

// TODO: make movement fps independent!!!
// FIXME: stop rendering/updating balls once they leave the frame!!
void Game::Update(float dt) {
	// if there is a selected ball and the mouse is down, make it follow the mouse pointer
	if (selectedBall && MouseButtons[GLFW_MOUSE_BUTTON_LEFT]) {
		selectedBall->transform->Position += ChangeInMousePos;
	}
}

void Game::Render() {
	for (GameObject& ball : balls) {
		renderer->DrawSprite(*ball.Render);
	}
	renderer->DrawSprite(*container->Render);
}

GameObject& Game::makeBall(glm::vec2 center, glm::vec3 color, glm::vec2 velocity) {
	auto ballTex = ResourceManager::GetTexture("ball");
	auto ballShader = ResourceManager::GetShader("sprite");
	float diameter = 50.0f;
	glm::vec2 pos = center - glm::vec2(diameter / 2.0f);

	if (selectedBall) {
		int selectedLoc = 0;
		for (int i = 0; i < (int)balls.size(); i++) {
			if (&balls.at(i) == selectedBall) {
				selectedLoc = i;
				selectedBall = nullptr;
			}
		}
		balls.emplace_back(pos, glm::vec2(diameter), CIRCLE, ballTex, ballShader, 1.0f, velocity, color);
		selectedBall = &balls.at(selectedLoc);
	} else
		balls.emplace_back(pos, glm::vec2(diameter), CIRCLE, ballTex, ballShader, 1.0f, velocity, color);
	return balls.back();
}

void Game::LoadCardTextures() {
	ResourceManager::LoadDDSTexture("ACE", "ACE.dds");
	ResourceManager::LoadDDSTexture("ONE", "ONE.dds");
	ResourceManager::LoadDDSTexture("TWO", "TWO.dds");
	ResourceManager::LoadDDSTexture("THREE", "THREE.dds");
	ResourceManager::LoadDDSTexture("FOUR", "FOUR.dds");
	ResourceManager::LoadDDSTexture("FIVE", "FIVE.dds");
	ResourceManager::LoadDDSTexture("SIX", "SIX.dds");
	ResourceManager::LoadDDSTexture("SEVEN", "SEVEN.dds");
	ResourceManager::LoadDDSTexture("EIGHT", "EIGHT.dds");
	ResourceManager::LoadDDSTexture("NINE", "NINE.dds");
	ResourceManager::LoadDDSTexture("TEN", "TEN.dds");
	ResourceManager::LoadDDSTexture("JACK", "JACK.dds");
	ResourceManager::LoadDDSTexture("QUEEN", "QUEEN.dds");
	ResourceManager::LoadDDSTexture("KING", "KING.dds");
	ResourceManager::LoadDDSTexture("JOKER", "JOKER.dds");
}
