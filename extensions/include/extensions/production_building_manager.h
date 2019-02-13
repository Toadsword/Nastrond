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

#include <extensions/building_utilities.h>

namespace sfge::ext
{
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

		/**
		 * \brief Spawn a building entity at the given position.
		 * \param position : the location of spawn wanted.
		 */
		void AddNewBuilding(Vec2f position, BuildingType buildingType);

		/**
		 * \brief Destroy the mine at the given index.
		 * \param entity : an entity that is a building.
		 * \return true if the Entity exist and is a building.
		 */
		bool DestroyBuilding(Entity entity, BuildingType buildingType);

		/**
		 * \brief attribute a dwarf to the given building entity.
		 * \param entity : an entity that is a building.
		 * \return true if the entity exist and is a building and if a dwarf has been added to the building.
		 */
		bool AddDwarfToBuilding(Entity entity, BuildingType buildingType);

		/**
		 * \brief remove the attribution of a dwarf to the given building entity.
		 * \param entity : an entity that is a building.
		 * \return true if the entity exist and is a building and if a dwarf has been added to the building.
		 */
		bool RemoveDwarfToBuilding(Entity entity);

		/**
		 * \brief Increment the dwarf slot to tell that a dwarf go in.
		 * \param entity : The Entity of a building that the dwarf want to go in.
		 */
		void DwarfEnterBuilding(Entity entity);

		/**
		 * \brief decrement the dwarf slot to tell that a dwarf go out.
		 * \param entity : The Entity of a building that the dwarf want to go out.
		 */
		void DwarfExitBuilding(Entity entity);

		/**
		 * \brief Get a building with a slot free for a dwarf.
		 * \return An entity of a mine.
		 */
		Entity GetFreeSlotInBuilding(BuildingType buildingType);

		/**
		 * \brief get a building that have resources ready to be taken.
		 * \return a mine entity with resources.
		 */
		Entity GetBuildingWithResources(BuildingType buildingType);

		/**
		 * \brief get the type of resources that a building produce.
		 * \return a resource type.
		 */
		ResourceType GetProducedResourceType(BuildingType buildingType);

		/**
		 * \brief get a certain amount of resources that a building have produce.
		 * \param entity : an entity that is a building.
		 * \return an int of an amount of resources.
		 */
		int TakeResources(Entity entity);

	private:
		/**
		 * \brief Produce iron.
		 */
		void Produce();

		/**
		 * \brief Resize all vector in one go to keep the synchronize all index.
		 * \param newSize : the size with the new number of building.
		 */
		void ResizeContainer(size_t newSize);


		/**
		 * \brief Resize the vertex array for the good type of building.
		 * \param newSize : the new size of the vertex array.
		 * \param buildingType : the type of building of the vertex array.
		 */
		void ResizeContainerForVertexArray(size_t newSize, BuildingType buildingType);

		/**
		 * \brief check if a slot already setup is empty and fill it.
		 * \param newEntity : the entity that is newly created.
		 * \param buildingType  the type of building that you are creating.
		 * \param transformPtr : the transform of the newly created.
		 * \return true if a slot was empty and has been fill.
		 */
		bool CheckEmptySlot(Entity newEntity, BuildingType buildingType, Transform2d* transformPtr);

		/**
		 * \brief Attribute a vertexArray ofr the entity.
		 * \param entityIndex : the index of the entity.
		 * \param buildingType : the type of building of the entity.
		 * \param transformPtr : the transform of the entity.
		 */
		void AttributionVertxArray(unsigned int entityIndex, BuildingType buildingType, Transform2d * transformPtr);

		/**
		 * \brief Setup The Vertex array for the sprite of the entity.
		 * \param newEntity : the index of the entity.
		 * \param vertexArrayPtr : the pointer of the vertex array of the needed building type.
		 * \param texture : the texture of the needed building.
		 * \param transformPtr : the transform of the entity.
		 */
		void SetupVertexArray(int newEntity, sf::VertexArray* vertexArrayPtr, sf::Texture* texture, Transform2d* transformPtr);

		bool m_Init = false;

		Transform2dManager* m_Transform2DManager;
		TextureManager* m_TextureManager;

		sf::RenderWindow* window;

		const unsigned short m_MaxCapacity = 200;
		const unsigned short m_StackSize = 10;
		const unsigned short m_CoolDown = 600;

		unsigned int m_BuildingIndexCount = 0;

		std::vector<Entity> m_EntityIndex;
		std::vector<DwarfSlots> m_DwarfSlots;

		std::vector<unsigned short> m_ResourcesInventories;
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

		//Mine texture
		std::string m_MineTexturePath;
		TextureId m_MineTextureId;
		sf::Texture* m_MineTexture;

		sf::VertexArray m_MineVertexArray;

		//Excavation post texture
		std::string m_ExcavationPostTexturePath;
		TextureId m_ExcavationPostTextureId;
		sf::Texture* m_ExcavationPostTexture;

		sf::VertexArray m_ExcavationPostVertexArray;

		//Mushroom farm texture
		std::string m_MushroomFarmTexturePath;
		TextureId m_MushroomFarmTextureId;
		sf::Texture* m_MushroomFarmTexture;

		sf::VertexArray m_MushroomFarmVertexArray;
	};
}
#endif