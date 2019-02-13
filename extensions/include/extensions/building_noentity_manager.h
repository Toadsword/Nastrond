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

#ifndef BUILDING_NOENTITY_MANAGER_H
#define BUILDING_NOENTITY_MANAGER_H

#include <engine/system.h>
#include <graphics/graphics2d.h>

#include <extensions/building_utilities.h>


namespace sfge::ext
{
	/**
	 * \author Robin Alves
	 */
	class BuildingNoEntityManager : public System
	{
	public:
		BuildingNoEntityManager(Engine& engine);

		void Init() override;

		void Update(float dt) override;

		void FixedUpdate() override;

		void Draw() override;

		/**
		 * \brief Spawn a dwelling entity at the given position.
		 * \param position : the location of spawn wanted.
		 */
		void AddNewBuilding(Vec2f position);

	private:

		/**
		 * \brief Consume Resources depending on the number of dwarf in.
		 */
		void Consume();

		/**
		 * \brief Resize all vector in one go to keep the synchronize all index.
		 * \param newSize : the size with the new number of building.
		 */
		void ResizeContainer(const size_t newSize);

		/**
		 * \brief Decrease happiness when call.
		 */
		void DecreaseHappiness();

		TextureManager* m_TextureManager;

		std::vector<Entity> m_EntityIndex;

		//Building texture
		std::string m_TexturePath;
		TextureId m_TextureId;
		sf::Texture* m_Texture;

		//Vertex array
		sf::VertexArray m_VertexArray;
	};
}
#endif