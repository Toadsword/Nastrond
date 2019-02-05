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

#ifndef MUSHROOM_FARM_MANAGER_H
#define MUSHROOM_FARM_MANAGER_H

#include <engine/system.h>
#include <graphics/graphics2d.h>

#include <extensions/building_utilities.h>

namespace sfge::ext
{
	/**
	 * \author Robin Alves
	 */
	class MushroomFarmManager : public System
	{
	public:
		MushroomFarmManager(Engine& engine);

		void Init() override;

		void Update(float dt) override;

		void FixedUpdate() override;

		/**
		 * \brief Spawn a mushroom farm entity at the given position.
		 * \param position : the location of spawn wanted.
		 */
		void AddNewBuilding(Vec2f position);

		/**
		 * \brief Destroy the mushroom farm at the given index.
		 * \param mushroomFarmEntity : an entity that is a mushroom farm.
		 * \return true if the Entity exist and is a mushroom farm.
		 */
		bool DestroyBuilding(Entity mushroomFarmEntity);

		/**
		 * \brief attribute a dwarf to the given mushroom farm entity.
		 * \param mushroomFarmEntity : an entity that is a mushroom farm.
		 * \return true if the entity exist and is a mushroom farm and if a dwarf has been added to the building.
		 */
		bool AddDwarfToBuilding(Entity mushroomFarmEntity);

		/**
		 * \brief remove the attribution of a dwarf to the given mushroom farm entity.
		 * \param mushroomFarmEntity : an entity that is a mushroom farm.
		 * \return true if the entity exist and is a mushroom farm and if a dwarf has been added to the building.
		 */
		bool RemoveDwarfToBuilding(Entity mushroomFarmEntity);

		/**
		 * \brief Increment the dwarf slot to tell that a dwarf go in.
		 * \param mushroomFarmEntity : The Entity of the mushroom farm that the dwarf want to go in.
		 */
		void DwarfEnterBuilding(Entity mushroomFarmEntity);

		/**
		 * \brief decrement the dwarf slot to tell that a dwarf go out.
		 * \param mushroomFarmEntity : The Entity of the mushroom farm that the dwarf want to go out.
		 */
		void DwarfExitBuilding(Entity mushroomFarmEntity);

		/**
		 * \brief Get a mushroom farm with a slot free for a dwarf.
		 * \return An entity of a mushroom farm.
		 */
		Entity GetFreeSlotInBuilding();

	private:
		Transform2dManager* m_Transform2DManager;
		TextureManager* m_TextureManager;
		SpriteManager* m_SpriteManager;

		std::vector<Entity> m_Entityindex;

		std::vector<DwarfSlots> m_DwarfSlots;

	};
}
#endif