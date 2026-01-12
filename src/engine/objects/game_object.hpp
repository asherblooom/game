#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <optional>
#include "../animation.hpp"

#include "../render/texture.hpp"

// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class GameObject {
public:
	glm::vec3 Color;
	glm::vec2 Size;
	glm::vec2 Position;
	float Rotation;
	// an object can either have its own texture or have its texture as part of a texture array
	std::optional<Texture2D> Texture;
	std::optional<Texture2DArray> TextureArray;
	int TextureIndex;

	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture2D texture, glm::vec3 color = glm::vec3(1.0f));
	GameObject(glm::vec2 pos, glm::vec2 size, Texture2DArray textureArray, int textureIndex, glm::vec3 color = glm::vec3(1.0f));

	bool DetectMouseOver();
	// Add a MoveTo animation onto the animation stack
	void MoveTo(glm::vec2 location, float speed = 10);
	// Add a RotateTo animation onto the animation stack.
	// newRotation is *not* relative to current rotation (i.e. a value of 45 will rotate the object until it is 45 degrees from rotation 0)
	void RotateTo(int newRotation, float speed = 10, Direction direction = CLOCKWISE);
	// Add a Rotate animation onto the animation stack.
	// degrees *is* relative to current rotation (i.e. a value of 45 will rotate the object until it is 45 degrees from current rotation)
	void Rotate(int degrees, float speed = 10, Direction direction = CLOCKWISE);
	// if current animation hasn't finished, run it for this frame, otherwise start next animation on stack
	void Animate(float dt);
	// returns true if the object has animations that haven't finished yet, false otherwise
	bool HasAnimations();

protected:
	std::map<AnimationType, std::unique_ptr<Animation>> animations;
};

#endif
