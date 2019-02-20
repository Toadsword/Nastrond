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

#ifndef SFGE_IMAGE_H
#define SFGE_IMAGE_H

#include <string>
#include <SFML/Graphics.hpp>
#include <engine/component.h>
#include <engine/rect_transform.h>
#include <graphics/texture.h>

namespace sfge
{
	class Image final
	{
	public:
		Image& operator=(const Image&);

		Image();

		void Init();
		void Update(Vec2f position);//RectTransform* rectTransform);
		void Draw(sf::RenderWindow& window);

		void SetSprite(std::string spritePath);
		void SetColor(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Uint8 a);
		void SetColor(sf::Color color);

		std::string spritePath;
		sf::Uint8 color[4] = {0, 0, 0, 255};
		TextureId textureId;
		sf::Sprite sprite;
	};
	namespace editor
	{
		struct ImageInfo : ComponentInfo
		{
			void DrawOnInspector() override;
			Image* image = nullptr;
		};
	}

	class ImageManager final : public SingleComponentManager<Image, editor::ImageInfo, ComponentType::IMAGE> {
	public:
		using SingleComponentManager::SingleComponentManager;

		ImageManager(Engine& engine);
		~ImageManager();

		ImageManager& operator=(const Image&);

		void CreateComponent(json& componentJson, Entity entity) override;
		Image* AddComponent(Entity entity) override;
		void DestroyComponent(Entity entity) override;

		void Init() override;
		void Update(float dt) override;
		void DrawImages(sf::RenderWindow& window);

		void LoadSprites();
		void SetSprite(Image* image) const;
	protected:
		RectTransformManager* m_RectTransformManager;
		TextureManager* m_TextureManager;
	};
}
#endif