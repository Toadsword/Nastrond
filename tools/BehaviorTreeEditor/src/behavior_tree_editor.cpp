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
void BehaviorTreeEditor::Init() {}

void BehaviorTreeEditor::Update(float dt)
{
	//ImGui::ShowDemoWindow();
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
			DisplaySaveMenu();
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
		ImGui::SetColumnWidth(0, ImGui::GetWindowWidth() - 250);
		//Reset value for node display
		if (!m_CurrentFilePath.empty())
		{
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
	m_RootNode = ext::behavior_tree::BehaviorTreeUtility::LoadNodesFromJson(*LoadJson(path), nullptr);
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
						                                    static_cast<ext::behavior_tree::NodeType>(m_SelectedNodeToAdd));
					}
				}
				ImGui::End();
			}
		}
		return;
	}

	auto nodeName = ext::behavior_tree::BehaviorTreeUtility::NodeTypeToString(node->nodeType);
	switch (node->nodeType)
	{
	case ext::behavior_tree::NodeType::SEQUENCE_COMPOSITE:
	{
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

			for (const auto& child : static_cast<ext::behavior_tree::CompositeData*>(node->data.get())->children)
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
	case ext::behavior_tree::NodeType::SELECTOR_COMPOSITE:
	{
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

			for (const auto& child : static_cast<ext::behavior_tree::CompositeData*>(node->data.get())->children)
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
	case ext::behavior_tree::NodeType::REPEATER_DECORATOR:
	{
		//Get node data
		auto* nodeData = static_cast<ext::behavior_tree::RepeaterData*>(node->data.get()
		);

		//Create node name
		nodeName += "(";
		nodeName += std::to_string(nodeData->limit);
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

			DisplayNode(nodeData->child);
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
	case ext::behavior_tree::NodeType::REPEAT_UNTIL_FAIL_DECORATOR:
	{
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

			DisplayNode(static_cast<ext::behavior_tree::RepeaterData*>(node->data.get())->child);
			ImGui::TreePop();
		}
		else
		{
			DisplayDeleteButton(node);
			DisplayAddButton(node);
		}
	}
	break;
	case ext::behavior_tree::NodeType::SUCCEEDER_DECORATOR:
	{
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

			DisplayNode(static_cast<ext::behavior_tree::RepeaterData*>(node->data.get())->child);
			ImGui::TreePop();
		}
		else
		{
			DisplayDeleteButton(node);
			DisplayAddButton(node);
		}
	}
	break;
	case ext::behavior_tree::NodeType::INVERTER_DECORATOR:
	{
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

			DisplayNode(static_cast<ext::behavior_tree::RepeaterData*>(node->data.get())->child);
			ImGui::TreePop();
		}
		else
		{
			DisplayDeleteButton(node);
			DisplayAddButton(node);
		}
	}
	break;
	case ext::behavior_tree::NodeType::WAIT_FOR_PATH_LEAF:
	case ext::behavior_tree::NodeType::MOVE_TO_LEAF:
	case ext::behavior_tree::NodeType::HAS_DWELLING_LEAF:
	case ext::behavior_tree::NodeType::SET_DWELLING_LEAF:
	case ext::behavior_tree::NodeType::ENTER_DWELLING_LEAF:
	case ext::behavior_tree::NodeType::EXIT_DWELLING_LEAF:
	case ext::behavior_tree::NodeType::ENTER_WORKING_PLACE_LEAF:
	case ext::behavior_tree::NodeType::EXIT_WORKING_PLACE_LEAF:
	case ext::behavior_tree::NodeType::HAS_JOB_LEAF:
	case ext::behavior_tree::NodeType::HAS_STATIC_JOB_LEAF:
	case ext::behavior_tree::NodeType::ASSIGN_JOB_LEAF:
	case ext::behavior_tree::NodeType::IS_DAY_TIME_LEAF:
	case ext::behavior_tree::NodeType::IS_NIGHT_TIME_LEAF:
	case ext::behavior_tree::NodeType::WAIT_DAY_TIME_LEAF:
	case ext::behavior_tree::NodeType::WAIT_NIGHT_TIME_LEAF:
	case ext::behavior_tree::NodeType::ASK_INVENTORY_TASK_LEAF:
	case ext::behavior_tree::NodeType::TAKE_RESOURCE_LEAF:
	case ext::behavior_tree::NodeType::PUT_RESOURCE_LEAF:
		ImGui::Text(nodeName.c_str());

		DisplayDeleteButton(node);
		break;
	case ext::behavior_tree::NodeType::FIND_PATH_TO_LEAF:
	{
		//Get node data
		auto* nodeData = static_cast<ext::behavior_tree::FindPathToData*>(node->data.get());

		//Create node name
		nodeName += "(";
		nodeName += ext::behavior_tree::BehaviorTreeUtility::NodeDestinationToString(static_cast<ext::behavior_tree::NodeDestination>(static_cast<int>(nodeData->destination)));
		nodeName += ")";

		//Set flags
		auto nodeFlags = ((selectionMask & (1 << m_IndexClickableNode)) ? ImGuiTreeNodeFlags_Selected : 0) |
			ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

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
	static auto openPopup = false;
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
			m_NodeToAddChild->AddChild(static_cast<ext::behavior_tree::NodeType>(m_SelectedNodeToAdd));
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
			ext::behavior_tree::BehaviorTreeUtility::NodeTypeToString(ext::behavior_tree::NodeType::SEQUENCE_COMPOSITE).c_str(),
			m_SelectedNodeToAdd == static_cast<int>(ext::behavior_tree::NodeType::SEQUENCE_COMPOSITE)))
		{
			m_SelectedNodeToAdd = static_cast<int>(ext::behavior_tree::NodeType::SEQUENCE_COMPOSITE);
		}
		if (ImGui::Selectable(
			ext::behavior_tree::BehaviorTreeUtility::NodeTypeToString(ext::behavior_tree::NodeType::SELECTOR_COMPOSITE).c_str(),
			m_SelectedNodeToAdd == static_cast<int>(ext::behavior_tree::NodeType::SELECTOR_COMPOSITE)))
		{
			m_SelectedNodeToAdd = static_cast<int>(ext::behavior_tree::NodeType::SELECTOR_COMPOSITE);
		}

		ImGui::TreePop();
	}
}

