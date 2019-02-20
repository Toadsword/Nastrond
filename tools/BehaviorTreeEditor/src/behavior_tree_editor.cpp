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

#include <string>
#include <fstream>
#include <experimental/filesystem>
#include <filesystem> 

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
	ImGui::ShowDemoWindow();
}

void BehaviorTreeEditor::Draw()
{
	ImGui::SetNextWindowPos(ImVec2(150.0f, 100.0f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(600.0f, 600.0f), ImGuiCond_FirstUseEver);
	auto windowName = "Behavior Tree " + m_CurrentFilePath;

	if (ImGui::Begin(windowName.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar))
	{
		//Menu bar
		auto isImportClicked = false;
		if (ImGui::BeginMenuBar())
		{
			DisplayNewMenu();

			if (ImGui::MenuItem("Open"))
			{
				isImportClicked = true;
			}

			DisplayDeleteMenu();

			ImGui::EndMenuBar();
		}

		std::string path;
		if (m_FileBrowser.Render(isImportClicked, path))
		{
			m_RootNode = nullptr;
			LoadBehaviourTree(path);
			m_CurrentFilePath = path;
		}

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, ImGui::GetWindowWidth() - 200);
		//Reset value for node display
		if (!m_CurrentFilePath.empty()) {
			m_IndexButton = 0;
			m_ClickedNode = -1;
			m_IndexClickableNode = 0;

			DisplayNode(m_RootNode);
		}
		ImGui::NextColumn();
		DisplayNodeInfo();
	}
	ImGui::End();

	if (m_NodeToAddChild != nullptr)
	{
		DisplayAddMenu();
	}
}

void BehaviorTreeEditor::LoadBehaviourTree(std::string& path)
{
	m_RootNode = ext::behavior_tree::BehaviorTreeFactory::LoadNodesFromJson(*LoadJson(path), nullptr);
}

