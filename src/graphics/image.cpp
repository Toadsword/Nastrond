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

#include <graphics/image.h>

namespace sfge
{
	void ImageInfo::DrawOnInspector()
	{
		
	}

	void ImageManager::CreateComponent(json& componentJson, Entity entity)
	{
		auto* image = AddComponent(entity);

		sf::Color sampleColor = sampleColor.Black;

		if (CheckJsonExists(componentJson, "sprite"))
		{
			if (image->texture.loadFromFile(componentJson["sprite"]))
				image->sprite.setTexture(image->texture);

			if (CheckJsonExists(componentJson, "color"))
			{
				/*
				* Color is an array in the json :
				* "color": [r,g,b,a]
				*/
				image->color.r = componentJson["color"][0];
				image->color.g = componentJson["color"][1];
				image->color.b = componentJson["color"][2];
				image->color.a = componentJson["color"][3];

				image->sprite.setColor(image->color);
			}
			else
			{
				image->sprite.setColor(sampleColor);
			}
		}
	}

	Image* ImageManager::AddComponent(Entity entity)
	{
		auto& image = GetComponentRef(entity);
		m_ComponentsInfo[entity - 1].image = &image;
		m_ComponentsInfo[entity - 1].SetEntity(entity);
		m_Engine.GetEntityManager()->AddComponentType(entity, ComponentType::IMAGE);
		return &image;
	}

	void ImageManager::DestroyComponent(Entity entity)
	{
		m_Engine.GetEntityManager()->RemoveComponentType(entity, ComponentType::IMAGE);
	}

	void ImageManager::Draw()
	{

	}

	void ImageManager::SetSprite(Entity entity, const std::string& newSpritePath)
	{
		auto component = GetComponentPtr(entity);

		component->sprite.setTexture(component->texture);
	}
}
