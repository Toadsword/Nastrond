/*
MIT License

Copyright (c) 2018 SAE Institute Switzerland AG

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

----------------------------
Author : Duncan Bourquard (bourquardduncan@gmail.com)
Date : November 2018
Project : AnimationTool for SFGE
*/
#include <tilemap_imgui_manager.h>

#include <engine/engine.h>
#include <tilemap_creator.h>

#include <imgui.h>
#include <imgui-SFML.h>

#include <input/input.h>

namespace sfge::tools
{
	void TilemapImguiManager::Init(TilemapCreator* engine)
	{
		m_TilemapCreator = engine;
		m_IsInit = true;
	}

	void TilemapImguiManager::Update(float dt)
	{
		if (!m_IsInit)
			return;

		InputManager* inputManager = m_TilemapCreator->GetEngine().GetInputManager();

		//Keyboard
		KeyboardManager& keyboardManager = inputManager->GetKeyboardManager();
		
		bool isControlHeld = keyboardManager.IsKeyHeld(sf::Keyboard::LControl) || keyboardManager.IsKeyHeld(sf::Keyboard::RControl);
		if(isControlHeld && keyboardManager.IsKeyDown(sf::Keyboard::S))
		{
			//m_SaveResult = m_AnimCreator->GetAnimationManager()->ExportToJson(m_AnimCreator->GetTextureManager()->GetAllTextures());
			//m_OpenModalSave = m_SaveResult != SAVE_SUCCESS;
		}

		//ImGui::ShowDemoWindow();
	}

	void TilemapImguiManager::Draw()
	{
		if (!m_IsInit)
			return;
	}
}
