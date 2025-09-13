#include "animation.hpp"
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
	bool setX = false, setY = false;
	// if card is flipping
	glm::vec2 nextPos;
	// only snap if size is original size??
	if (size != originalSize) {
		nextPos = position - (originalSize - size) / glm::vec2(2) + speed * direction;
	} else
		nextPos = position + speed * direction;
	glm::vec2 toMove = position + speed * direction;

	if (direction.x <= 0 && direction.y >= 0) {
		if (nextPos.x <= targetLocation.x) {
			setX = true;
		}
		if (nextPos.y >= targetLocation.y) {
			setY = true;
		}
	} else if (direction.x >= 0 && direction.y <= 0) {
		if (nextPos.x >= targetLocation.x) {
			setX = true;
		}
		if (nextPos.y <= targetLocation.y) {
			setY = true;
		}
	} else if (direction.x >= 0 && direction.y >= 0) {
		if (nextPos.x >= targetLocation.x) {
			setX = true;
		}
		if (nextPos.y >= targetLocation.y) {
			setY = true;
		}
	} else if (direction.x <= 0 && direction.y <= 0) {
		if (nextPos.x <= targetLocation.x) {
			setX = true;
		}
		if (nextPos.y <= targetLocation.y) {
			setY = true;
		}
	}
	if (setX && setY) {
		Finished = true;
		position = targetLocation + (originalSize - size) / glm::vec2(2);
	} else if (setX) {
		position.x = targetLocation.x + (originalSize.x - size.x) / 2;
		position.y = toMove.y;
	} else if (setY) {
		position.y = targetLocation.y + (originalSize.y - size.y) / 2;
		position.x = toMove.x;
	} else
		position = toMove;
}

FlipAnimation::FlipAnimation(glm::vec2& position, glm::vec2& size, int& textureIndex, int cardTexIndex, int backTexIndex, float speed)
	: position{position}, size{size}, textureIndex{textureIndex}, originalSize{size}, cardTexIndex{cardTexIndex}, backTexIndex{backTexIndex}, speed{speed} {}

void FlipAnimation::Run() {
	if (size.x > 0 && size.x <= originalSize.x) {
		size.x += sign * speed;
		// FIXME: bug with modifying position here! affects MoveTo :( :(
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
