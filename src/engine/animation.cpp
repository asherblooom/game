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