void BehaviorTreeEditor::DisplayNode(const Node::ptr& node)
{
	static auto selectionMask = (1 << 2);

	if (node == nullptr)
	{
		if (m_RootNode == nullptr)
		{
			if (ImGui::Begin("Add node to behavior tree", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
			{
				DisplayAddMenuComposite();

				DisplayAddMenuDecorator();

				if (ImGui::Button("Add"))
				{
					if (m_SelectedNodeToAdd != -1)
					{
						m_RootNode = std::make_shared<Node>(nullptr, nullptr,
						                                    static_cast<Node::NodeType>(m_SelectedNodeToAdd));
					}
				}
				ImGui::End();
			}
		}
		return;
	}

	std::string nodeName;
	switch (node->nodeType)
	{
	case Node::NodeType::SEQUENCE_COMPOSITE:
	{
		nodeName += "Sequence Composite";
		nodeName += "##";
		nodeName += std::to_string(m_IndexButton);
		m_IndexButton++;

		if (ImGui::TreeNode(nodeName.c_str()))
		{
			if (DisplayDeleteButton(node))
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
		}
		else
		{
			DisplayDeleteButton(node);
			DisplayAddButton(node);
		}
	}
	break;
	case Node::NodeType::SELECTOR_COMPOSITE:
	{
		nodeName += "Selector Composite";
		nodeName += "##";
		nodeName += std::to_string(m_IndexButton);
		m_IndexButton++;

		if (ImGui::TreeNode(nodeName.c_str()))
		{
			if (DisplayDeleteButton(node))
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
		}
		else
		{
			DisplayDeleteButton(node);
			DisplayAddButton(node);
		}
	}
	break;
	case Node::NodeType::REPEATER_DECORATOR:
	{
		//Get node data
		auto* nodeData = static_cast<ext::behavior_tree::RepeaterData*>(node->m_Datas.get()
		);

		//Create node name
		nodeName += "Repeater Decorator (";
		nodeName += std::to_string(nodeData->m_Limit);
		nodeName += ")";

		//Set flags
		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | (
			(selectionMask & (1 << m_IndexClickableNode)) ? ImGuiTreeNodeFlags_Selected : 0);

		//Check if is open/selected
		auto nodeOpen = ImGui::TreeNodeEx(reinterpret_cast<void*>(static_cast<intptr_t>(m_IndexClickableNode)),
		                                  nodeFlags, nodeName.c_str());
		if (ImGui::IsItemClicked())
		{
			m_ClickedNode = m_IndexClickableNode;
			m_SelectedNode = node;
		}
		m_IndexClickableNode++;

		//If open => display child and button, else only button
		if (nodeOpen)
		{
			//Force early exit to don't draw deleted node
			if (DisplayDeleteButton(node))
			{
				ImGui::TreePop();
				return;
			}
			DisplayAddButton(node);

			DisplayNode(nodeData->m_Child);
			ImGui::TreePop();
		}
		else
		{
			DisplayDeleteButton(node);
			DisplayAddButton(node);
		}

		//If node clicked => changed selectionMask
		if (m_ClickedNode != -1)
		{
			if (ImGui::GetIO().KeyCtrl)
			{
				selectionMask ^= (1 << m_ClickedNode); // CTRL+click to toggle
			}
			else
			{
				selectionMask = (1 << m_ClickedNode); // Click to single-select
			}
		}
	}
	break;
	case Node::NodeType::REPEAT_UNTIL_FAIL_DECORATOR:
	{
		nodeName += "Repeater Until Fail Decorator";
		nodeName += "##";
		nodeName += std::to_string(m_IndexButton);
		m_IndexButton++;

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
		}
		else
		{
			DisplayDeleteButton(node);
			DisplayAddButton(node);
		}
	}
	break;
	case Node::NodeType::SUCCEEDER_DECORATOR:
	{
		nodeName += "Succeeder Decorator";
		nodeName += "##";
		nodeName += std::to_string(m_IndexButton);
		m_IndexButton++;

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
		}
		else
		{
			DisplayDeleteButton(node);
			DisplayAddButton(node);
		}
	}
	break;
	case Node::NodeType::INVERTER_DECORATOR:
	{
		nodeName += "Inverter Decorator";
		nodeName += "##";
		nodeName += std::to_string(m_IndexButton);
		m_IndexButton++;

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
		}
		else
		{
			DisplayDeleteButton(node);
			DisplayAddButton(node);
		}
	}
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
		ImGui::Text("Has dwelling leaf");

		DisplayDeleteButton(node);
		break;
	case Node::NodeType::SET_DWELLING_LEAF:
		ImGui::Text("Set dwelling leaf");

		DisplayDeleteButton(node);
		break;
	case Node::NodeType::ENTER_DWELLING_LEAF:
		ImGui::Text("Enter dwelling leaf");

		DisplayDeleteButton(node);
		break;
	case Node::NodeType::EXIT_DWELLING_LEAF:
		ImGui::Text("Exit dwelling leaf");

		DisplayDeleteButton(node);
		break;
	case Node::NodeType::ENTER_WORKING_PLACE_LEAF:
		ImGui::Text("Enter working place leaf");

		DisplayDeleteButton(node);
		break;
	case Node::NodeType::EXIT_WORKING_PLACE_LEAF:
		ImGui::Text("Exit working place leaf");

		DisplayDeleteButton(node);
		break;
	case Node::NodeType::HAS_JOB_LEAF:
		ImGui::Text("Has job leaf");

		DisplayDeleteButton(node);
		break;
	case Node::NodeType::HAS_STATIC_JOB_LEAF:
		ImGui::Text("Has static job leaf");

		DisplayDeleteButton(node);
		break;
	case Node::NodeType::ASSIGN_JOB_LEAF:
		ImGui::Text("Assign job leaf");

		DisplayDeleteButton(node);
		break;
	case Node::NodeType::IS_DAY_TIME_LEAF:
		ImGui::Text("Is day time leaf");

		DisplayDeleteButton(node);
		break;
	case Node::NodeType::IS_NIGHT_TIME_LEAF:
		ImGui::Text("Is night time leaf");

		DisplayDeleteButton(node);
		break;
	case Node::NodeType::WAIT_DAY_TIME_LEAF:
		ImGui::Text("Wait day time leaf");

		DisplayDeleteButton(node);
		break;
	case Node::NodeType::WAIT_NIGHT_TIME_LEAF:
		ImGui::Text("Wait night time leaf");

		DisplayDeleteButton(node);
		break;
	case Node::NodeType::ASK_INVENTORY_TASK_LEAF:
		ImGui::Text("Ask inventory task leaf");

		DisplayDeleteButton(node);
		break;
	case Node::NodeType::TAKE_RESOURCE_LEAF:
		ImGui::Text("Take resource leaf");

		DisplayDeleteButton(node);
		break;
	case Node::NodeType::PUT_RESOURCE_LEAF:
		ImGui::Text("Put resource leaf");

		DisplayDeleteButton(node);
		break;
	case Node::NodeType::FIND_PATH_TO_LEAF:
	{
		//Get node data
		auto* nodeData = static_cast<ext::behavior_tree::FindPathToData*>(node->m_Datas.get());

		//Create node name
		nodeName += "Find path to leaf (";
		switch (nodeData->m_Destination)
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
		default:;
		}
		nodeName += ")";

		//Set flags
		auto nodeFlags = ((selectionMask & (1 << m_IndexClickableNode)) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

		//Check if is open/selected
		ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
		ImGui::TreeNodeEx(reinterpret_cast<void*>(static_cast<intptr_t>(m_IndexClickableNode)),
			nodeFlags, nodeName.c_str());
		ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
		if (ImGui::IsItemClicked())
		{
			m_ClickedNode = m_IndexClickableNode;
			m_SelectedNode = node;
		}
		m_IndexClickableNode++;

		DisplayDeleteButton(node);

		//If node clicked => changed selectionMask
		if (m_ClickedNode != -1)
		{
			if (ImGui::GetIO().KeyCtrl)
			{
				selectionMask ^= (1 << m_ClickedNode); // CTRL+click to toggle
			}
			else
			{
				selectionMask = (1 << m_ClickedNode); // Click to single-select
			}
		}
	}
		break;
	default: ;
	}
}

