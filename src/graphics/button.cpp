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

#include <graphics/button.h>

namespace sfge
{
	void ButtonInfo::DrawOnInspector()
	{

	}

	void ButtonManager::CreateComponent(json& componentJson, Entity entity)
	{
		auto* button = AddComponent(entity);

		sf::Color sampleColor = sampleColor.Black;

		if (CheckJsonExists(componentJson, "sprite"))
		{
			if (button->texture.loadFromFile(componentJson["sprite"]))
				button->sprite.setTexture(button->texture);

			if (CheckJsonExists(componentJson, "color"))
			{
				/*
				* Color is an array in the json :
				* "color": [r,g,b,a]
				*/
				button->color.r = componentJson["color"][0];
				button->color.g = componentJson["color"][1];
				button->color.b = componentJson["color"][2];
				button->color.a = componentJson["color"][3];

				button->sprite.setColor(button->color);
			}
			else
			{
				button->sprite.setColor(sampleColor);
			}
		}
		if (CheckJsonExists(componentJson, "spriteHovered"))
		{
			if (button->textureHovered.loadFromFile(componentJson["spriteHovered"]))
				button->spriteHovered.setTexture(button->textureHovered);

			if (CheckJsonExists(componentJson, "colorHovered"))
			{
				button->colorHovered.r = componentJson["colorHovered"][0];
				button->colorHovered.g = componentJson["colorHovered"][1];
				button->colorHovered.b = componentJson["colorHovered"][2];
				button->colorHovered.a = componentJson["colorHovered"][3];

				button->spriteHovered.setColor(button->colorHovered);
			}
			else
			{
				button->spriteHovered.setColor(sampleColor);
			}
		}
		if (CheckJsonExists(componentJson, "spriteClicked"))
		{
			if (button->textureClicked.loadFromFile(componentJson["spriteClicked"]))
				button->spriteClicked.setTexture(button->textureClicked);

			if (CheckJsonExists(componentJson, "colorClicked"))
			{
				button->colorClicked.r = componentJson["colorClicked"][0];
				button->colorClicked.g = componentJson["colorClicked"][1];
				button->colorClicked.b = componentJson["colorClicked"][2];
				button->colorClicked.a = componentJson["colorClicked"][3];

				button->spriteClicked.setColor(button->colorClicked);
			}
			else
			{
				button->spriteClicked.setColor(sampleColor);
			}
		}

		/*
		* TODO:
		* Assign an action to the button (both in python and C++)
		* Python only need the module and the function name (as string) to be done with :
		*		getattr(module, 'function')
		*		Now : How to launch a function with parameter?
		* C++ need both pointer to the function and the datas :
		*		A dictionary could be made with [key][ptr], but it needs to be done dynamically
		*		as it should be built on initialisation. (-> put int UIManager and add to it when
		*		initializing a button) How to get the ptr of the function?
		*/
	}

	Button* ButtonManager::AddComponent(Entity entity)
	{
		auto& button = GetComponentRef(entity);
		m_ComponentsInfo[entity - 1].button = &button;
		m_ComponentsInfo[entity - 1].SetEntity(entity);
		m_Engine.GetEntityManager()->AddComponentType(entity, ComponentType::BUTTON);
		return &button;
	}

	void ButtonManager::DestroyComponent(Entity entity)
	{
		m_Engine.GetEntityManager()->RemoveComponentType(entity, ComponentType::BUTTON);
	}

	void ButtonManager::SetSprite(Entity entity, const std::string& newSpritePath)
	{
		auto component = GetComponentPtr(entity);

		component->sprite.setTexture(component->texture);
	}

	void ButtonManager::SetAction(Entity entity, const void* function, const void* data)
	{
		const auto component = GetComponentPtr(entity);

		/*
		 * TODO:
		 * Rebuilt the function when it is known how to call a function both in C++ and Python
		 */
		component->action = (void*)function;
		component->data = (void*)data;
	}

	void ButtonManager::CheckMouseOnButton(Entity entity, class Vec2f& mousePosition)
	{

	}

	void ButtonManager::OnClick(Entity entity, void* function(void*), void* data)
	{
		function(data);
	}
}
