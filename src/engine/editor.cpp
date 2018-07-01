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

#include <engine/scene.h>
#include <engine/transform.h>

#include <graphics/graphics.h>
#include <graphics/sprite.h>

#include <physics/physics.h>

#include <physics/body2d.h>


namespace sfge
{
Editor::Editor(Engine& engine, bool enable): Module(engine, enable), 
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
		Log::GetInstance()->Msg("Enabling Editor");
		ImGui::SFML::Init(*m_GraphicsManager.GetWindow(), true);
	}
}
void Editor::Update(sf::Time dt)
{
	if (m_Enable)
	{

		ImGui::SFML::Update(*m_GraphicsManager.GetWindow(), dt);
		static GameObject* selectedGameObject = nullptr;
		//GameObject window
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(150.0f, m_Engine.GetConfig()->screenResolution.y), ImGuiCond_FirstUseEver);
		ImGui::Begin("GameObjects");
		if (m_SceneManager.GetCurrentScene() != nullptr && !m_SceneManager.IsSwitching())
		{  

			static int selected = -1;
			int n = 0;
			
			for (auto gameObject : m_SceneManager.GetCurrentScene()->GetGameObjects())
			{

				if (ImGui::Selectable(gameObject->GetName().c_str(), selected == n))
				{
					selected = n;
					selectedGameObject = gameObject;
				}
				n++;
			}
			
		}
		else
		{
			Log::GetInstance()->Error("No Current Scene for editor");
			selectedGameObject = nullptr;
		}
		ImGui::End();
		//Component inspector window
		ImGui::SetNextWindowPos(ImVec2(m_Engine.GetConfig()->screenResolution.x - 50.0f, 0), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(150.0f, m_Engine.GetConfig()->screenResolution.y), ImGuiCond_FirstUseEver);
		ImGui::Begin("Inspector");
		if (selectedGameObject != nullptr)
		{
			for (auto component : selectedGameObject->GetAllComponents())
			{
				if (ImGui::CollapsingHeader(component->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
				{
					Offsetable* offsetableComponent = dynamic_cast<Offsetable*>(component);
					if (offsetableComponent != nullptr)
					{
						float offset[2] = { offsetableComponent->GetOffset().x, offsetableComponent->GetOffset().y };
						ImGui::InputFloat2("Offset", offset);
					}
					switch (component->GetComponentType())
					{
					case ComponentType::TRANSFORM:
						{
							Transform * transform = dynamic_cast<Transform*>(component);
							if (transform != nullptr)
							{

								float pos[2] = { transform->GetPosition().x, transform->GetPosition().y };
								ImGui::InputFloat2("Position", pos);
							}
						}

						break;
					case ComponentType::SHAPE:
						break;
					case ComponentType::SPRITE:
						
						break;
					case ComponentType::PYCOMPONENT:
						break;
					case ComponentType::BODY2D:
						Body2d * body2d = dynamic_cast<Body2d*>(component);
						if (body2d != nullptr)
						{
							b2Body* b2Body = body2d->GetBody();
							if (b2Body != nullptr)
							{
								sf::Vector2f speed = meter2pixel(b2Body->GetLinearVelocity());
								float speedArray[2] = { speed.x, speed.y };
								ImGui::InputFloat2("Speed", speedArray);
							}
						}

						break;
					}
				}
			}
		}
		else
		{
			ImGui::Text("No selected game object");
		}
		ImGui::End();

		
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
	if(m_Enable)
	{
		ImGui::SFML::Render(*m_GraphicsManager.GetWindow());
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

void Editor::Reset()
{
}

}
