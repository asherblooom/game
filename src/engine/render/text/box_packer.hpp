#ifndef BOX_PACKER_HPP
#define BOX_PACKER_HPP

#include <glm/glm.hpp>
#include <list>

using Position = glm::vec2;
using Node = glm::vec2;

class BoxPacker {
public:
	BoxPacker(unsigned int width = 32, unsigned int height = 32);
	// returns bottom left position??
	// is that what we want????
	Position AddBox(glm::vec2 size);

private:
	std::list<Node> skyline;
	unsigned int width, height;

	// returns an iterator to the (starting) node with lowest y coord that has space for box
	std::list<Node>::iterator packBox(glm::vec2 size);
	// returns the smallest y coordinate that startingNode will need to have
	// in order for box to not collide with any other boxes when inserted
	int checkYCollisions(glm::vec2 size, std::list<Node>::iterator startingNode);
};

#endif
