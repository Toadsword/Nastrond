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

#include <extensions/Building/building_utilities.h>


namespace sfge::ext
{
	class BuildingManager;

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


		void SpawnBuilding(Vec2f position);

		void DestroyBuilding(Entity dwellingEntity);

		void AttributeDwarfToBuilding(Entity dwellingEntity);

		void DeallocateDwarfToBuilding(Entity dwellingEntity);

		Entity GetBuildingWithFreePlace();

		void DwarfEnterBuilding(Entity dwellingEntity);

		void DwarfExitBuilding(Entity dwellingEntity);

		void DwarfPutsResources(Entity dwellingEntity);

	private:

		void Consume();

		void ReserveContainer(const size_t newSize);

		void AttributeContainer();

		bool CheckFreeSlot(Entity newEntity);

		void SetupTexture(const Entity entity);

		/**
		 * \brief Decrease happiness when call.
		 */
		void DecreaseHappiness();

		EntityManager* m_EntityManager;
		Transform2dManager* m_Transform2DManager;
		TextureManager* m_TextureManager;
		SpriteManager* m_SpriteManager;
		Configuration* m_Configuration;
		BuildingManager* m_BuildingManager;

		bool m_Init = false;

		sf::RenderWindow* m_Window;

		unsigned int m_NmbReservation = 0u;

		const unsigned short m_MaxCapacity = 200;
		const unsigned short m_CoolDownGoal = 600;

		unsigned int m_BuildingIndexCount = 0;

		std::vector<Entity> m_EntityIndex;
		std::vector<DwarfSlots> m_DwarfSlots;

		std::vector<unsigned short> m_ResourcesInventories;
		std::vector<unsigned char> m_ReservedImportStackNumber;

		std::vector<unsigned short> m_ProgressionCoolDown;

		const ResourceType m_ResourceTypeNeeded = ResourceType::FOOD;

		//Building texture
		std::string m_TexturePath;
		TextureId m_TextureId;
		sf::Texture* m_Texture;
	};
}

#endif