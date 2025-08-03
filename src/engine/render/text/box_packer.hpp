#ifndef BOX_PACKER_HPP
#define BOX_PACKER_HPP

#include <glm/glm.hpp>
#include <list>
#include <vector>

using Position = glm::vec2;
using Node = glm::vec2;

class BoxPacker {
public:
	BoxPacker(unsigned int width = 32, unsigned int height = 32)
		: width{width}, height{height} {
		skyline.emplace_back(0, 0);
	}

	// TODO: handle box not fitting!!

	// returns top left position??
	// is that what we want????
	Position addBox(glm::vec2 size) {
		std::list<Node>::iterator bottomLeft = packBox(size);
		Node topLeft = {bottomLeft->x, bottomLeft->y + size.y};
		Node bottomRight = {bottomLeft->x + size.x, bottomLeft->y};

		// replace bottomLeft with topLeft coords
		*bottomLeft = topLeft;
		//check if previous node is duplicate
		std::list<Node>::iterator prev = bottomLeft;
		if (*(--prev) == topLeft)
			skyline.erase(prev);

		// insert new bottomRight node
		std::list<Node>::iterator insertPos = overlappingNodesEnd;
		insertPos++;
		if (overlappingNodesEnd->y < bottomRight.y) {
			Node newNode = {bottomRight.x, overlappingNodesEnd->y};
			skyline.insert(insertPos, newNode);
		} else if (overlappingNodesEnd->x == bottomRight.x) {
		}
		// delete all nodes that have x coords overlapping with our new box
		std::list<Node>::iterator node = bottomLeft;
		skyline.erase(++node, ++overlappingNodesEnd);
		// while (node->x <= topLeft.x + size.x && node != skyline.end()) {
		// 	yUnderneath = node->y;
		// 	std::list<Node>::iterator deleteNode = node;
		// 	node++;
		// 	skyline.erase(deleteNode);
		// }
		// TODO: update skyline edge cases:
		// https://jvernay.fr/en/blog/skyline-2d-packer/implementation/
		// TODO: generate texture:
		// https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_02

		return topLeft;
	}

private:
	std::list<Node> skyline;
	unsigned int width, height;

	std::list<Node>::iterator overlappingNodesEnd;

	// find node with lowest y coord that has space for box
	std::list<Node>::iterator packBox(glm::vec2 size) {
		std::list<Node>::iterator bestNode;
		int minY;
		std::list<Node>::iterator node = skyline.begin();
		while (node->x + size.x <= width && node != skyline.end()) {
			if (node->y < minY && node->y + size.y <= height) {
				int y = findMinY(size, node);
				if (y < minY) {
					minY = y;
					bestNode = node;
				}
				node++;
			}
		}
		return bestNode;
	}

	int findMinY(glm::vec2 size, std::list<Node>::iterator startingNode) {
		int boxWidth = startingNode->x + size.x;
		int minY = startingNode->y;
		std::list<Node>::iterator node = startingNode;
		overlappingNodesEnd = node;
		node++;
		while (node->x <= boxWidth) {
			if (node->x < boxWidth) {
				if (node->y > minY) {
					// raise minY higher to avoid collision
					minY = node->y;
				}
			}
			overlappingNodesEnd = node;
			node++;
		}
		return minY;
	}
};

#endif
