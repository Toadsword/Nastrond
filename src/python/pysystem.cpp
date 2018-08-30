//
// Created by efarhan on 21.08.18.
//

#include <sstream>

#include <python/pysystem.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <utility/python_utility.h>
#include "engine/log.h"

namespace sfge
{

void PySystem::Init()
{
	try
	{
		py::gil_scoped_release release;
		PYBIND11_OVERLOAD_NAME(
			void,
			System,
			"init",
			Init,

			);
	}
	catch (std::runtime_error& e)
	{
		std::stringstream oss;
		oss << "Python error on PySystem Init\n" << e.what();
		Log::GetInstance()->Error(oss.str());
	}
}

void PySystem::Update(float dt)
{
	try
	{

		py::gil_scoped_release release;
		PYBIND11_OVERLOAD_NAME(
			void,
			System,
			"update",
			Update,
			dt
			);
	}
	catch (std::runtime_error& e)
	{
		std::stringstream oss;
		oss << "Python error on PySystem Update\n" << e.what();
		Log::GetInstance()->Error(oss.str());
	}
}

void PySystem::FixedUpdate()
{
	try
	{

		py::gil_scoped_release release;
		PYBIND11_OVERLOAD_NAME(
			void,
			System,
			"fixed_update",
			FixedUpdate,
			);
	}
	catch (std::runtime_error& e)
	{
		std::stringstream oss;
		oss << "Python error on PySystem FixedUpdate\n" << e.what();
		Log::GetInstance()->Error(oss.str());
	}
}

void PySystem::Draw()
{
	try
	{

		py::gil_scoped_release release;
		PYBIND11_OVERLOAD_NAME(
			void,
			System,
			"draw",
			Draw,
			);
	}
	catch (std::runtime_error& e)
	{
		std::stringstream oss;
		oss << "Python error on PySystem Draw\n" << e.what();
		Log::GetInstance()->Error(oss.str());
	}
}
}
