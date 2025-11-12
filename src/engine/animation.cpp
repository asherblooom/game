#include "animation.hpp"
#include <iostream>
#include "glm/detail/func_geometric.hpp"

MoveToAnimation::MoveToAnimation(glm::vec2& position, glm::vec2& size, glm::vec2 targetLocation, float speed)
	: position{position}, size{size}, targetLocation{targetLocation}, originalSize{size}, speed{speed} {
	if (targetLocation == position)
		Finished = true;
	else {
		direction = glm::normalize(targetLocation - position);
	}
}

void MoveToAnimation::Run() {
	bool finishX = false, finishY = false;
	// If size has changed since MoveTo was started, change nextPos
	// so that it is where the top left of the object would be if the card had original size.
	// This is so that objects end up centered on the target location if their size is smaller than original,
	// instead of aligned on the top left corner.
	// This is done to prevent issues with this animation interacting with flipping animation
	glm::vec2 nextPos = position - (originalSize - size) / glm::vec2(2) + speed * direction;
	glm::vec2 toMove = position + speed * direction;

	if (direction.x <= 0) {
		if (nextPos.x <= targetLocation.x) finishX = true;
	} else if (direction.x > 0) {
		if (nextPos.x > targetLocation.x) finishX = true;
	}
	if (direction.y <= 0) {
		if (nextPos.y <= targetLocation.y) finishY = true;
	} else if (direction.y > 0) {
		if (nextPos.y > targetLocation.y) finishY = true;
	}

	if (finishX && finishY) {
		Finished = true;
		// snap to center position, instead of top left aligned position (see above for explanation)
		position = targetLocation + (originalSize - size) / glm::vec2(2);
	} else if (finishX) {
		position.x = targetLocation.x + (originalSize.x - size.x) / 2;
		position.y = toMove.y;
	} else if (finishY) {
		position.y = targetLocation.y + (originalSize.y - size.y) / 2;
		position.x = toMove.x;
	} else
		position = toMove;
}

FlipAnimation::FlipAnimation(glm::vec2& position, glm::vec2& size, int& textureIndex, int cardTexIndex, int backTexIndex, float speed)
	: position{position}, size{size}, textureIndex{textureIndex}, originalSize{size}, cardTexIndex{cardTexIndex}, backTexIndex{backTexIndex}, speed{speed} {}

void FlipAnimation::Run() {
	if (size.x > 0 && size.x <= originalSize.x) {
		// decrease/increase size (depending on sign) until we get to 0 or original size
		size.x += sign * speed;
		// modify position so that object looks like it is turning around its center rather than its left side
		// (as positions describe the top left coordinate of an object)
		position.x -= (sign * speed) / 2;
	} else if (size.x <= 0) {
		// when we get to 0, reverse direction, and change texture
		sign = 1;
		size.x = 1;
		if (textureIndex == cardTexIndex) {
			textureIndex = backTexIndex;
		} else {
			textureIndex = cardTexIndex;
		}
	}
	if (size.x >= originalSize.x) {
		size.x = originalSize.x;
		Finished = true;
	}
}

RotateAnimation::RotateAnimation(float& rotation, int targetRotation, float speed, Direction direction)
	: rotation{rotation}, targetRotation{targetRotation}, speed{speed}, direction{direction} {
	// make sure targetRotation is always greater than rotation for clockwise
	if (targetRotation < rotation && direction == CLOCKWISE) this->targetRotation += 360;
	// make sure targetRotation is always less than rotation for anticlockwise
	if (targetRotation > rotation && direction == ANTICLOCKWISE) this->targetRotation -= 360;
	// ensure that targetRotation and rotation are not below 0, as this complicates anticlockwise calculations
	if (direction == ANTICLOCKWISE) {
		this->targetRotation += 360;
		this->rotation += 360;
	}
}

void RotateAnimation::Run() {
	float newRotation = 0;
	if (direction == CLOCKWISE) {
		newRotation = rotation + speed;
		if (newRotation >= targetRotation) {
			// we use modulo 360 as targetRotation might be greater than 360
			rotation = (int)targetRotation % 360;
			Finished = true;
			return;
		}
	} else if (direction == ANTICLOCKWISE) {
		newRotation = rotation - speed;
		if (newRotation <= targetRotation) {
			rotation = (int)targetRotation % 360;
			Finished = true;
			return;
		}
	}
	rotation = newRotation;
}
