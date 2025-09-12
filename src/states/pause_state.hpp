#ifndef PAUSE_STATE_HPP
#define PAUSE_STATE_HPP

#include "../engine/objects/ui_object.hpp"
#include "../state_manager.hpp"
#include "state_interface.hpp"

#include <map>

class PauseState : public StateInterface {
public:
	PauseState(StateManager& manager);

	void ProcessInput(float dt) override;
	void Update(float dt) override;
	void Render() override;

private:
	StateManager& manager;

	std::map<std::string, std::unique_ptr<Button>> buttons;
	GameObject background;
};

#endif
