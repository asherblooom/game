#ifndef MENU_STATE_HPP
#define MENU_STATE_HPP

#include "../engine/resource_manager.hpp"
#include "../state_manager.hpp"
#include "state_interface.hpp"

class MenuState : public StateInterface {
public:
	MenuState(StateManager& manager) : manager{manager} {
		background = GameObject({0, 0}, {manager.Width, manager.Width * (9.0 / 16.0)}, ResourceManager::GetTexture("background"));
	}

	void ProcessInput(float dt) override;
	void Update(float dt) override;
	void Render() override;

private:
	StateManager& manager;

	GameObject background;
};

#endif
