#ifndef RESOURCE_LOADER_HPP
#define RESOURCE_LOADER_HPP

#include "engine/resource_manager.hpp"

class ResourceLoader {
public:
	static void LoadAll() {
		ResourceManager::LoadDDSTexture("background0", "background0.dds");
		ResourceManager::LoadDDSTexture("play-button", "play-button.dds");
		ResourceManager::LoadDDSTexture("pause-button", "pause-button.dds");
		ResourceManager::LoadDDSTexture("return-button", "return-button.dds");
		ResourceManager::LoadFont("default-100", "OpenSans-Regular.ttf", 100);
		ResourceManager::LoadFont("default-50", "OpenSans-Regular.ttf", 50);
		ResourceManager::LoadFont("default-25", "OpenSans-Regular.ttf", 25);
		LoadCardTextures();
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
