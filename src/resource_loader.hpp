#ifndef RESOURCE_LOADER_HPP
#define RESOURCE_LOADER_HPP

#include "engine/resource_manager.hpp"

class ResourceLoader {
public:
	static void LoadAll() {
		ResourceManager::LoadDDSTexture("background", "background.dds");
		ResourceManager::LoadDDSTexture("button", "button.dds");
		LoadCardTextures();
		ResourceManager::LoadFont("default", "OpenSans-Regular.ttf", 32);
	}

private:
	static void LoadCardTextures() {
		std::vector<std::string> names{"JOKER-BLACKJOKER", "JOKER-REDJOKER"};
		std::vector<std::string> fileNames{"JOKER-BLACKJOKER.dds", "JOKER-REDJOKER.dds"};

		std::string suits[] = {"SPADES", "HEARTS", "DIAMONDS", "CLUBS"};
		std::string values[] = {"ACE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN",
								"EIGHT", "NINE", "TEN", "JACK", "QUEEN", "KING"};
		for (std::string suit : suits) {
			for (std::string value : values) {
				std::string name = value + "-" + suit;
				names.emplace_back(name);
				fileNames.emplace_back(name + ".dds");
				// ResourceManager::LoadDDSTexture(name, (name + ".dds"), false);
			}
		}
		ResourceManager::LoadDDSTextureArray("cards", names, fileNames, false);
	}
};

#endif
