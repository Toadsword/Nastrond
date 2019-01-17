/*
MIT License

Copyright (c) 2017 SAE Institute Switzerland AG

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <engine/engine.h>
#include <engine/tilemap.h>
#include <utility/json_utility.h>
#include <gtest/gtest.h>


TEST(Tilemap, TestLoadTilemap)
{

	sfge::Engine engine;
	engine.Init();

	engine.GetTilemapSystem()->GetTileTypeManager()->LoadTileType("data/tilemap/nastrond_tiles.asset");

	json sceneJson;
	json entityJson;
	json tilemapJson;
	tilemapJson["path"] = "data/tilemap/nastrond_tilemap.asset";
	tilemapJson["type"] = static_cast<int>(sfge::ComponentType::TILEMAP);
	tilemapJson["isometric"] = false;
	tilemapJson["size"] = std::array<int, 2>{10, 10};
	tilemapJson["map"] = std::array<std::array<int, 10>, 10>{
		std::array <int, 10>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		std::array <int, 10>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		std::array <int, 10>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		std::array <int, 10>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		std::array <int, 10>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		std::array <int, 10>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		std::array <int, 10>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		std::array <int, 10>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		std::array <int, 10>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		std::array <int, 10>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	};
	entityJson["components"] = json::array({ tilemapJson });
	entityJson["name"] = "TilemapTest";

	sceneJson["entities"] = json::array({ entityJson });
	sceneJson["name"] = "Test Tilemap";
	engine.GetSceneManager()->LoadSceneFromJson(sceneJson);

	//Ce que je faisais :
	// Tenter de créer une scène et y ajouter une tilemap : la créer, faire en sorte qu'elle se génère comme il faut et qu'il affiche son contenu
	// Je dois encore gérer le transform d'une tile en fonction de sa position dans la tilemape, et ne pas oublier le fait que la tilemap a une position de base.
	// Manque encore la gestion du scale des tilemaps, et gérer la position de manière isométrique ou non.



    engine.Start();
}