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

#ifndef MINE_MANAGER_H
#define MINE_MANAGER_H

#include <iostream>

#include <engine/system.h>
#include <graphics/graphics2d.h>

#include "extensions/building_utilities.h"

namespace sfge::ext
{
#define DEBUG_CHECK_PRODUCTION
#define SYSTEM_TEST_DEBUG
	/**
	 * \author Robin Alves
	 */
	class MineManager : public System
	{
	public:
		MineManager(Engine& engine);

		void Init() override;

		void Update(float dt) override;

		void FixedUpdate() override;

		void Draw() override;

		/**
		 * \brief Method that create a new mine est setup it at the given position.
		 */
		void AddNewMine(Vec2f pos);

		bool AddDwarfToMine(Entity mineEntity);

	private:
		/**
		 * \brief Method to produce ressources.
		 */
		void RessourcesProduction();

		/**
		 * \brief Method to resize all vector to keep the same index for mine.
		 */
		void ResizeContainer(size_t newSize);

		/**
		 * \brief Method that ping the Task Manager when a stack of iron is avalaible.
		 */
		void IronStackAvalaible(Entity entity);

		Transform2dManager* m_Transform2DManager;
		TextureManager* m_TextureManager;
		SpriteManager* m_SpriteManager;

		const size_t m_entitiesNmb = 10;

		std::vector<Entity> m_mineEntityIndex;

		std::vector<DwarfSlots> m_dwarfSlots;
		std::vector<GiverInventory> m_IronProduction;

		float m_ProductionRate = 0.01f;
		unsigned int m_packSize = 20u;

		int m_TmpDwarfNumber = 5;
	};
}
#endif
