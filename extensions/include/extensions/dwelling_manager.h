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

#include <extensions/building_utilities.h>


namespace sfge::ext
{

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

		/**
		 * \brief Spawn a dwelling entity at the given position.
		 * \param position : the location of spawn wanted.
		 */
		void AddNewBuilding(Vec2f position);

		/**
		 * \brief Destroy the dwelling at the given index.
		 * \param dwellingEntity : an entity that is a dwelling.
		 * \return true if the Entity exist and is a dwelling.
		 */
		bool DestroyBuilding(Entity dwellingEntity);

		/**
		 * \brief attribute a dwarf to the given dwelling entity.
		 * \param dwellingEntity : an entity that is a dwelling.
		 * \return true if the entity exist and is a dwelling and if a dwarf has been added to the building.
		 */ 
		bool AddDwarfToBuilding(Entity dwellingEntity);

		/**
		 * \brief remove the attribution of a dwarf to the given dwelling entity.
		 * \param dwellingEntity : an entity that is a dwelling.
		 * \return true if the entity exist and is a dwelling and if a dwarf has been added to the building.
		 */
		bool RemoveDwarfToBuilding(Entity dwellingEntity);

		/**
		 * \brief Get a dwelling with a slot free for a dwarf.
		 * \return An entity of a dwelling.
		 */
		Entity GetFreeSlotInBuilding();

		/**
		 * \brief Increment the dwarf slot to tell that a dwarf go in.
		 * \param dwellingEntity : The Entity of the dwelling that the dwarf want to go in.
		 */
		void DwarfEnterBuilding(Entity dwellingEntity);

		/**
		 * \brief Increment the dwarf slot to tell that a dwarf go out.
		 * \param dwellingEntity : The Entity of the dwelling that the dwarf want to go out.
		 */
		void DwarfExitBuilding(Entity dwellingEntity);

		/**
		 * \brief Get all resources that the building need to do his work.
		 * \return a vector with all the type of resources that the dwelling need.
		 */
		std::vector<ResourceType> GetNeededResourceType();

		/**
		 * \brief give resources that the dwelling need.
		 * \param dwellingEntity : an entity that is a dwelling.
		 * \param nmbResources : the number of resources that needed to be deposit.
		 * \param resourceType : the type of resources that want to be deposit.
		 * \return the rest of resources if all the resources can't be taken or if the type doesn't match.
		 */
		float GiveResources(Entity dwellingEntity, int nmbResources, ResourceType resourceType);

	private:

		/**
		 * \brief Consume Resources depending on the number of dwarf in.
		 */
		void Consume();

		/**
		 * \brief Resize all vector in one go to keep the synchronize all index.
		 * \param newSize : the size with the new number of building.
		 */
		void ResizeContainer(const size_t newSize);

		/**
		 * \brief return true if a slot in the index is empty then take this slot.
		 */
		bool CheckEmptySlot(Entity newEntity, Transform2d* transformPtr);

		/**
		 * \brief Decrease happiness when call.
		 */
		void DecreaseHappiness();

		Transform2dManager* m_Transform2DManager;
		TextureManager* m_TextureManager;
		SpriteManager* m_SpriteManager;

		std::vector<Entity> m_EntityIndex;


		std::vector<DwarfSlots> m_DwarfSlots;
		std::vector<ReceiverInventory> m_FoodInventories;
		std::vector<unsigned int> m_CoolDownFramesProgression;

		const ResourceType m_ResourceTypeNeeded = ResourceType::FOOD;
		const unsigned int m_CoolDownFrames = 2000;

		//Building texture
		std::string m_TexturePath;
		TextureId m_TextureId;
		sf::Texture* m_Texture;

		//Vertex array
		sf::VertexArray m_VertexArray;
	};
}

#endif