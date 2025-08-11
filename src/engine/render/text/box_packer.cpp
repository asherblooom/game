#include "box_packer.hpp"

BoxPacker::BoxPacker(unsigned int width, unsigned int height)
	: width{width}, height{height} {
	skyline.emplace_back(0, 0);
}

// returns bottom left position??
// is that what we want????
Position BoxPacker::AddBox(glm::vec2 size) {
	std::list<Node>::iterator pos = packBox(size);
	int boxWidth = pos->x + size.x;
	Node bottomLeft = *pos;
	Node topLeft = {pos->x, pos->y + size.y};
	Node bottomRight = {pos->x + size.x, pos->y};

	// find the node closest to the end of the box that is overlapped by the box
	std::list<Node>::iterator overlappingNodesEnd;
	for (auto node = pos; node != skyline.end(); node++) {
		if (node->x <= boxWidth)
			overlappingNodesEnd = node;
		else
			break;
	}

	// topLeft is always inserted
	skyline.insert(pos, topLeft);
	// check for bottomRight insertion
	if (bottomRight.x == width) {
		// don't add a bottomRight node
	} else if (overlappingNodesEnd->x == bottomRight.x) {
		// leave that node (with same x as bottom right) untouched, and don't add a bottomRight
		overlappingNodesEnd--;
	} else if (overlappingNodesEnd->y < bottomRight.y) {
		// make our bottomRight go all the way down
		Node newBR = {bottomRight.x, overlappingNodesEnd->y};
		skyline.insert(pos, newBR);
	} else
		skyline.insert(pos, bottomRight);
	// delete all nodes that have x coords overlapping with our new box
	skyline.erase(pos, ++overlappingNodesEnd);

	return bottomLeft;
}

// returns an iterator to the (starting) node with lowest y coord that has space for box
std::list<Node>::iterator BoxPacker::packBox(glm::vec2 size) {
	std::list<Node>::iterator bestNode;
	int bestY = height;
	std::list<Node>::iterator node = skyline.begin();
	while (node->x + size.x <= width && node != skyline.end()) {
		// if we think we have a new best y, check that the box can
		// be fitted at that y without overlapping another box,
		// and then if it is still best y, set best y
		if (node->y < bestY && node->y + size.y <= height) {
			int y = checkYCollisions(size, node);
			if (y < bestY && y + size.y <= height) {
				bestY = y;
				bestNode = node;
			}
		}
		node++;
	}

	if (bestY + size.y > height) {
		// not enough room for box, so double size and try again
		width *= 2;
		height *= 2;
		return packBox(size);
	} else
		return bestNode;
}

// returns the smallest y coordinate that startingNode will need to have
// in order for box to not collide with any other boxes when inserted
int BoxPacker::checkYCollisions(glm::vec2 size, std::list<Node>::iterator startingNode) {
	int boxWidth = startingNode->x + size.x;
	int minY = startingNode->y;
	std::list<Node>::iterator node = startingNode;
	node++;
	// iterate from starting node to width of box and check for boxes with overlapping y-coords
	while (node->x < boxWidth && node != skyline.end()) {
		if (node->y > minY) {
			// raise minY higher to avoid collision
			minY = node->y;
		}
		node++;
	}
	return minY;
}
