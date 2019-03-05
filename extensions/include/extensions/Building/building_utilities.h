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

	enum RoadCase : unsigned char
	{
		GROUND = 0,
		SOLO = 1,
		END_TOP_LEFT = 3,
		END_TOP_RIGHT = 5,
		END_BOTTOM_LEFT = 9,
		END_BOTTOM_RIGHT = 17,
		TURN_WEST = 11,
		TURN_EAST = 21,
		TURN_NORTH = 7,
		TURN_SOUTH = 25,
		THREE_WAY_NOT_TOP_LEFT = 29,
		THREE_WAY_NOT_TOP_RIGHT = 27,
		THREE_WAY_NOT_BOTTOM_LEFT = 23,
		THREE_WAY_NOT_BOTTOM_RIGHT = 15,
		TOP_LEFT_TO_BOTTOM_RIGHT = 19,
		TOP_RIGHT_TO_BOTTOM_LEFT = 13,
		CROSS = 31
	};

	enum BuildingType : unsigned char
	{
		NO_BUILDING_TYPE,
		MINE,
		EXCAVATION_POST,
		MUSHROOM_FARM,
		FORGE,
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
		const unsigned char maxDwarfCapacity = 5;
		unsigned char dwarfAttributed = 0;
		unsigned char dwarfIn = 0;

		DwarfSlots& operator=(const DwarfSlots &a)
		{
			this->dwarfIn = a.dwarfIn;
			this->dwarfAttributed = a.dwarfAttributed;

			return *this;  // Return a reference to myself.
		}
	};

	const DwarfSlots RESET_DWARF_SLOTS;

	const unsigned int EMPTY_INVENTORY = 0U;
	const unsigned short CONTAINER_RESERVATION = 2000;
	
}
#endif
