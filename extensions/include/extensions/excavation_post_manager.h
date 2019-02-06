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

#ifndef EXCAVATION_POST_MANAGER_H
#define EXCAVATION_POST_MANAGER_H

#include <engine/system.h>
#include <graphics/graphics2d.h>

#include <extensions/building_utilities.h>

namespace sfge::ext
{

	/**
	 * \author Robin Alves
	 */
	class ExcavationPostManager : public System
	{
	public:
		ExcavationPostManager(Engine& engine);

		void Init() override;

		void Update(float dt) override;

		void FixedUpdate() override;

		void Draw() override;

		/**
		 * \brief spawn an excavation post entity at the given position.
		 * \param position : the location of spawn wanted.
		 */
		void AddNewBuilding(Vec2f position);

		/**
		 * \brief destroy the excavation post at the given index.
		 * \param excavationPostEntity : an entity that is an excavation post.
		 * \return true if the Entity exist.
		 */
		bool DestroyBuilding(Entity excavationPostEntity);

		/**
		 * \brief attribute a dwarf to the given excavation post entity.
		 * \param excavationPostEntity : an entity that is an excavation post.
		 * \return true if the entity exist and is excavation post and if a dwarf has been added to the building.
		 */
		bool AddDwarfToBuilding(Entity excavationPostEntity);

		/**
		 * \brief remove the attribution of a dwarf to the given excavation post entity.
		 * \param excavationPostEntity : an entity that is an excavation post.
		 * \return true if the entity exist and is an excavation post and if a dwarf has been added to the building.
		 */
		bool RemoveDwarfToBuilding(Entity excavationPostEntity);

		/**
		 * \brief Increment the dwarf slot to tell that a dwarf go in.
		 * \param excavationPostEntity : The Entity of the excavation post that the dwarf want to go in.
		 */
		void DwarfEnterBuilding(Entity excavationPostEntity);

		/**
		 * \brief decrement the dwarf slot to tell that a dwarf go out.
		 * \param excavationPostEntity : The Entity of the excavation post that the dwarf want to go out.
		 */
		void DwarfExitBuilding(Entity excavationPostEntity);

		/**
		 * \brief get a excavation post with a slot free for a dwarf.
		 * \return an entity of an Excavation post.
		 */
		Entity GetFreeSlotInBuilding();

		/**
		 * \brief get a forge that have resources ready to be taken.
		 * \return a excavation post entity with resources.
		 */
		Entity GetBuildingWithResources();

		/**
		 * \brief get the type of resources that an excavation post produce.
		 * \return a resource type.
		 */
		ResourceType GetProducedResourceType();

		/**
		 * \brief get a certain amount of resources.
		 * \param excavationPostEntity : an entity that is an excavation post.
		 * \return an int of an amount of resources.
		 */
		int GetResourcesBack(Entity excavationPostEntity);

	private:
		/**
		 * \brief Resize all vector in one go to keep the synchronize all index.
		 * \param newSize : the size with the number of building.
		 */
		void ResizeContainer(const size_t newSize);

		/**
		 * \brief produce stone.
		 */
		void Produce();

		/**
		 * \brief check if a slot already setup is empty and fill it.
		 * \param newEntity : the entity that is newly created.
		 * \param transformPtr : the transform of the newly created.
		 * \return true if a slot was empty and has been fill.
		 */
		bool CheckEmptySlot(Entity newEntity, Transform2d* transformPtr);

		Transform2dManager* m_Transform2DManager;
		TextureManager* m_TextureManager;

		std::vector<Entity> m_EntityIndex;

		std::vector<DwarfSlots> m_DwarfSlots;
		std::vector<GiverInventory> m_StoneInventories;

		float m_ProductionRate = 0.01f;

		unsigned int m_StackSize = 20u;

		ResourceType m_ResourceType = ResourceType::STONE;

		//Building texture
		std::string m_TexturePath;
		TextureId m_TextureId;
		sf::Texture* m_Texture;

		//Vertex array
		sf::VertexArray m_VertexArray;

	};
}

#endif