bool BehaviorTreeEditor::DisplayDeleteButton(const Node::ptr& node)
{
	std::string buttonName = "-##";
	buttonName += std::to_string(m_IndexButton);

	ImGui::SameLine(300);
	if (ImGui::SmallButton(buttonName.c_str()))
	{
		if (node == m_RootNode)
		{
			m_RootNode = nullptr;
		}
		else
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
	buttonName += std::to_string(m_IndexButton);

	ImGui::SameLine(325);
	if (ImGui::SmallButton(buttonName.c_str()))
	{
		m_NodeToAddChild = node;
	}
	m_IndexButton++;
}

void BehaviorTreeEditor::DisplayDeleteMenu()
{
	static bool openPopup = false;
	if (ImGui::BeginMenu("Delete"))
	{
		if (ImGui::MenuItem("Confirm"))
		{
			openPopup = true;
		}

		if (ImGui::MenuItem("Cancel")) { }
		ImGui::EndMenu();
	}

	if (openPopup)
	{
		ImGui::OpenPopup("Delete?");
		if (ImGui::BeginPopupModal("Delete?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n");
			ImGui::Separator();

			static auto askNextTime = false;
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
			ImGui::Checkbox("Don't ask me next time", &askNextTime);
			ImGui::PopStyleVar();

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				if (remove(m_CurrentFilePath.c_str()) != 0)
				{
					perror("Error deleting file");
					m_CurrentFilePath = "";
					m_RootNode = nullptr;
				}
				else
				{
					puts("File successfully deleted");
					m_CurrentFilePath = "";
					m_RootNode = nullptr;
				}
				openPopup = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				openPopup = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
}

void BehaviorTreeEditor::DisplayAddMenu()
{
	if (ImGui::Begin("Add node to behavior tree", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		DisplayAddMenuComposite();

		DisplayAddMenuDecorator();

		DisplayAddMenuLeaf();
	}

	if (ImGui::Button("Cancel"))
	{
		m_NodeToAddChild = nullptr;
	}

	ImGui::SameLine();
	if (ImGui::Button("Add"))
	{
		if (m_SelectedNodeToAdd != -1)
		{
			m_NodeToAddChild->AddChild(static_cast<Node::NodeType>(m_SelectedNodeToAdd));
			m_NodeToAddChild = nullptr;
		}
	}
	ImGui::End();
}

void BehaviorTreeEditor::DisplayAddMenuComposite()
{
	if (ImGui::TreeNode("Composite : "))
	{
		if (ImGui::Selectable(
			"Sequence", m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::SEQUENCE_COMPOSITE)))
		{
			m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::SEQUENCE_COMPOSITE);
		}
		if (ImGui::Selectable(
			"Selector", m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::SELECTOR_COMPOSITE)))
		{
			m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::SELECTOR_COMPOSITE);
		}

		ImGui::TreePop();
	}
}

