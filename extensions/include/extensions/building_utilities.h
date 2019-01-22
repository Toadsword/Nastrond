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
	enum RessourceType
	{
		NONE,
		IRON,
		STONE,
		TOOL,
		FOOD
	};
	struct GiverInventory
	{
		unsigned int maxCapacity = 100u;
		float inventory = 0u;
		unsigned short packNumber = 0u;
		RessourceType ressourceType = RessourceType::NONE;
	};

	struct RecieverInventory
	{
		unsigned int maxCapacity = 100u;
		float inventory = 0u;
		RessourceType ressourceType = RessourceType::NONE;
	};

	struct ProgressionProduction
	{
		float progression = 0.0f;
		int goal = 20;
		int FrameCoolDown = 0;
		RessourceType ressourceType = RessourceType::NONE;
	};

	struct DwarfSlots
	{
		int maxDwarfCapacity = 5;
		int dwarfAttributed = 0;
		int dwarfIn = 0;
	};
}

#endif
