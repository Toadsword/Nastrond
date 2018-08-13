//
// Created by efarhan on 09.08.18.
//

#include <engine/system.h>

namespace sfge
{

void System::Destroy()
{
	Clear();
	Collect();
}


void System::SetEnable(bool enable)
{
	m_Enable = enable;
}

bool System::GetEnable() const
{
	return m_Enable;
}

}