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
#include <engine/globals.h>
#include <engine/engine.h>
#include <engine/log.h>
#include <engine/config.h>
#include <engine/scene.h>
#include <graphics/graphics.h>


namespace sfge
{

Editor::Editor(Engine& engine): Module(engine),
	m_GraphicsManager(m_Engine.GetGraphicsManager()), 
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
		if (auto graphicsManager = m_GraphicsManager.lock())
		{
			m_Window = graphicsManager->GetWindow();
		}
		Log::GetInstance()->Msg("Enabling Editor");
		if(const auto window = m_Window.lock())
		{
			ImGui::SFML::Init(*window, true);
			m_IsImguiInit = true;
		}
		else
		{
			Log::GetInstance()->Msg("Could not enable Editor");
		}
	}
}
void Editor::Update(sf::Time dt)
{
	if (m_Enable)
	{
		const auto windowPtr = m_Window.lock();
		const auto configPtr = m_Engine.GetConfig().lock();
		if (windowPtr and configPtr)
		{
			ImGui::SFML::Update(*windowPtr, dt);

			//GameObject window
			ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(150.0f, configPtr->screenResolution.y), ImGuiCond_FirstUseEver);
			ImGui::Begin("Entities");
			//TODO ADD THE ENTITES ON THE WINDOW
			ImGui::End();
			//Component inspector window
			ImGui::SetNextWindowPos(ImVec2(configPtr->screenResolution.x - 50.0f, 0), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(150.0f, configPtr->screenResolution.y), ImGuiCond_FirstUseEver);
			ImGui::Begin("Inspector");
			//TODO ADD THE SELECTED ENTITY COMPONENTS ON THE WINDOW
			ImGui::End();
		}
		
	}
}
/**
* \brief Update the SceneManager, mostly updating the GameObjects of the current Scene and doing the transition when needed
* \param event SFML event used by ImGUI for mouse click and others
*/
void Editor::ProcessEvent(sf::Event& event) const
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

void Editor::SetCurrentScene(std::unique_ptr<editor::SceneInfo> sceneInfo)
{
	m_CurrentScene = std::move(sceneInfo);
	if(auto window = m_Window.lock())
	{
		std::ostringstream oss;
		oss << "SFGE " << SFGE_VERSION << " - " << m_CurrentScene->name;
		window->setTitle(oss.str());
	}

}
}
