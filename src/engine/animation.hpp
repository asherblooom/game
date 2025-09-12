#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <glm/glm.hpp>

class Animation {
public:
	bool Finished = false;

	virtual void Run() = 0;
	virtual ~Animation() {}
};

class MoveToAnimation : public Animation {
public:
	MoveToAnimation(glm::vec2& position, glm::vec2 targetLocation, float speed = 1);
	void Run() override;

private:
	glm::vec2& position;
	glm::vec2 targetLocation;
	glm::vec2 direction;
	float speed;
};

class FlipAnimation : public Animation {
public:
	FlipAnimation(glm::vec2& size, float speed = 1);
	void Run() override;

private:
	glm::vec2& size;
	float speed;
};

#endif
