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

#include <imgui.h>
#include <imgui-SFML.h>

#include <engine/editor.h>

#include <engine/log.h>

#include <engine/config.h>
#include <engine/scene.h>
#include <engine/transform.h>

#include <graphics/graphics.h>
#include <graphics/sprite.h>

#include <physics/physics.h>

#include <physics/body2d.h>


namespace sfge
{
Editor::Editor(Engine& engine): Module(engine), 
	m_Window(m_Engine.GetGraphicsManager()->GetWindow()), 
	m_SceneManager(m_Engine.GetSceneManager()),
	m_EntityManager(m_Engine.GetEntityManager())
{
}

/**
* \brief Initialize the SceneManager, get the Configuration from Engine and save the Scene lists from it
*/
void Editor::Init()
{
	if (m_Enable)
	{
		Log::GetInstance()->Msg("Enabling Editor");
		if(const auto window = m_Window.lock())
		{
			ImGui::SFML::Init(*window, true);
			m_IsImguiInit = true;
		}
	}
}
void Editor::Update(sf::Time dt)
{
	if (m_Enable)
	{
		if (const auto window = m_Window.lock())
		{
			ImGui::SFML::Update(*window, dt);

			//GameObject window
			ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(150.0f, m_Engine.GetConfig()->screenResolution.y), ImGuiCond_FirstUseEver);
			ImGui::Begin("GameObjects");

			ImGui::End();
			//Component inspector window
			ImGui::SetNextWindowPos(ImVec2(m_Engine.GetConfig()->screenResolution.x - 50.0f, 0), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(150.0f, m_Engine.GetConfig()->screenResolution.y), ImGuiCond_FirstUseEver);
			ImGui::Begin("Inspector");

			ImGui::End();
		}
		
	}
}
/**
* \brief Update the SceneManager, mostly updating the GameObjects of the current Scene and doing the transition when needed
* \param dt Delta time since last frame
*/
void Editor::ProcessEvent(sf::Event& event)
{
	if (m_Enable)
	{
		ImGui::SFML::ProcessEvent(event);
	}
}

void Editor::Draw()
{
	if (m_Enable)
	{
		if (const auto window = m_Window.lock())
		{
			ImGui::SFML::Render(*window);
		}
	}
}



/**
* \brief Finalize and delete everything created in the SceneManager
*/
void Editor::Destroy()
{
	if (m_Enable)
	{
		ImGui::Shutdown();
	}
}

void Editor::Collect()
{
}

void Editor::Clear()
{
}

}
