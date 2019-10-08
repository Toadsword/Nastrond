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

#ifndef PRODUCTION_BUILDING_MANAGER_H
#define PRODUCTION_BUILDING_MANAGER_H

#include <engine/system.h>
#include <graphics/graphics2d.h>
#include <python/python_engine.h>

#include <extensions/Building/building_utilities.h>

namespace sfge::ext
{
	class BuildingManager;

	/**
	 * \author Robin Alves
	 */
	class ProductionBuildingManager : public System
	{
	public:
		ProductionBuildingManager(Engine& engine);

		void Init() override;

		void Update(float dt) override;

		void FixedUpdate() override;

		void Draw() override;


		void SpawnBuilding(Vec2f position, BuildingType buildingType);

		void DestroyBuilding(Entity entity, BuildingType buildingType);

		void AttributeDwarfToBuilding(Entity entity);

		void DeallocateDwarfToBuilding(Entity entity);

		void DwarfEnterBuilding(Entity entity);

		void DwarfExitBuilding(Entity entity);

		Entity GetBuildingWithFreePlace(BuildingType buildingType);

		int DwarfTakesResources(Entity entity, BuildingType buildingType);

	private:

		void Produce();

		void ResizeContainer(size_t newSize);

		bool CheckFreeSlot(Entity newEntity, BuildingType buildingType);

		void AttributionTexture(Entity newEntity, BuildingType buildingType);

		void SetupTexture(Entity newEntity, sf::Texture* texture);

		bool m_Init = false;

		EntityManager* m_EntityManager;
		Transform2dManager* m_Transform2DManager;
		TextureManager* m_TextureManager;
		SpriteManager* m_SpriteManager;
		BuildingManager* m_BuildingManager;
		Configuration* m_Configuration;

		sf::RenderWindow* m_Window;

		const unsigned short m_MaxCapacity = 200;
		const unsigned short m_CoolDownGoal = 600;

		unsigned int m_BuildingIndexCount = 0;

		std::vector<Entity> m_EntityIndex;
		std::vector<DwarfSlots> m_DwarfSlots;

		std::vector<unsigned short> m_ResourcesInventories;
		std::vector<unsigned char> m_ReservedExportStackNumber;

		std::vector<ResourceType> m_ResourceTypes;
		std::vector<BuildingType> m_BuildingTypes;

		std::vector<unsigned short> m_ProgressionCoolDowns;

		ResourceType m_MineResourceType = ResourceType::IRON;
		ResourceType m_ExcavationPostResourceType = ResourceType::STONE;
		ResourceType m_MushroomFarmResourceType = ResourceType::FOOD;

		std::vector<bool> m_MineVertexIndex;
		std::vector<bool> m_ExcavationPostVertexIndex;
		std::vector<bool> m_MushroomFarmVertexIndex;

		std::vector<unsigned int> m_GeneralVertexIndex;

		unsigned int m_MineCount = 0;
		unsigned int m_ExcavationPostCount = 0;
		unsigned int m_MushroomFarmCount = 0;

#pragma region Graphics section
		//Mine texture
		std::string m_MineTexturePath;
		TextureId m_MineTextureId;
		sf::Texture* m_MineTexture;

		//Excavation post texture
		std::string m_ExcavationPostTexturePath;
		TextureId m_ExcavationPostTextureId;
		sf::Texture* m_ExcavationPostTexture;

		//Mushroom farm texture
		std::string m_MushroomFarmTexturePath;
		TextureId m_MushroomFarmTextureId;
		sf::Texture* m_MushroomFarmTexture;
#pragma endregion
	};
}
#endif