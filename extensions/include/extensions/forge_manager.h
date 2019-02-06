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
		 * \brief Spawn a forge entity at the given position.
		 * \param position : the location of spawn wanted.
		 */
		void AddNewBuilding(Vec2f position);

		/**
		 * \brief Destroy the forge at the given index.
		 * \param forgeEntity : an entity that is a forge.
		 * \return true if the Entity exist and is a forge.
		 */
		bool DestroyBuilding(Entity forgeEntity);

		/**
		 * \brief attribute a dwarf to the given forge entity.
		 * \param forgeEntity : an entity that is a forge.
		 * \return true if the entity exist and is a forge and if a dwarf has been added to the building.
		 */
		bool AddDwarfToBuilding(Entity forgeEntity);

		/**
		 * \brief remove the attribution of a dwarf to the given forge entity.
		 * \param forgeEntity : an entity that is a forge.
		 * \return true if the entity exist and is a forge and if a dwarf has been added to the building.
		 */
		bool RemoveDwarfToBuilding(Entity forgeEntity);

		/**
		 * \brief Increment the dwarf slot to tell that a dwarf go in.
		 * \param forgeEntity : The Entity of the forge that the dwarf want to go in.
		 */
		void DwarfEnterBuilding(Entity forgeEntity);

		/**
		 * \brief decrement the dwarf slot to tell that a dwarf go out.
		 * \param forgeEntity : The Entity of the forge that the dwarf want to go out.
		 */
		void DwarfExitBuilding(Entity forgeEntity);

		/**
		 * \brief Get a forge with a slot free for a dwarf.
		 * \return An entity of a forge.
		 */
		Entity GetFreeSlotInBuilding();

		/**
		 * \brief get a forge that have resources ready to be taken.
		 * \return a forge entity with resources.
		 */
		Entity GetBuildingWithResources();

		/**
		 * \brief get the type of resources that a forge produce.
		 * \return a resource type.
		 */
		ResourceType GetProducedResourceType();

		/**
		 * \brief Get all resources that the building need to do his work.
		 * \return a vector with all the type of resources that the forge need.
		 */
		std::vector<ResourceType> GetNeededResourceType();

		/**
		 * \brief get a certain amount of resources that a forge have produce.
		 * \param forgeEntity : an entity that is a forge.
		 * \return an int of an amount of resources.
		 */
		int GetResourcesBack(Entity forgeEntity);

		/**
		 * \brief give resources that the forge need to produce.
		 * \param forgeEntity : an entity that is a forge.
		 * \param nmbResources : the number of resources that needed to be deposit.
		 * \param resourceType : the type of resources that want to be deposit.
		 * \return the rest of resources if all the resources can't be taken or if the type doesn't match.
		 */
		float GiveResources(Entity forgeEntity, int nmbResources, ResourceType resourceType);

	private:
		/**
		 * \brief Consume iron to produce tool.
		 */
		void ProduceTools();

		/**
		 * \brief Resize all vector in one go to keep the synchronize all index.
		 * \param newSize : the size with the new number of building.
		 */
		void ResizeContainer(const size_t newSize);


		/**
		 * \brief check if a slot already setup is empty and fill it.
		 * \param newEntity : the entity that is newly created.
		 * \param transformPtr : the transform of the newly created.
		 * \return true if a slot was empty and has been fill.
		 */
		bool CheckEmptySlot(Entity newEntity, Transform2d* transformPtr);

		Transform2dManager* m_Transform2DManager;
		TextureManager* m_TextureManager;
		SpriteManager* m_SpriteManager;

		std::vector<Entity> m_EntityIndex;

		std::vector<DwarfSlots> m_DwarfSlots;
		std::vector<GiverInventory> m_ToolsInventories;
		std::vector<ReceiverInventory> m_IronsInventories;
		std::vector<ProgressionProduction> m_ProgressionProdTool;


		const int m_StackSize = 5;

		const int m_CoolDownFrames = 20;

		ResourceType m_ResourceTypeNeeded = ResourceType::IRON;
		ResourceType m_ResourceTypeProduced = ResourceType::TOOL;


		//Building texture
		std::string m_TexturePath;
		TextureId m_TextureId;
		sf::Texture* m_Texture;

		//Vertex array
		sf::VertexArray m_VertexArray;
	};
}
#endif