void BehaviorTreeEditor::DisplayAddMenuDecorator()
{
	if (ImGui::TreeNode("Decorator : "))
	{
		if (ImGui::Selectable(
			"Repeater", m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::REPEATER_DECORATOR)))
		{
			m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::REPEATER_DECORATOR);
		}
		if (ImGui::Selectable("Repeat until fail",
			m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::
				REPEAT_UNTIL_FAIL_DECORATOR)))
		{
			m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::REPEAT_UNTIL_FAIL_DECORATOR);
		}
		if (ImGui::Selectable(
			"Inverter", m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::INVERTER_DECORATOR)))
		{
			m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::INVERTER_DECORATOR);
		}
		if (ImGui::Selectable("Succeeder",
			m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::SUCCEEDER_DECORATOR)))
		{
			m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::SUCCEEDER_DECORATOR);
		}

		ImGui::TreePop();
	}
}

void BehaviorTreeEditor::DisplayAddMenuLeaf()
{
	if (ImGui::TreeNode("Leaf : "))
	{
		if (ImGui::Selectable("Move to", m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::MOVE_TO_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::MOVE_TO_LEAF);
		}
		if (ImGui::Selectable("Wait for path",
			m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::WAIT_FOR_PATH_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::WAIT_FOR_PATH_LEAF);
		}
		if (ImGui::Selectable("Find path to",
			m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::FIND_PATH_TO_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::FIND_PATH_TO_LEAF);
		}
		if (ImGui::Selectable("Has dwelling",
			m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::HAS_DWELLING_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::HAS_DWELLING_LEAF);
		}
		if (ImGui::Selectable("Set dwelling",
			m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::SET_DWELLING_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::SET_DWELLING_LEAF);
		}
		if (ImGui::Selectable("Enter dwelling",
			m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::ENTER_DWELLING_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::ENTER_DWELLING_LEAF);
		}
		if (ImGui::Selectable("Exit dwelling",
			m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::EXIT_DWELLING_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::EXIT_DWELLING_LEAF);
		}
		if (ImGui::Selectable("Enter working place",
			m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::ENTER_WORKING_PLACE_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::ENTER_DWELLING_LEAF);
		}
		if (ImGui::Selectable("Has job",
			m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::HAS_JOB_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::HAS_JOB_LEAF);
		}
		if (ImGui::Selectable("Has static job",
			m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::HAS_STATIC_JOB_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::HAS_STATIC_JOB_LEAF);
		}
		if (ImGui::Selectable("Assign job",
			m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::ASSIGN_JOB_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::ASSIGN_JOB_LEAF);
		}
		if (ImGui::Selectable("Is day time",
			m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::IS_DAY_TIME_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::IS_DAY_TIME_LEAF);
		}
		if (ImGui::Selectable("Is night time",
			m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::IS_NIGHT_TIME_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::IS_NIGHT_TIME_LEAF);
		}
		if (ImGui::Selectable("Wait day time",
			m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::WAIT_DAY_TIME_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::WAIT_DAY_TIME_LEAF);
		}
		if (ImGui::Selectable("Wait night time",
			m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::WAIT_NIGHT_TIME_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::WAIT_NIGHT_TIME_LEAF);
		}
		if (ImGui::Selectable("Ask inventory task",
			m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::ASK_INVENTORY_TASK_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::ASK_INVENTORY_TASK_LEAF);
		}
		if (ImGui::Selectable("Take resources",
			m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::TAKE_RESOURCE_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::TAKE_RESOURCE_LEAF);
		}
		if (ImGui::Selectable("Put resources",
			m_SelectedNodeToAdd == static_cast<int>(Node::NodeType::PUT_RESOURCE_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(Node::NodeType::PUT_RESOURCE_LEAF);
		}

		ImGui::TreePop();
	}
}

void BehaviorTreeEditor::DisplayNewMenu()
{
	if (ImGui::BeginMenu("New"))
	{
		static char buf1[64] = "";
		ImGui::InputText("behavior_tree_name", buf1, 64);

		if (ImGui::Button("Confirm"))
		{
			NewBehaviorTreeFile(buf1);
			memset(buf1, 0, 64);
		}
		ImGui::EndMenu();
	}
}

void BehaviorTreeEditor::DisplayNodeInfo()
{
	if (m_SelectedNode != nullptr) {
		switch (m_SelectedNode->nodeType) { 
		case Node::NodeType::SEQUENCE_COMPOSITE: {
			ImGui::Text("Sequence");
			ImGui::TextWrapped("Description : \nGo through all child. If a child return success => proceed to the next one. If a child return failure => return failure. If the last child return success => return success");
		}
			break;
		case Node::NodeType::SELECTOR_COMPOSITE:
		{
			ImGui::Text("Selector");
		}break;
		case Node::NodeType::REPEATER_DECORATOR:
		{
			auto data = static_cast<ext::behavior_tree::RepeaterData*>(m_SelectedNode->m_Datas.get());

			ImGui::Text("Repeater");
			ImGui::Spacing();
			ImGui::TextWrapped("Description : \nRepeat the node underneath until the limit has been reached. If limit == 0 => always repeat.");
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Text("Datas:");
			ImGui::Spacing();
			ImGui::InputInt("Limit", &data->m_Limit, 1);
			if(data->m_Limit < 0)
			{
				data->m_Limit = 0;
			}
		}break;
		case Node::NodeType::REPEAT_UNTIL_FAIL_DECORATOR:
		{
			ImGui::Text("Repeat until fail");
		}break;
		case Node::NodeType::SUCCEEDER_DECORATOR:
		{
			ImGui::Text("Succeeder");
		}break;
		case Node::NodeType::INVERTER_DECORATOR:
		{
			ImGui::Text("Inverter");
		}break;
		case Node::NodeType::WAIT_FOR_PATH_LEAF:
		{
			ImGui::Text("Wait for path");
		}break;
		case Node::NodeType::MOVE_TO_LEAF:
		{
			ImGui::Text("Move to");
		}break;
		case Node::NodeType::HAS_DWELLING_LEAF:
		{
			ImGui::Text("Has dwelling");
		}break;
		case Node::NodeType::SET_DWELLING_LEAF:
		{
			ImGui::Text("Set Dwelling");
		}break;
		case Node::NodeType::ENTER_DWELLING_LEAF:
		{
			ImGui::Text("Enter dwelling");
		}break;
		case Node::NodeType::EXIT_DWELLING_LEAF:
		{
			ImGui::Text("Exit dwelling");
		}break;
		case Node::NodeType::ENTER_WORKING_PLACE_LEAF:
		{
			ImGui::Text("Enter working place");
		}break;
		case Node::NodeType::EXIT_WORKING_PLACE_LEAF:
		{
			ImGui::Text("Exit working place");
		}break;
		case Node::NodeType::HAS_JOB_LEAF:
		{
			ImGui::Text("Has job");
		}break;
		case Node::NodeType::HAS_STATIC_JOB_LEAF:
		{
			ImGui::Text("Has static job");
		}break;
		case Node::NodeType::ASSIGN_JOB_LEAF:
		{
			ImGui::Text("Assign job");
		}break;
		case Node::NodeType::IS_DAY_TIME_LEAF:
		{
			ImGui::Text("Is day time");
		}break;
		case Node::NodeType::IS_NIGHT_TIME_LEAF:
		{
			ImGui::Text("Is night time");
		}break;
		case Node::NodeType::WAIT_DAY_TIME_LEAF:
		{
			ImGui::Text("Wait day time");
		}break;
		case Node::NodeType::WAIT_NIGHT_TIME_LEAF:
		{
			ImGui::Text("Wait night time");
		}break;
		case Node::NodeType::ASK_INVENTORY_TASK_LEAF:
		{
			ImGui::Text("Ask inventory task");
		}break;
		case Node::NodeType::TAKE_RESOURCE_LEAF:
		{
			ImGui::Text("Take resource");
		}break;
		case Node::NodeType::PUT_RESOURCE_LEAF:
		{
			ImGui::Text("Put resource");
		}break;
		case Node::NodeType::FIND_PATH_TO_LEAF:
		{
			ImGui::Text("Find path to");
			ImGui::Spacing();
			ImGui::TextWrapped("Description : \nAsk path finder to find a path at from the current position to the giver position.");
			ImGui::Spacing();
		}break;
		default: ;
		}
	}
}

void BehaviorTreeEditor::NewBehaviorTreeFile(const std::string& fileName)
{
	auto currentPath = fs::current_path();
	//TODO faire une variable global pour le chemin d'accès des fichier behavior tree
	currentPath = currentPath.parent_path();
	currentPath.append("data");
	currentPath.append("behavior_tree");
	currentPath.append(fileName + ".asset");

	m_CurrentFilePath = currentPath.generic_string();

	std::ostringstream oss;
	oss << "[System] New file : " << currentPath;
	Log::GetInstance()->Error(oss.str());

	std::ofstream outfile(currentPath);

	outfile << "{\n}";

	outfile.close();
}
}