#ifndef STATE_INTERFACE_HPP
#define STATE_INTERFACE_HPP

class StateInterface {
public:
	// called when state is pushed to stack
	virtual void OnEnter() = 0;
	virtual void ProcessInput(float dt) = 0;
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
	// called when state is popped from stack
	virtual void OnExit() = 0;
	virtual ~StateInterface() = default;
};

#endif
