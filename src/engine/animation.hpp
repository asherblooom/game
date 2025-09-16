#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <glm/glm.hpp>

enum AnimationType {
	MOVETO,
	FLIP,
	ROTATE
};

class Animation {
public:
	bool Finished = false;

	virtual void Run() = 0;
	// virtual ~Animation() {}
};

class MoveToAnimation : public Animation {
public:
	MoveToAnimation(glm::vec2& position, glm::vec2& size, glm::vec2 targetLocation, float speed);
	void Run() override;

private:
	glm::vec2& position;
	glm::vec2& size;
	glm::vec2 targetLocation;
	glm::vec2 direction;
	glm::vec2 originalSize;
	float speed;
};

class FlipAnimation : public Animation {
public:
	FlipAnimation(glm::vec2& position, glm::vec2& size, int& textureIndex, int cardTexIndex, int backTexIndex, float speed);
	void Run() override;

private:
	glm::vec2& position;
	glm::vec2& size;
	int& textureIndex;
	glm::vec2 originalSize;
	int cardTexIndex;
	int backTexIndex;
	float speed;
	int sign = -1;
};

enum Direction {
	CLOCKWISE,
	ANTICLOCKWISE
};

class RotateAnimation : public Animation {
public:
	RotateAnimation(float& rotation, int targetRotation, float speed, Direction direction);
	void Run() override;

private:
	float& rotation;
	int targetRotation;
	float speed;
	Direction direction;
};

#endif
