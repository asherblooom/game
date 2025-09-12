#ifndef STATE_INTERFACE_HPP
#define STATE_INTERFACE_HPP

#include <memory>
#include <vector>
#include "../engine/animation.hpp"

class StateInterface {
public:
	std::vector<std::unique_ptr<Animation>> Animations;
	virtual void ProcessInput(float dt) = 0;
	// can return a pointer to a new (heap allocated) state if the state should change
	// otherwise should return nullptr
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
	virtual ~StateInterface() {}
};

#endif
