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

#ifndef DWELLING_MANAGER_H
#define DWELLING_MANAGER_H

#include <engine/system.h>
#include <graphics/graphics2d.h>

#include <extensions/building_utilities.h>


namespace sfge::ext
{
#define TEST_SYSTEM_DEBUG

	/**
	 * \author Robin Alves
	 */
	class DwellingManager : public System
	{
	public:
		DwellingManager(Engine& engine);

		void Init() override;

		void Update(float dt) override;

		void FixedUpdate() override;

		void Draw() override;

		/**
		 * \brief Spawn a new dwelling at the given position.
		 */
		void AddNewBuilding(Vec2f pos);

		/**
		 * \brief Destroy the dwelling at the given index. Return if the entity is not available.
		 */
		bool DestroyBuilding(Entity mineEntity);

		/**
		 * \brief Add dwarf to the dwarf slot struct of the given entity. Return false if there is no slot available or if the entity do not exist.
		 */
		bool AddDwarfToBuilding(Entity dwellingEntity);

		/**
		 * \brief Remove dwarf to the dwarf slot struct of the given building. Return false if there is no slot available or if the entity do not exist.
		 */
		bool RemoveDwarfToBuilding(Entity mineEntity);

		/**
		 * \brief Return a dwelling entity with a slot available for a dwarf. If not
		 */
		Entity GetFreeSlotInBuilding();

	private:

		/**
		 * \brief Consume Resources depending on the number of dwarf in.
		 */
		void ConsumeResources();

		/**
		 * \brief Resize all vector to keep the same index for mine.
		 */
		void ResizeContainer(const size_t newSize);

		/**
		 * \brief return true if a slot in the index is empty then take this slot.
		 */
		bool CheckEmptySlot(Entity newEntity, Transform2d* transformPtr);

		/**
		 * \brief Decrease happiness when call.
		 */
		void DecreaseHappiness();

		Transform2dManager* m_Transform2DManager;
		TextureManager* m_TextureManager;
		SpriteManager* m_SpriteManager;

		std::vector<Entity> m_EntityIndex;


		std::vector<DwarfSlots> m_DwarfSlots;
		std::vector<ReceiverInventory> m_FoodInventories;
		std::vector<unsigned int> m_CoolDownFramesProgression;

#ifdef TEST_SYSTEM_DEBUG
		const size_t m_EntitiesNmb = 10;
		size_t m_EntitiesCount = 0;

		const unsigned int m_FramesBeforeAdd = 0u;
		unsigned int m_FrameInProgress = 0u;
#endif

		const unsigned int m_CoolDownFrames = 2000;

		//Building texture
		std::string m_TexturePath;
		TextureId m_TextureId;
		sf::Texture* m_Texture;

		//Vertex array
		sf::VertexArray m_VertexArray;
	};
}

#endif