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


#include <extensions/building_manager.h>


namespace sfge::ext
{
	BuildingManager::BuildingManager(Engine & engine) : System(engine) {}

	void BuildingManager::Init()
	{

		m_DwellingManager = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<DwellingManager>(
			"DwellingManager");
		m_ForgeManager = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<ForgeManager>(
			"ForgeManager");
		m_MineManager = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<MineManager>(
			"MineManager");
	}

	void BuildingManager::Update(float dt){}

	void BuildingManager::FixedUpdate(){}

	void BuildingManager::Draw(){}

	Entity BuildingManager::AttributeDwarfToWorkingPlace(BuildingType buildingType)
	{
		//std::vector<BuildingType> buildingTypes;
		//for (int i = 0; i < BuildingType::LENGTH - 1; i++)
		//{
		//	if (buildingType == static_cast<BuildingType>(i))
		//	{
		//		buildingTypes.push_back(static_cast<BuildingType>(i));
		//	}
		//}

		//for (int i = 0; i < BuildingType::LENGTH - 1; i++)
		//{
		//	if (buildingType == static_cast<BuildingType>(i))
		//		continue;

		//	buildingTypes.push_back(static_cast<BuildingType>(i));
		//}

		//for (BuildingType type : buildingTypes)
		//{
		switch (/*type*/ buildingType)
		{
		case BuildingType::FORGE :
			Entity forgeEntity = INVALID_ENTITY;
			forgeEntity = m_ForgeManager->GetFreeSlotInBuilding();

			if(forgeEntity == INVALID_ENTITY)
				return INVALID_ENTITY;

			m_ForgeManager->AddDwarfToBuilding(forgeEntity);
			return forgeEntity;

		case BuildingType::MINE:
			Entity mineEntity = INVALID_ENTITY;
			mineEntity = m_MineManager->GetFreeSlotInBuilding();

			if (mineEntity == INVALID_ENTITY)
				return INVALID_ENTITY;

			m_MineManager->AddDwarfToBuilding(mineEntity);
			return mineEntity;

		case BuildingType::EXCAVATION_POST:
			break;
		case BuildingType::MUSHROOM_FARM:
			break;
		case BuildingType::WAREHOUSE:
			break;
		}
		//}
		return INVALID_ENTITY;
	}


}
