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

#ifndef BUILDING_MANAGER_H
#define BUILDING_MANAGER_H

#include <engine/system.h>
#include <python/python_engine.h>

#include <extensions/dwelling_manager.h>
#include <extensions/forge_manager.h>
#include <extensions/mine_manager.h>
#include <extensions/excavation_post_manager.h>
#include <extensions/mushroom_farm_manager.h>
#include <extensions/warehouse_manager.h>


namespace sfge::ext
{
	/**
	 * \author Robin Alves
	 */
	class BuildingManager : public System
	{
	public:
		BuildingManager(Engine& engine);

		void Init() override;

		void Update(float dt) override;

		void FixedUpdate() override;

		void Draw() override;


		enum BuildingType
		{
			FORGE,
			MINE,
			EXCAVATION_POST,
			MUSHROOM_FARM,
			WAREHOUSE,
			DWELLING, //Warning : Dwelling are not consider as working place
			LENGTH
		};

		void DwarfEnterBuilding(BuildingType buildingType, Entity entity);

		void DwarfExitBuilding(BuildingType buildingType, Entity entity);

		Entity GetDwellingFreeSlots();

		void AddDwarfToBuilding(Entity dwellingEntity);

		Entity AttributeDwarfToWorkingPlace(BuildingType buildingType);

	private:
		DwellingManager* m_DwellingManager;
		ForgeManager* m_ForgeManager;
		MineManager* m_MineManager;
		ExcavationPostManager* m_ExcavationPostManager;
		MushroomFarmManager* m_MushroomFarmManager;
		WarehouseManager* m_WarehouseManager;

	};
}
#endif