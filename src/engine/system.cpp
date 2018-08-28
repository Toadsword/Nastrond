//
// Created by efarhan on 09.08.18.
//

#include <engine/system.h>

namespace sfge
{
System::System(Engine& engine) : m_Engine(engine)
{
}

System::System(const System& system) : 
	m_Engine(system.GetEngine())
{
}

void System::Init()
{
	m_Initialized = true;
}

void System::Destroy()
{
	Clear();
	Collect();
	m_Initialized = false;
}


void System::SetEnable(bool enable)
{
	m_Enable = enable;
}

bool System::GetEnable() const
{
	return m_Enable;
}

Engine& System::GetEngine() const
{
	return m_Engine;
}

bool System::GetInitlialized() const
{
	return m_Initialized;
}
}
