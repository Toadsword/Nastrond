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

#include <engine/system.h>
#include <engine/engine.h>

#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include <extensions/python_extensions.h>
#include <extensions/planet_system.h>
#include <extensions/AI/navigation_graph_manager.h>
#include <extensions/dwarf_manager.h>
#include <extensions/Building/forge_manager.h>
#include <extensions/Building/dwelling_manager.h>
#include <extensions/Building/production_building_manager.h>

#include <tools/tools_pch.h>
#include <extensions/Building/building_manager.h>
#include <extensions/AI/behavior_tree.h>


namespace sfge::ext
{

static std::vector<std::function<void(py::module&)>> m_OtherPythonExtensions;

void ExtendPython(py::module& m)
{
	py::class_<PlanetSystem, System> planetSystem(m, "PlanetSystem");
	planetSystem
		.def(py::init<Engine&>());

	py::class_<DwarfManager, System> dwarfManager(m, "DwarfManager");
	dwarfManager
		.def(py::init<Engine&>());

	py::class_<NavigationGraphManager, System> navigationGraphManager(m, "NavigationGraphManager");
	navigationGraphManager
		.def(py::init<Engine&>());

	py::class_<ProductionBuildingManager, System> productionBuildingManager(m, "ProductionBuildingManager");
	productionBuildingManager
		.def(py::init<Engine&>());

	py::class_<ForgeManager, System> forgeManager(m, "ForgeManager");
	forgeManager
		.def(py::init<Engine&>());

	py::class_<DwellingManager, System> dwellingManager(m, "DwellingManager");
	dwellingManager
		.def(py::init<Engine&>());

	py::class_<WarehouseManager, System> warehouseManager(m, "WarehouseManager");
	warehouseManager
		.def(py::init<Engine&>());

	py::class_<BuildingManager, System> buildingManager(m, "BuildingManager");
	buildingManager
		.def(py::init<Engine&>());

	py::class_<behavior_tree::BehaviorTree, System> behaviourTree(m, "BehaviorTree");
	behaviourTree
		.def(py::init<Engine&>());

	tools::ExtendPythonTools(m);
}

}
