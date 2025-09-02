#ifndef STATE_INTERFACE_HPP
#define STATE_INTERFACE_HPP

class StateInterface {
public:
	virtual void ProcessInput(float dt) = 0;
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
};

#endif
