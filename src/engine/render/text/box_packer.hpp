#ifndef BOX_PACKER_HPP
#define BOX_PACKER_HPP

#include <climits>
#include <glm/glm.hpp>
#include <iostream>
#include <list>

using Position = glm::vec2;
using Node = glm::vec2;

class BoxPacker {
public:
	BoxPacker(unsigned int width = 32, unsigned int height = 32)
		: width{width}, height{height} {
		skyline.emplace_back(0, 0);
	}

	// TODO: handle box not fitting!!

	// returns bottom left position??
	// is that what we want????
	Position addBox(glm::vec2 size) {
		std::list<Node>::iterator pos = packBox(size);
		std::cout << "box packed ";
		Node bottomLeft = *pos;
		Node topLeft = {pos->x, pos->y + size.y};
		Node bottomRight = {pos->x + size.x, pos->y};

		skyline.insert(pos, topLeft);
		// insert new bottomRight node
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

		// TODO: generate texture:
		// https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_02

		return bottomLeft;
	}

private:
	std::list<Node> skyline;
	unsigned int width, height;

	std::list<Node>::iterator overlappingNodesEnd;

	// find node with lowest y coord that has space for box
	std::list<Node>::iterator packBox(glm::vec2 size) {
		std::list<Node>::iterator bestNode;
		int minY = INT_MAX;
		std::list<Node>::iterator node = skyline.begin();
		std::cout << "NODES: ";
		for (Node n : skyline) {
			std::cout << "(" << n.x << ", " << n.y << ") ";
		}
		while (node->x + size.x <= width && node != skyline.end()) {
			if (node->y < minY && node->y + size.y <= height) {
				int y = findMinY(size, node);
				if (y < minY) {
					minY = y;
					bestNode = node;
				}
			}
			node++;
		}
		return bestNode;
	}

	int findMinY(glm::vec2 size, std::list<Node>::iterator startingNode) {
		int boxWidth = startingNode->x + size.x;
		int minY = startingNode->y;
		std::list<Node>::iterator node = startingNode;
		overlappingNodesEnd = node;
		node++;
		while (node->x <= boxWidth && node != skyline.end()) {
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
