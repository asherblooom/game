#include "animation.hpp"
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
