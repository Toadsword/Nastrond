//
// Created by efarhan on 09.08.18.
//

#include <engine/system.h>

namespace sfge
{
    System::System ()
        : m_ThreadPool (Engine::GetInstance ()->GetThreadPool ())
    {

    }

}