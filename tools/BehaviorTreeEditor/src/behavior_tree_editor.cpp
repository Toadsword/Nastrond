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
#include <fstream>  
#include <experimental/filesystem> // C++-standard header file name
#include <filesystem> // Microsoft-specific implementation header file name

#include <behavior_tree_editor.h>
#include <utility/json_utility.h>
#include <utility/log.h>
#include <extensions/AI/behavior_tree_nodes_core.h>
#include <extensions/AI/behavior_tree_factory.h>


namespace sfge::tools
{
void BehaviorTreeEditor::Init() { }

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
		//Menu bar
		auto isImportClicked = false;
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("New"))
			{
				static char buf1[64] = ""; 
				ImGui::InputText("behavior_tree_name", buf1, 64);

				if(ImGui::Button("Confirm"))
				{
					NewBehaviorTreeFile(buf1);
					memset(buf1, 0, 64);
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Open"))
			{
				isImportClicked = true;
			}
			if (ImGui::MenuItem("Delete"))
			{
				m_HasToDeleteFile = true;
			}
			ImGui::EndMenuBar();
		}

		std::string path;
		if (m_FileBrowser.Render(isImportClicked, path))
		{
			m_RootNode = nullptr;
			LoadBehaviourTree(path);
			m_CurrentFilePath = path;
		}

		//Tree displayed
		if (m_CurrentFilePath != "")
		{
			m_IndexButton = 0;
			DisplayNode(m_RootNode);
		}
	}
	ImGui::End();

	if(m_HasToDeleteFile)
	{
		if (ImGui::Begin("Are you sure to delete", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			
		}
		ImGui::End();
	}

	if (m_NodeToAddChild != nullptr)
	{
		if (ImGui::Begin("Add node to behavior tree", NULL, ImGuiWindowFlags_AlwaysAutoResize))
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

		if (ImGui::Button("Add"))
		{
			if (m_SelectedNodeToAdd != -1)
			{
				m_NodeToAddChild->AddChild(static_cast<Node::NodeType>(m_SelectedNodeToAdd));
				m_NodeToAddChild = nullptr;
			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			m_NodeToAddChild = nullptr;
		}
		ImGui::End();
	}

	if(m_HasToDeleteFile)
	{
		ImGui::OpenPopup("Delete");

		if(ImGui::BeginPopup("Delete"))
		{
			if(ImGui::Button("Confirm"))
			{
				if (remove(m_CurrentFilePath.c_str()) != 0) {
					perror("Error deleting file");
					m_HasToDeleteFile = false;
					m_CurrentFilePath = "";
					m_RootNode = nullptr;
				}
				else {
					puts("File successfully deleted");
					m_HasToDeleteFile = false;
					m_CurrentFilePath = "";
					m_RootNode = nullptr;
				}
			}

			if (ImGui::Button("Cancel"))
			{
				m_HasToDeleteFile = false;
			}
			ImGui::EndPopup();
		}
	}
}

void BehaviorTreeEditor::LoadBehaviourTree(std::string& path)
{
	m_RootNode = ext::behavior_tree::BehaviorTreeFactory::LoadNodesFromJson(*LoadJson(path), nullptr);
}

void BehaviorTreeEditor::DisplayNode(const Node::ptr& node)
{
	if (node == nullptr)
	{
		if(m_RootNode == nullptr)
		{
			
			if (ImGui::Begin("Add node to behavior tree", NULL, ImGuiWindowFlags_AlwaysAutoResize))
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

				if (ImGui::Button("Add"))
				{
					if (m_SelectedNodeToAdd != -1)
					{
						m_RootNode = std::make_shared<Node>(nullptr, nullptr, static_cast<Node::NodeType>(m_SelectedNodeToAdd));
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
		nodeName += m_IndexButton;
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
		nodeName += m_IndexButton;
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
		nodeName += "Repeater Decorator (";
		nodeName += std::to_string(static_cast<ext::behavior_tree::RepeaterData*>(node->m_Datas.get())->m_Limit);
		nodeName += ")";
		nodeName += "##";
		nodeName += m_IndexButton;
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
	case Node::NodeType::REPEAT_UNTIL_FAIL_DECORATOR:
	{
		nodeName += "Repeater Until Fail Decorator";
		nodeName += "##";
		nodeName += m_IndexButton;
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
		nodeName += m_IndexButton;
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
		nodeName += m_IndexButton;
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
		nodeName += "Find path to leaf (";
		switch (static_cast<ext::behavior_tree::FindPathToData*>(node->m_Datas.get())->m_Destination)
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
	buttonName += m_IndexButton;

	ImGui::SameLine(325);
	if (ImGui::SmallButton(buttonName.c_str()))
	{
		m_NodeToAddChild = node;
	}
	m_IndexButton++;
}

void BehaviorTreeEditor::NewBehaviorTreeFile(const std::string& fileName)
{
	auto currentPath = fs::current_path(); //TODO faire une variable global pour le chemin d'accès des fichier behavior tree
	currentPath = currentPath.parent_path();
	currentPath.append("data");
	currentPath.append("behavior_tree");
	currentPath.append(fileName+ ".asset");

	m_CurrentFilePath = currentPath.generic_string();

	std::ostringstream oss;
	oss << "[System] New file : " << currentPath;
	Log::GetInstance()->Error(oss.str());

	std::ofstream outfile(currentPath);

	outfile << "{\n}";

	outfile.close();
}
}
