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

#include <graphics/button.h>
#include <imgui.h>
#include <engine/ui.h>

namespace sfge
{
	void editor::ButtonInfo::DrawOnInspector()
	{
		ImGui::Separator();
		ImGui::Text("Button");
	}

	Button::Button() { }

	Button::~Button() { }

	void Button::Init() { }

	ButtonManager::ButtonManager(Engine& engine):SingleComponentManager(engine)
	{
		m_RectTransformManager = m_Engine.GetRectTransformManager();
		m_MouseManager = &m_Engine.GetInputManager()->GetMouseManager();
	}

	ButtonManager::~ButtonManager()
	= default;


	void ButtonManager::CreateComponent(json& componentJson, Entity entity) { }

	Button* ButtonManager::AddComponent(Entity entity)
	{
		auto& button = GetComponentRef(entity);
		m_ComponentsInfo[entity - 1].button = &button;
		m_ComponentsInfo[entity - 1].SetEntity(entity);
		m_Engine.GetEntityManager()->AddComponentType(entity, ComponentType::BUTTON);
		return &button;
	}

	void ButtonManager::DestroyComponent(Entity entity)
	{
		m_Engine.GetEntityManager()->RemoveComponentType(entity, ComponentType::BUTTON);
	}

	void ButtonManager::Init()
	{
		SingleComponentManager::Init();
		m_MouseManager = &m_Engine.GetInputManager()->GetMouseManager();
		m_RectTransformManager = m_Engine.GetRectTransformManager();
	}

	Vec2f ButtonManager::GetLocalMousePosition()
	{
		Vec2f mousePosition = { 0,0 };

		mousePosition.x = m_MouseManager->GetLocalPosition(*m_Engine.GetGraphics2dManager()->GetWindow()).x;
		mousePosition.y = m_MouseManager->GetLocalPosition(*m_Engine.GetGraphics2dManager()->GetWindow()).y;
		
		return mousePosition;
	}


}
