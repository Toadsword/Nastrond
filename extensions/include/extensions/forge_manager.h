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

#ifndef FORGE_MANAGER_H
#define FORGE_MANAGER_H

#include <engine/system.h>
#include <engine/transform2d.h>
#include <graphics/graphics2d.h>

#include <extensions/building_utilities.h>

namespace sfge::ext
{
#define DEBUG_CHECK_PRODUCTION
#define TEST_SYSTEM_DEBUG

	/**
	 * \author Robin Alves
	 */
	class ForgeManager : public System
	{
	public:
		ForgeManager(Engine& engine);

		void Init() override;

		void Update(float dt) override;

		void FixedUpdate() override;

		void Draw() override;

		/**
		 * \brief Method that spawn a new forge at the given position.
		 */
		void SpawnForge(Vec2f pos);

		/**
		 * \brief Method that add dwarf to the dwarf slot struc of the given entity. Return false if there is no place available or if the entity do not exist.
		 */
		bool AddDwarfToForge(Entity mineEntity);

	private:
		/**
		 * \brief Method to resize all vector to keep the same index for mine.
		 */
		void ResizeContainer(const size_t newSize);

		void ProduceTools();

		Transform2dManager* m_Transform2DManager;
		TextureManager* m_TextureManager;
		SpriteManager* m_SpriteManager;

#ifdef TEST_SYSTEM_DEBUG
		const size_t m_entitiesNmb = 500;
#endif

		std::vector<Entity> m_forgeEntityIndex;

		std::vector<DwarfSlots> m_dwarfSlots;
		std::vector<GiverInventory> m_toolsInventories;
		std::vector<RecieverInventory> m_ironsInventories;
		std::vector<ProgressionProduction> m_progressionProdTool;


		const int m_stackSize = 5;

		const int m_FrameBeforAdd = 5;
	};
}
#endif

