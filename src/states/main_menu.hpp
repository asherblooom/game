#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "../engine/objects/ui_object.hpp"
#include "../state_manager.hpp"
#include "state_interface.hpp"

#include <map>

class MainMenu : public StateInterface {
public:
	MainMenu(StateManager& manager);

	void Update(float dt) override;
	void Render() override;

	void OnEnter() override {}
	void OnExit() override {}

private:
	StateManager& manager;

	std::map<std::string, std::unique_ptr<Button>> buttons;
	GameObject background;
};

#endif
