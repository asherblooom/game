#include "animation.hpp"
#include "glm/detail/func_geometric.hpp"

void Animation::updateTargetLocation(GameObject* target) {
}

MoveToAnimation::MoveToAnimation(GameObject* target, glm::vec2 location, float speed)
	: Animation{target}, location{location}, speed{speed} {
	direction = glm::normalize(location - target->Position);
}

void MoveToAnimation::Run() {
	if (direction.x < 0 && direction.y > 0) {
		if (target->Position.x <= location.x || target->Position.y >= location.y) {
			Finished = true;
		}
	} else if (direction.x > 0 && direction.y < 0) {
		if (target->Position.x >= location.x || target->Position.y <= location.y) {
			Finished = true;
		}
	} else if (direction.x > 0 && direction.y > 0) {
		if (target->Position.x >= location.x || target->Position.y >= location.y) {
			Finished = true;
		}
	} else if (direction.x < 0 && direction.y < 0) {
		if (target->Position.x <= location.x || target->Position.y <= location.y) {
			Finished = true;
		}
	}
	if (Finished == false)
		target->Position += speed * direction;
	else if (Finished == true)
		target->Position = location;

	// TODO: if array changes size, pointer to object changes size as well!
}
