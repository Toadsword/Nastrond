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

#include <extensions/Building/building_utilities.h>


namespace sfge::ext
{

	class BuildingManager;
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

		
		void SpawnBuilding(Vec2f position);

		void DestroyBuilding(Entity forgeEntity);

		void AttributeDwarfToBuilding(Entity forgeEntity);

		void DeallocateDwarfToBuilding(Entity forgeEntity);

		void DwarfEnterBuilding(Entity forgeEntity);

		void DwarfExitBuilding(Entity forgeEntity);

		Entity GetBuildingWithFreePlace();

		void DwarfTakesResources(Entity forgeEntity);

		void DwarfPutsResources(Entity forgeEntity);

	private:

		void ProduceTools();

		void ReserveContainer(const size_t newSize);

		bool CheckFreeSlot(Entity newEntity);

		void AttributeContainer();

		void SetupTexture(const Entity entity);



		EntityManager* m_EntityManager;
		Transform2dManager* m_Transform2DManager;
		TextureManager* m_TextureManager;
		SpriteManager* m_SpriteManager;
		Configuration* m_Configuration;
		BuildingManager* m_BuildingManager;

		bool m_Init = false;

		sf::RenderWindow* m_Window;

		unsigned int m_NmbReservation = 0u;

		unsigned short m_CoolDownGoal = 100;
		unsigned short m_ConsumptionGoal = 20;

		unsigned short m_MaxCapacityGiver = 200;
		unsigned short m_MaxCapacityReceiver = 100;

		unsigned int m_BuildingIndexCount = 0;

		std::vector<Entity> m_EntityIndex;

		std::vector<DwarfSlots> m_DwarfSlots;

		std::vector<unsigned short> m_ResourcesInventoriesGiver;
		std::vector<unsigned short> m_ResourcesInventoriesReceiver;

		std::vector<unsigned char> m_ProgressionConsumption;
		std::vector<unsigned short> m_ProgressionCoolDown;


		std::vector<unsigned char> m_ReservedExportStackNumber;
		std::vector<unsigned char> m_ReservedImportStackNumber;

		const ResourceType m_ResourceTypeNeeded = ResourceType::IRON;
		const ResourceType m_ResourceTypeProduced = ResourceType::TOOL;

		//Forge texture
		std::string m_TexturePath;
		TextureId m_TextureId;
		sf::Texture* m_Texture;
	};
}
#endif

