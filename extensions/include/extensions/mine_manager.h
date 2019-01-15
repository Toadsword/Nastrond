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

#include "engine/system.h"
#include "graphics/graphics2d.h"

namespace sfge::ext
{
#define DEBUG_CHECK_PRODUCTION

	struct GiverInventory
	{
		unsigned const int MAX_CAPACITY = 100u;
		float inventory = 0u;
		unsigned short packNumber = 0u;
	};

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
		 * \brief Methode that ping the Task Manager when a stack of iron is avalaible.
		 */
		void IronStackAvalaible(Entity entity);

	private:
		/**
		 * \brief Methode to produceRessources
		 */
		void RessourcesProduction();


		/**
		 * \brief Methode that simulate the shiffting of the iron.
		 */
		void RessourcesShifftingSimulation(int EntityIndex);

		Transform2dManager* m_Transform2DManager;
		TextureManager* m_TextureManager;
		SpriteManager* m_SpriteManager;

		const size_t m_entitiesNmb = 10;

		std::vector<GiverInventory> m_IronProduction{ m_entitiesNmb };

		float m_ProductionRate = 0.01f;

		unsigned int m_packSize = 20u;

		int m_TmpDwarfNumber = 5;
	};
}
#endif
