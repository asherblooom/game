#ifndef PAUSE_MENU_HPP
#define PAUSE_MENU_HPP

#include "../engine/objects/ui_object.hpp"
#include "../state_manager.hpp"
#include "state_interface.hpp"

#include <map>

class PauseMenu : public StateInterface {
public:
	PauseMenu(StateManager& manager);

	void ProcessInput(float dt) override;
	void Update(float dt) override;
	void Render() override;

private:
	StateManager& manager;

	std::map<std::string, std::unique_ptr<Button>> buttons;
	GameObject background;
};

#endif
