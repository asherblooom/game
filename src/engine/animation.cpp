#include "animation.hpp"
#include <iostream>
#include "glm/detail/func_geometric.hpp"

MoveToAnimation::MoveToAnimation(glm::vec2& position, glm::vec2 targetLocation, float speed)
	: position{position}, targetLocation{targetLocation}, speed{speed} {
	if (targetLocation == position)
		Finished = true;
	else {
		direction = glm::normalize(targetLocation - position);
	}
}

void MoveToAnimation::Run() {
	glm::vec2 nextPos = position + speed * direction;
	if (direction.x < 0 && direction.y > 0) {
		if (nextPos.x <= targetLocation.x || nextPos.y >= targetLocation.y) {
			Finished = true;
		}
	} else if (direction.x > 0 && direction.y < 0) {
		if (nextPos.x >= targetLocation.x || nextPos.y <= targetLocation.y) {
			Finished = true;
		}
	} else if (direction.x > 0 && direction.y > 0) {
		if (nextPos.x >= targetLocation.x || nextPos.y >= targetLocation.y) {
			Finished = true;
		}
	} else if (direction.x < 0 && direction.y < 0) {
		if (nextPos.x <= targetLocation.x || nextPos.y <= targetLocation.y) {
			Finished = true;
		}
	}
	if (Finished == false)
		position = nextPos;
	else if (Finished == true)
		position = targetLocation;
}

FlipAnimation::FlipAnimation(glm::vec2& position, glm::vec2& size, int& textureIndex, int cardTexIndex, int backTexIndex, float speed)
	: position{position}, size{size}, textureIndex{textureIndex}, originalSize{size}, cardTexIndex{cardTexIndex}, backTexIndex{backTexIndex}, speed{speed} {}

void FlipAnimation::Run() {
	if (size.x > 0 && size.x <= originalSize.x) {
		size.x += sign * speed;
		position.x -= (sign * speed) / 2;
	} else if (size.x <= 0) {
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
