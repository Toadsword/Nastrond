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
#include <utility/log.h>
#include <extensions/AI/behavior_tree_nodes_core.h>
#include <extensions/AI/behavior_tree_factory.h>

namespace sfge::tools
{
	void BehaviorTreeEditor::Init()
	{
	}

	void BehaviorTreeEditor::Update(float dt)
	{
		//ImGui::ShowDemoWindow();
	}

	void BehaviorTreeEditor::Draw()
	{
		ImGui::SetNextWindowPos(ImVec2(150.0f, 100.0f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(600.0f, 600.0f), ImGuiCond_FirstUseEver);
		if (ImGui::Begin("Behavior Tree", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar))
		{
			//Menu bar
			auto isImportClicked = false;
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
				m_RootNode = nullptr;
				LoadBehaviourTree(path);
			}

			//Tree displayed
			if (m_RootNode != nullptr) {
				m_IndexButton = 0;
				DisplayNode(m_RootNode);
			}
		}
		ImGui::End();

		if(m_NodeToAddChild != nullptr)
		{
			if (ImGui::Begin("Add node to behavior tree", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				if (ImGui::TreeNode("Composite : "))
				{
					if(ImGui::Selectable("Sequence", m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::SEQUENCE_COMPOSITE))) {
						m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::SEQUENCE_COMPOSITE);
					}
					if (ImGui::Selectable("Selector", m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::SELECTOR_COMPOSITE))) {
						m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::SELECTOR_COMPOSITE);
					}

					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Decorator : "))
				{
					if (ImGui::Selectable("Repeater", m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::REPEATER_DECORATOR))) {
						m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::REPEATER_DECORATOR);
					}
					if (ImGui::Selectable("Repeat until fail", m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::REPEAT_UNTIL_FAIL_DECORATOR))) {
						m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::REPEAT_UNTIL_FAIL_DECORATOR);
					}
					if (ImGui::Selectable("Inverter", m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::INVERTER_DECORATOR))) {
						m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::INVERTER_DECORATOR);
					}
					if (ImGui::Selectable("Succeeder", m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::SUCCEEDER_DECORATOR))) {
						m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::SUCCEEDER_DECORATOR);
					}

					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Leaf : "))
				{
					if (ImGui::Selectable("Move to", m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::MOVE_TO_LEAF))) {
						m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::MOVE_TO_LEAF);
					}
					if (ImGui::Selectable("Wait for path", m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::WAIT_FOR_PATH_LEAF))) {
						m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::WAIT_FOR_PATH_LEAF);
					}
					if (ImGui::Selectable("Find path to", m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::FIND_PATH_TO_LEAF))) {
						m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::FIND_PATH_TO_LEAF);
					}

					ImGui::TreePop();
				}
			}

			if(ImGui::Button("Add"))
			{
				if (m_SelectedNodeToAdd != -1) {
					m_NodeToAddChild->AddChild(static_cast<Node::NodeType>(m_SelectedNodeToAdd));
					m_NodeToAddChild = nullptr;
				}
			}
			ImGui::End();
		}
	}

	void BehaviorTreeEditor::LoadBehaviourTree(std::string& path)
	{
		m_RootNode = ext::behavior_tree::BehaviorTreeFactory::LoadNodesFromJson(*LoadJson(path), nullptr);
	}

	void BehaviorTreeEditor::DisplayNode(const Node::ptr& node)
	{
		if(node == nullptr)
		{
			return;
		}

		std::string nodeName = "";
		switch(node->nodeType)
		{
		case Node::NodeType::SEQUENCE_COMPOSITE:
			nodeName += "Sequence Composite";

			if (ImGui::TreeNode(nodeName.c_str()))
			{
				if(DisplayDeleteButton(node))
				{
					ImGui::TreePop();
					return;
				}

				DisplayAddButton(node);

				for (const auto& child : static_cast<ext::behavior_tree::CompositeData*>(node->m_Datas.get())->m_Children)
				{
					DisplayNode(child);
				}
				ImGui::TreePop();

			}else
			{
				DisplayDeleteButton(node);
				DisplayAddButton(node);
			}
			break;

		case Node::NodeType::SELECTOR_COMPOSITE: 
			break;
		case Node::NodeType::REPEATER_DECORATOR:
			nodeName += "Repeater Decorator (";
			nodeName += std::to_string(static_cast<ext::behavior_tree::RepeaterData*>(node->m_Datas.get())->m_Limit);
			nodeName += ")";

			if (ImGui::TreeNode(nodeName.c_str()))
			{
				if (DisplayDeleteButton(node))
				{
					ImGui::TreePop();
					return;
				}
				DisplayAddButton(node);

				DisplayNode(static_cast<ext::behavior_tree::RepeaterData*>(node->m_Datas.get())->m_Child);
				ImGui::TreePop();

			}else
			{
				DisplayDeleteButton(node);
				DisplayAddButton(node);
			}
			break;
		case Node::NodeType::REPEAT_UNTIL_FAIL_DECORATOR: 
			break;
		case Node::NodeType::SUCCEEDER_DECORATOR: 
			break;
		case Node::NodeType::INVERTER_DECORATOR: 
			break;
		case Node::NodeType::WAIT_FOR_PATH_LEAF:
			ImGui::Text("Wait for path leaf");

			DisplayDeleteButton(node);
			break;
		case Node::NodeType::MOVE_TO_LEAF:
			ImGui::Text("Move to leaf");

			DisplayDeleteButton(node);
			break;
		case Node::NodeType::HAS_DWELLING_LEAF: 
			break;
		case Node::NodeType::SET_DWELLING_LEAF: 
			break;
		case Node::NodeType::ENTER_DWELLING_LEAF: 
			break;
		case Node::NodeType::EXIT_DWELLING_LEAF: 
			break;
		case Node::NodeType::ENTER_WORKING_PLACE_LEAF: 
			break;
		case Node::NodeType::EXIT_WORKING_PLACE_LEAF: 
			break;
		case Node::NodeType::HAS_JOB_LEAF: 
			break;
		case Node::NodeType::HAS_STATIC_JOB_LEAF: 
			break;
		case Node::NodeType::ASSIGN_JOB_LEAF: 
			break;
		case Node::NodeType::IS_DAY_TIME_LEAF: 
			break;
		case Node::NodeType::IS_NIGHT_TIME_LEAF: 
			break;
		case Node::NodeType::WAIT_DAY_TIME_LEAF: 
			break;
		case Node::NodeType::WAIT_NIGHT_TIME_LEAF: 
			break;
		case Node::NodeType::ASK_INVENTORY_TASK_LEAF: 
			break;
		case Node::NodeType::TAKE_RESOURCE_LEAF: 
			break;
		case Node::NodeType::FIND_PATH_TO_LEAF:
			nodeName += "Find path to leaf (";
			switch(static_cast<ext::behavior_tree::FindPathToData*>(node->m_Datas.get())->m_Destination)
			{
			case ext::behavior_tree::NodeDestination::RANDOM: 
				nodeName += "random";
				break;
			case ext::behavior_tree::NodeDestination::DWELLING: 
				nodeName += "dwelling"; 
				break;
			case ext::behavior_tree::NodeDestination::WORKING_PLACE: 
				nodeName += "working place";
				break;
			case ext::behavior_tree::NodeDestination::INVENTORY_TASK_GIVER: 
				nodeName += "inventory task giver";
				break;
			case ext::behavior_tree::NodeDestination::INVENTORY_TASK_RECEIVER: 
				nodeName += "inventory task receiver";
				break;
			default: ;
			}
			nodeName += ")";

			ImGui::Text(nodeName.c_str());

			DisplayDeleteButton(node);
			break;
		default: ;
		}
	}

	bool BehaviorTreeEditor::DisplayDeleteButton(const Node::ptr& node)
	{
		std::string buttonName = "-##";
		buttonName += m_IndexButton;

		ImGui::SameLine(300);
		if (ImGui::SmallButton(buttonName.c_str()))
		{
			if (node == m_RootNode)
			{
				m_RootNode = nullptr;
			}else
			{
				node->Destroy();
			}
			return true;
		}
		m_IndexButton++;
		return false;
	}

	void BehaviorTreeEditor::DisplayAddButton(const Node::ptr& node)
	{
		std::string buttonName = "+##";
		buttonName += m_IndexButton;

		ImGui::SameLine(325);
		if (ImGui::SmallButton(buttonName.c_str()))
		{
			m_NodeToAddChild = node;
		}
		m_IndexButton++;
	}
}
