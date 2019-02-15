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

*/

#include <imgui.h>
#include <imgui-SFML.h>

#include <string>
#include <iostream>

#include <behavior_tree_editor.h>
#include <utility/json_utility.h>
#include "utility/log.h"

namespace sfge::tools
{
void BehaviorTreeEditor::Init()
{
	std::cout << "ici\n";
}

void BehaviorTreeEditor::Update(float dt)
{
	ImGui::ShowDemoWindow();
}

void BehaviorTreeEditor::Draw()
{
	ImGui::SetNextWindowPos(ImVec2(150.0f, 100.0f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(600.0f, 600.0f), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Behavior Tree", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar))
	{
		bool isImportClicked = false;
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Import")) {
					isImportClicked = true;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		std::string path;
		if (m_FileBrowser.Render(isImportClicked, path)) {
			//Clear previous behaviorTree
			LoadBehaviourTree(path);
		}
	}
	ImGui::End();
}

void BehaviorTreeEditor::LoadBehaviourTree(std::string& path)
{
	const auto behaviorTreeJson = LoadJson(path);

	
}
}
