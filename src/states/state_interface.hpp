#ifndef STATE_INTERFACE_HPP
#define STATE_INTERFACE_HPP

class StateInterface {
	virtual void Init() = 0;
	virtual void ProcessInput(float dt) = 0;
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
};

#endif
