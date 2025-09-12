#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "objects/game_object.hpp"

class Animation {
public:
	bool Finished = false;
	virtual void Run() = 0;
	void updateTargetLocation(GameObject* target);
	virtual ~Animation() {}

protected:
	GameObject* target;
	Animation(GameObject* target) : target{target} {}
};

class MoveToAnimation : public Animation {
public:
	MoveToAnimation(GameObject* target, glm::vec2 location, float speed = 1);
	void Run() override;

private:
	glm::vec2 location;
	glm::vec2 direction;
	float speed;
};

class FlipAnimation : public Animation {
public:
	FlipAnimation(GameObject* target, float speed = 1);
	void Run() override;

private:
	float speed;
};

#endif
