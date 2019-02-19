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

#include <extensions/Building/warehouse_manager.h>

namespace sfge::ext
{
	WarehouseManager::WarehouseManager(Engine& engine) : System(engine){}

	void WarehouseManager::Init()
	{
		std::cout << "Warehouse Manager \n";

	}

	void WarehouseManager::Update(float dt)
	{
	}

	void WarehouseManager::FixedUpdate()
	{
	}

	void WarehouseManager::AddNewBuilding(Vec2f position)
	{
	}

	bool WarehouseManager::DestroyBuilding(Entity warehouseEntity)
	{
		return false;
	}

	bool WarehouseManager::AddDwarfToBuilding(Entity warehouseEntity)
	{
		return false;
	}

	bool WarehouseManager::RemoveDwarfToBuilding(Entity warehouseEntity)
	{
		return false;
	}

	void WarehouseManager::DwarfEnterBuilding(Entity warehouseEntity)
	{
	}

	void WarehouseManager::DwarfExitBuilding(Entity warehouseEntity)
	{
	}

	Entity WarehouseManager::GetFreeSlotInBuilding()
	{
		return Entity();
	}

	Entity WarehouseManager::GetWarehouseWithResources(ResourceType resourceType)
	{
		return Entity();
	}

	Entity WarehouseManager::GetWarehouseWithFreeSpaceAvailable(ResourceType resourceType)
	{
		return Entity();
	}

	void WarehouseManager::DwarfTakeResources(Entity entity, ResourceType resourceType)
	{
	}

	void WarehouseManager::DwarfPutResources(Entity entity, ResourceType resourceType)
	{
	}
	void WarehouseManager::ReserveFill(Entity entity, ResourceType resourceType)
	{
	}
	void WarehouseManager::ReserveEmpty(Entity entity, ResourceType)
	{
	}
}