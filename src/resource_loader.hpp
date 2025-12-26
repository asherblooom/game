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
		ResourceManager::LoadSound("meow", "meow.wav");
		// ResourceManager::LoadSound("countdown", "countdown.ogg");
		ResourceManager::LoadSound("countdown", "countdown.wav", true);
		LoadCardTextures();
	}

private:
	static void LoadCardTextures(bool highRes = false) {
		std::string path;
		if (highRes)
			path = "high-res-cards/";
		else
			path = "cards/";
		std::vector<std::string> names{"BACK", "JOKER-BLACKJOKER", "JOKER-REDJOKER"};
		std::vector<std::string> fileNames{path + "BACK.dds", path + "JOKER-BLACKJOKER.dds", path + "JOKER-REDJOKER.dds"};

		std::string suits[] = {"SPADES", "HEARTS", "DIAMONDS", "CLUBS"};
		std::string values[] = {"ACE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN",
								"EIGHT", "NINE", "TEN", "JACK", "QUEEN", "KING"};
		for (std::string suit : suits) {
			for (std::string value : values) {
				std::string name = value + "-" + suit;
				names.emplace_back(name);
				fileNames.emplace_back(path + name + ".dds");
			}
		}
		ResourceManager::LoadDDSTextureArray("cards", names, fileNames, false);
	}
};

#endif
