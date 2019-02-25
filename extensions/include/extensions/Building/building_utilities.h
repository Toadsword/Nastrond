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

#ifndef BUILDING_UTILITIES_H
#define BUILDING_UTILITIES_H

namespace sfge::ext
{
	/**
	 * \author Robin Alves
	 */
	enum ResourceType : unsigned char
	{
		NO_RESOURCE_TYPE,
		IRON,
		STONE,
		TOOL,
		FOOD
	};

	static const struct StackSize
	{
		static const unsigned short rawResources = 20;
		static const unsigned short transformedResources = 5;
	};

	static unsigned short GetStackSizeByResourceType(ResourceType resourceType)
	{
		switch(resourceType)
		{
		case ResourceType::IRON :
			return StackSize::rawResources;
		case ResourceType::STONE:
			return StackSize::rawResources;
		case ResourceType::FOOD:
			return StackSize::rawResources;
		case ResourceType::TOOL:
			return StackSize::rawResources;
		}
		return NO_RESOURCE_TYPE;
	}

	/**
	 * \brief Struct that represent an inventory that must be empty.
	 * \author Robin Alves
	 */
	struct GiverInventory
	{
		unsigned int maxCapacity = 100u;
		unsigned int packNumber = 0u;
		unsigned int packSize = 0u;
		float inventory = 0u;
		ResourceType resourceType = ResourceType::NO_RESOURCE_TYPE;
	};

	/**
	 * \brief Represent an inventory that must be fill.
	 * \author Robin Alves
	 */
	struct ReceiverInventory
	{
		unsigned int maxCapacity = 100u;
		float inventory = 0u;
		ResourceType resourceType = ResourceType::NO_RESOURCE_TYPE;
	};

	/**
	 * \brief Represent an inventory that must be fill.
	 * \author Robin Alves
	 */
	struct Inventory
	{
		unsigned int maxCapacity = 100u;
		float inventory = 0u;
		ResourceType resourceType = ResourceType::NO_RESOURCE_TYPE;
	};


	/**
	 * \brief Represent the progression of the production by the consumption of one or more receiver inventory to a giver inventory.
	 * \author Robin Alves
	 */
	struct ProgressionProduction
	{
		float progression = 0.0f;
		int goal = 20;
		int FrameCoolDown = 0;
		ResourceType resourceType = ResourceType::NO_RESOURCE_TYPE;
	};

	enum BuildingType : unsigned char
	{
		NO_BUILDING_TYPE,
		FORGE,
		MINE,
		EXCAVATION_POST,
		MUSHROOM_FARM,
		WAREHOUSE,
		DWELLING, //Warning : Dwelling are not consider as working place
		LENGTH
	};

	/**
	 * \brief Represent the number of dwarf attribute to a building, in a building and the maximum of them allow to be attribute.
	 * \author Robin Alves
	 */
	struct DwarfSlots
	{
		int maxDwarfCapacity = 5;
		int dwarfAttributed = 0;
		int dwarfIn = 0;
	};

	const DwarfSlots RESET_DWARF_SLOTS;

	const unsigned int EMPTY_INVENTORY = 0U;
	const unsigned short CONTAINER_RESERVATION = 2000;
	
}
#endif