void BehaviorTreeEditor::DisplayAddMenuDecorator()
{
	if (ImGui::TreeNode("Decorator : "))
	{
		if (ImGui::Selectable(
			ext::behavior_tree::BehaviorTreeUtility::NodeTypeToString(ext::behavior_tree::NodeType::REPEATER_DECORATOR).c_str(),
			m_SelectedNodeToAdd == static_cast<int>(ext::behavior_tree::NodeType::REPEATER_DECORATOR)))
		{
			m_SelectedNodeToAdd = static_cast<int>(ext::behavior_tree::NodeType::REPEATER_DECORATOR);
		}
		if (ImGui::Selectable(
			ext::behavior_tree::BehaviorTreeUtility::NodeTypeToString(ext::behavior_tree::NodeType::REPEAT_UNTIL_FAIL_DECORATOR).
			c_str(),
			m_SelectedNodeToAdd == static_cast<int>(ext::behavior_tree::NodeType::
				REPEAT_UNTIL_FAIL_DECORATOR)))
		{
			m_SelectedNodeToAdd = static_cast<int>(ext::behavior_tree::NodeType::REPEAT_UNTIL_FAIL_DECORATOR);
		}
		if (ImGui::Selectable(
			ext::behavior_tree::BehaviorTreeUtility::NodeTypeToString(ext::behavior_tree::NodeType::INVERTER_DECORATOR).c_str(),
			m_SelectedNodeToAdd == static_cast<int>(ext::behavior_tree::NodeType::INVERTER_DECORATOR)))
		{
			m_SelectedNodeToAdd = static_cast<int>(ext::behavior_tree::NodeType::INVERTER_DECORATOR);
		}
		if (ImGui::Selectable(
			ext::behavior_tree::BehaviorTreeUtility::NodeTypeToString(ext::behavior_tree::NodeType::SUCCEEDER_DECORATOR).c_str(),
			m_SelectedNodeToAdd == static_cast<int>(ext::behavior_tree::NodeType::SUCCEEDER_DECORATOR)))
		{
			m_SelectedNodeToAdd = static_cast<int>(ext::behavior_tree::NodeType::SUCCEEDER_DECORATOR);
		}

		ImGui::TreePop();
	}
}

