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

#ifndef SFGE_TILEMAP_H_
#define SFGE_TILEMAP_H_

 //tool_engine
#include <engine/component.h>
#include <engine/transform2d.h>

namespace sfge
{

class Tile : public TransformRequiredComponent
{
	Tile();
	Tile(Transform2d* transform);

	void Init();
	void Update();

protected:
	short layer = -1;
	int type = -1;
};


namespace editor
{
	struct TileInfo : ComponentInfo
	{
		void DrawOnInspector() override;
		Tile* tile = nullptr;
		short layer = -1;
		int type = -1;
	};
}

class TileManager :
	public SingleComponentManager<Tile, editor::TileInfo, ComponentType::TILE>
{
public:
	using SingleComponentManager::SingleComponentManager;
	void Init() override;
	void Update(float dt) override;

	void Collect() override;
	Tile* AddComponent(Entity entity) override;
	void CreateComponent(json& componentJson, Entity entity) override;
	void DestroyComponent(Entity entity) override;
	void OnResize(size_t new_size) override;
};




class Tilemap: public TransformRequiredComponent
{
	Tilemap();
	Tilemap(Transform2d* transform, sf::Vector2f offset);

	void Init();
	void Update();

protected:
	std::vector<Entity> tiles;
	short layer = 0;
};


namespace editor
{
struct TilemapInfo : ComponentInfo
{
	void DrawOnInspector() override;
	Tilemap* tilemap = nullptr;
	short layer = 0;
};
}

class TilemapManager :
	public SingleComponentManager<Tilemap, editor::TilemapInfo, ComponentType::TILEMAP>
{
public:
	using SingleComponentManager::SingleComponentManager;
	void Init() override;
	void Update(float dt) override;

	void Collect() override;
	Tilemap* AddComponent(Entity entity) override;
	void CreateComponent(json& componentJson, Entity entity) override;
	void DestroyComponent(Entity entity) override;
	void OnResize(size_t new_size) override;
};
}

#endif /* INCLUDE_ENGINE_TILEMAP_H_ */
