#ifndef STATE_INTERFACE_HPP
#define STATE_INTERFACE_HPP

class StateInterface {
public:
	virtual void ProcessInput(float dt) = 0;
	// can return a pointer to a new (heap allocated) state if the state should change
	// otherwise should return nullptr
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
	virtual ~StateInterface() {}
};

#endif