void BehaviorTreeEditor::DisplayAddMenuLeaf()
{
	if (ImGui::TreeNode("Leaf : "))
	{
		if (ImGui::Selectable(ext::behavior_tree::BehaviorTreeUtility::NodeTypeToString(ext::behavior_tree::NodeType::MOVE_TO_LEAF).c_str(), m_SelectedNodeToAdd == static_cast<int>(ext::behavior_tree::NodeType::MOVE_TO_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(ext::behavior_tree::NodeType::MOVE_TO_LEAF);
		}
		if (ImGui::Selectable(ext::behavior_tree::BehaviorTreeUtility::NodeTypeToString(ext::behavior_tree::NodeType::WAIT_FOR_PATH_LEAF).c_str(),
		                      m_SelectedNodeToAdd == static_cast<int>(ext::behavior_tree::NodeType::WAIT_FOR_PATH_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(ext::behavior_tree::NodeType::WAIT_FOR_PATH_LEAF);
		}
		if (ImGui::Selectable(ext::behavior_tree::BehaviorTreeUtility::NodeTypeToString(ext::behavior_tree::NodeType::FIND_PATH_TO_LEAF).c_str(),
		                      m_SelectedNodeToAdd == static_cast<int>(ext::behavior_tree::NodeType::FIND_PATH_TO_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(ext::behavior_tree::NodeType::FIND_PATH_TO_LEAF);
		}
		if (ImGui::Selectable(ext::behavior_tree::BehaviorTreeUtility::NodeTypeToString(ext::behavior_tree::NodeType::HAS_DWELLING_LEAF).c_str(),
		                      m_SelectedNodeToAdd == static_cast<int>(ext::behavior_tree::NodeType::HAS_DWELLING_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(ext::behavior_tree::NodeType::HAS_DWELLING_LEAF);
		}
		if (ImGui::Selectable(ext::behavior_tree::BehaviorTreeUtility::NodeTypeToString(ext::behavior_tree::NodeType::SET_DWELLING_LEAF).c_str(),
		                      m_SelectedNodeToAdd == static_cast<int>(ext::behavior_tree::NodeType::SET_DWELLING_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(ext::behavior_tree::NodeType::SET_DWELLING_LEAF);
		}
		if (ImGui::Selectable(ext::behavior_tree::BehaviorTreeUtility::NodeTypeToString(ext::behavior_tree::NodeType::ENTER_DWELLING_LEAF).c_str(),
		                      m_SelectedNodeToAdd == static_cast<int>(ext::behavior_tree::NodeType::ENTER_DWELLING_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(ext::behavior_tree::NodeType::ENTER_DWELLING_LEAF);
		}
		if (ImGui::Selectable(ext::behavior_tree::BehaviorTreeUtility::NodeTypeToString(ext::behavior_tree::NodeType::EXIT_DWELLING_LEAF).c_str(),
		                      m_SelectedNodeToAdd == static_cast<int>(ext::behavior_tree::NodeType::EXIT_DWELLING_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(ext::behavior_tree::NodeType::EXIT_DWELLING_LEAF);
		}
		if (ImGui::Selectable(ext::behavior_tree::BehaviorTreeUtility::NodeTypeToString(ext::behavior_tree::NodeType::ENTER_DWELLING_LEAF).c_str(),
		                      m_SelectedNodeToAdd == static_cast<int>(ext::behavior_tree::NodeType::ENTER_WORKING_PLACE_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(ext::behavior_tree::NodeType::ENTER_DWELLING_LEAF);
		}
		if (ImGui::Selectable(ext::behavior_tree::BehaviorTreeUtility::NodeTypeToString(ext::behavior_tree::NodeType::HAS_JOB_LEAF).c_str(),
		                      m_SelectedNodeToAdd == static_cast<int>(ext::behavior_tree::NodeType::HAS_JOB_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(ext::behavior_tree::NodeType::HAS_JOB_LEAF);
		}
		if (ImGui::Selectable(ext::behavior_tree::BehaviorTreeUtility::NodeTypeToString(ext::behavior_tree::NodeType::HAS_STATIC_JOB_LEAF).c_str(),
		                      m_SelectedNodeToAdd == static_cast<int>(ext::behavior_tree::NodeType::HAS_STATIC_JOB_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(ext::behavior_tree::NodeType::HAS_STATIC_JOB_LEAF);
		}
		if (ImGui::Selectable(ext::behavior_tree::BehaviorTreeUtility::NodeTypeToString(ext::behavior_tree::NodeType::ASSIGN_JOB_LEAF).c_str(),
		                      m_SelectedNodeToAdd == static_cast<int>(ext::behavior_tree::NodeType::ASSIGN_JOB_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(ext::behavior_tree::NodeType::ASSIGN_JOB_LEAF);
		}
		if (ImGui::Selectable(ext::behavior_tree::BehaviorTreeUtility::NodeTypeToString(ext::behavior_tree::NodeType::IS_DAY_TIME_LEAF).c_str(),
		                      m_SelectedNodeToAdd == static_cast<int>(ext::behavior_tree::NodeType::IS_DAY_TIME_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(ext::behavior_tree::NodeType::IS_DAY_TIME_LEAF);
		}
		if (ImGui::Selectable(ext::behavior_tree::BehaviorTreeUtility::NodeTypeToString(ext::behavior_tree::NodeType::IS_NIGHT_TIME_LEAF).c_str(),
		                      m_SelectedNodeToAdd == static_cast<int>(ext::behavior_tree::NodeType::IS_NIGHT_TIME_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(ext::behavior_tree::NodeType::IS_NIGHT_TIME_LEAF);
		}
		if (ImGui::Selectable(ext::behavior_tree::BehaviorTreeUtility::NodeTypeToString(ext::behavior_tree::NodeType::WAIT_DAY_TIME_LEAF).c_str(),
		                      m_SelectedNodeToAdd == static_cast<int>(ext::behavior_tree::NodeType::WAIT_DAY_TIME_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(ext::behavior_tree::NodeType::WAIT_DAY_TIME_LEAF);
		}
		if (ImGui::Selectable(ext::behavior_tree::BehaviorTreeUtility::NodeTypeToString(ext::behavior_tree::NodeType::WAIT_NIGHT_TIME_LEAF).c_str(),
		                      m_SelectedNodeToAdd == static_cast<int>(ext::behavior_tree::NodeType::WAIT_NIGHT_TIME_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(ext::behavior_tree::NodeType::WAIT_NIGHT_TIME_LEAF);
		}
		if (ImGui::Selectable(ext::behavior_tree::BehaviorTreeUtility::NodeTypeToString(ext::behavior_tree::NodeType::ASK_INVENTORY_TASK_LEAF).c_str(),
		                      m_SelectedNodeToAdd == static_cast<int>(ext::behavior_tree::NodeType::ASK_INVENTORY_TASK_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(ext::behavior_tree::NodeType::ASK_INVENTORY_TASK_LEAF);
		}
		if (ImGui::Selectable(ext::behavior_tree::BehaviorTreeUtility::NodeTypeToString(ext::behavior_tree::NodeType::TAKE_RESOURCE_LEAF).c_str(),
		                      m_SelectedNodeToAdd == static_cast<int>(ext::behavior_tree::NodeType::TAKE_RESOURCE_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(ext::behavior_tree::NodeType::TAKE_RESOURCE_LEAF);
		}
		if (ImGui::Selectable(ext::behavior_tree::BehaviorTreeUtility::NodeTypeToString(ext::behavior_tree::NodeType::PUT_RESOURCE_LEAF).c_str(),
		                      m_SelectedNodeToAdd == static_cast<int>(ext::behavior_tree::NodeType::PUT_RESOURCE_LEAF)))
		{
			m_SelectedNodeToAdd = static_cast<int>(ext::behavior_tree::NodeType::PUT_RESOURCE_LEAF);
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

void BehaviorTreeEditor::DisplaySaveMenu() const
{
	if (ImGui::Button("Save"))
	{
		if (!m_CurrentFilePath.empty())
		{
			ext::behavior_tree::BehaviorTreeUtility::SaveBehaviorTreeToJson(m_RootNode, m_CurrentFilePath);
		}
	}
}

void BehaviorTreeEditor::DisplayNodeInfo() const
{
	if (m_SelectedNode != nullptr)
	{
		//Display node's name
		auto nodeName = ext::behavior_tree::BehaviorTreeUtility::NodeTypeToString(m_SelectedNode->nodeType);
		ImGui::Text(nodeName.c_str());

		//Display node's details
		switch (m_SelectedNode->nodeType)
		{
		case ext::behavior_tree::NodeType::SEQUENCE_COMPOSITE:
		{
			ImGui::TextWrapped(
				"Description : \nGo through all child. If a child return success => proceed to the next one. If a child return failure => return failure. If the last child return success => return success");
		}
		break;
		case ext::behavior_tree::NodeType::SELECTOR_COMPOSITE:

		break;
		case ext::behavior_tree::NodeType::REPEATER_DECORATOR:
		{
			auto data = static_cast<ext::behavior_tree::RepeaterData*>(m_SelectedNode->data.get());

			ImGui::Spacing();
			ImGui::TextWrapped(
				"Description : \nRepeat the node underneath until the limit has been reached. If limit == 0 => always repeat.");
			ImGui::Spacing();
			ImGui::Text("Datas:");
			ImGui::Spacing();
			ImGui::Text("Limit : ");
			ImGui::SameLine();
			ImGui::InputInt("", &data->limit, 1);
			if (data->limit < 0)
			{
				data->limit = 0;
			}
		}
		break;
		case ext::behavior_tree::NodeType::REPEAT_UNTIL_FAIL_DECORATOR:

		break;
		case ext::behavior_tree::NodeType::SUCCEEDER_DECORATOR:

		break;
		case ext::behavior_tree::NodeType::INVERTER_DECORATOR:

		break;
		case ext::behavior_tree::NodeType::WAIT_FOR_PATH_LEAF:

		break;
		case ext::behavior_tree::NodeType::MOVE_TO_LEAF:

		break;
		case ext::behavior_tree::NodeType::HAS_DWELLING_LEAF:

		break;
		case ext::behavior_tree::NodeType::SET_DWELLING_LEAF:

		break;
		case ext::behavior_tree::NodeType::ENTER_DWELLING_LEAF:

		break;
		case ext::behavior_tree::NodeType::EXIT_DWELLING_LEAF:

		break;
		case ext::behavior_tree::NodeType::ENTER_WORKING_PLACE_LEAF:

		break;
		case ext::behavior_tree::NodeType::EXIT_WORKING_PLACE_LEAF:

		break;
		case ext::behavior_tree::NodeType::HAS_JOB_LEAF:

		break;
		case ext::behavior_tree::NodeType::HAS_STATIC_JOB_LEAF:

		break;
		case ext::behavior_tree::NodeType::ASSIGN_JOB_LEAF:

		break;
		case ext::behavior_tree::NodeType::IS_DAY_TIME_LEAF:

		break;
		case ext::behavior_tree::NodeType::IS_NIGHT_TIME_LEAF:

		break;
		case ext::behavior_tree::NodeType::WAIT_DAY_TIME_LEAF:

		break;
		case ext::behavior_tree::NodeType::WAIT_NIGHT_TIME_LEAF:

		break;
		case ext::behavior_tree::NodeType::ASK_INVENTORY_TASK_LEAF:

		break;
		case ext::behavior_tree::NodeType::TAKE_RESOURCE_LEAF:

		break;
		case ext::behavior_tree::NodeType::PUT_RESOURCE_LEAF:

		break;
		case ext::behavior_tree::NodeType::FIND_PATH_TO_LEAF:
		{
			auto* data = static_cast<ext::behavior_tree::FindPathToData*>(m_SelectedNode->data.get());

			
			ImGui::Spacing();
			ImGui::TextWrapped(
				"Description : \nAsk path finder to find a path at from the current position to the giver position.");
			ImGui::Spacing();
			ImGui::Text("Datas:");
			ImGui::Spacing();

			ImGui::Text("Destination : ");
			ImGui::SameLine();
			if (ImGui::Button(ext::behavior_tree::BehaviorTreeUtility::NodeDestinationToString(static_cast<ext::behavior_tree::NodeDestination>(static_cast<int>(data->destination))).c_str()))
				ImGui::OpenPopup("my_select_popup");
			ImGui::SameLine();
			if (ImGui::BeginPopup("my_select_popup"))
			{
				ImGui::Text("Destination");
				ImGui::Separator();
				for (auto i = 0; i < static_cast<int>(ext::behavior_tree::NodeDestination::LENGTH); i++)
					if (ImGui::Selectable(ext::behavior_tree::BehaviorTreeUtility::NodeDestinationToString(static_cast<ext::behavior_tree::NodeDestination>(i)).c_str()))
						data->destination = static_cast<ext::behavior_tree::NodeDestination>(i);
				ImGui::EndPopup();
			}
		}
		break;
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
