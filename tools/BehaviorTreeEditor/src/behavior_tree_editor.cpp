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

namespace sfge::tools
{
	void BehaviorTreeEditor::Init()
	{
		std::cout << "ici\n";
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
				m_RootNode = nullptr;
				LoadBehaviourTree(path);
			}

			//Tree displayed
			if (m_RootNode != nullptr) {
				DisplayNode(m_RootNode);
			}
		}
		ImGui::End();
	}

	void BehaviorTreeEditor::LoadBehaviourTree(std::string& path)
	{
		const auto behaviorTreeJson = *LoadJson(path);

		if (CheckJsonParameter(behaviorTreeJson, "rootNode", json::value_t::array))
		{
			auto rootNodeJson = behaviorTreeJson["rootNode"];
			if (CheckJsonExists(rootNodeJson[0], "type"))
			{
				const NodeType nodeType = rootNodeJson[0]["type"];

				switch (nodeType)
				{
				case NodeType::NONE:
				{
					std::ostringstream oss;
					oss << "[Error] No type specified for root node : " << rootNodeJson[0];
					Log::GetInstance()->Error(oss.str());
				}
				break;
				case NodeType::LEAF:
					m_RootNode = AddLeafNodeFromJson(rootNodeJson[0], nullptr);
					break;
				case NodeType::COMPOSITE:
					m_RootNode = AddCompositeNodeFromJson(rootNodeJson[0], nullptr);
					break;
				case NodeType::DECORATOR:
					m_RootNode = AddDecoratorNodeFromJson(rootNodeJson[0], nullptr);
					break;
				default:;
				}
			}
			else
			{
				std::ostringstream oss;
				oss << "[Error] the type for the rootNode is missing : " << rootNodeJson[0];
				Log::GetInstance()->Error(oss.str());
			}
		}
		else
		{
			std::ostringstream oss;
			oss << "[Error] the rootNode parameter is missing : " << behaviorTreeJson;
			Log::GetInstance()->Error(oss.str());
		}
	}

	std::shared_ptr<Node> BehaviorTreeEditor::AddLeafNodeFromJson(json& behaviorTreeJson, const Node::ptr& parentNode)
	{
		std::cout << "Load leaf\n";
		auto leaf = std::make_shared<Node>(nullptr, parentNode);

		if (behaviorTreeJson["name"] == "WaitForPathLeaf")
		{
			std::cout << "   -> Wait for path leaf\n";
			leaf->nodeType = Node::NodeType::WAIT_FOR_PATH_LEAF;
		}
		else if (behaviorTreeJson["name"] == "FindRandomPathLeaf")
		{
			std::cout << "   -> find random path leaf\n";
			leaf->nodeType = Node::NodeType::FIND_RANDOM_PATH_LEAF;
		}
		else if (behaviorTreeJson["name"] == "MoveToLeaf")
		{
			std::cout << "   -> Move to leaf\n";
			leaf->nodeType = Node::NodeType::MOVE_TO_LEAF;
		}
		else if (behaviorTreeJson["name"] == "FindPathToLeaf")
		{
			std::cout << "   -> Find path to leaf\n";
			leaf->nodeType = Node::NodeType::FIND_PATH_TO_LEAF;

			ext::behavior_tree::FindPathToData findPathToData;
			if (CheckJsonExists(behaviorTreeJson, "destination")) {
				findPathToData.m_Destination = static_cast<ext::behavior_tree::NodeDestination>(behaviorTreeJson["destination"]);
			}
			else
			{
				findPathToData.m_Destination = ext::behavior_tree::NodeDestination::RANDOM;
			}

			leaf->m_Datas = std::make_unique<ext::behavior_tree::FindPathToData>(findPathToData);
		}

		return leaf;
	}

	std::shared_ptr<Node> BehaviorTreeEditor::AddCompositeNodeFromJson(json& behaviorTreeJson, const Node::ptr& parentNode)
	{
		std::cout << "Load Composite\n";
		auto composite = std::make_shared<Node>(nullptr, parentNode);

		ext::behavior_tree::CompositeData compositeData;

		if (behaviorTreeJson["name"] == "SequenceComposite")
		{
			std::cout << "   ->Sequence composite\n";
			composite->nodeType = Node::NodeType::SEQUENCE_COMPOSITE;
		}
		else if (behaviorTreeJson["name"] == "SelectorComposite")
		{
			composite->nodeType = Node::NodeType::SELECTOR_COMPOSITE;
		}

		if (CheckJsonExists(behaviorTreeJson, "childs"))
		{
			for (auto& childJson : behaviorTreeJson["childs"])
			{
				if (CheckJsonExists(childJson, "type"))
				{
					const NodeType nodeType = childJson["type"];

					switch (nodeType)
					{
					case NodeType::NONE:
					{
						std::ostringstream oss;
						oss << "[Error] No type specified for root node : " << childJson;
						Log::GetInstance()->Error(oss.str());
					}
					break;
					case NodeType::LEAF:
						compositeData.m_Children.push_back(AddLeafNodeFromJson(childJson, composite));
						break;
					case NodeType::COMPOSITE:
						compositeData.m_Children.push_back(AddCompositeNodeFromJson(childJson, composite));
						break;
					case NodeType::DECORATOR:
						compositeData.m_Children.push_back(AddDecoratorNodeFromJson(childJson, composite));
						break;
					default:;
					}
				}
			}
		}

		composite->m_Datas = std::make_unique<ext::behavior_tree::CompositeData>(compositeData);
		return composite;
	}

	std::shared_ptr<Node> BehaviorTreeEditor::AddDecoratorNodeFromJson(json& behaviorTreeJson, const Node::ptr& parentNode)
	{
		std::cout << "Load Decorator\n";
		auto decorator = std::make_shared<Node>(nullptr, parentNode);
		if (CheckJsonExists(behaviorTreeJson, "limit"))
		{
			ext::behavior_tree::RepeaterData repeaterData;
			if (CheckJsonExists(behaviorTreeJson, "childs"))
			{
				decorator->nodeType = Node::NodeType::REPEATER_DECORATOR;
				repeaterData.m_Limit = 0;
				for (auto& childJson : behaviorTreeJson["childs"])
				{
					if (CheckJsonExists(childJson, "type"))
					{
						const NodeType nodeType = childJson["type"];

						switch (nodeType)
						{
						case NodeType::NONE:
						{
							std::ostringstream oss;
							oss << "[Error] No type specified for root node : " << childJson;
							Log::GetInstance()->Error(oss.str());
						}
						break;
						case NodeType::LEAF:
							repeaterData.m_Child = (AddLeafNodeFromJson(childJson, decorator));
							break;
						case NodeType::COMPOSITE:
							repeaterData.m_Child = (AddCompositeNodeFromJson(childJson, decorator));
							break;
						case NodeType::DECORATOR:
							repeaterData.m_Child = (AddDecoratorNodeFromJson(childJson, decorator));
							break;
						default:;
						}
					}
				}
			}
			decorator->m_Datas = std::make_unique<ext::behavior_tree::RepeaterData>(repeaterData);
		}
		else
		{
			if (CheckJsonExists(behaviorTreeJson, "childs"))
			{
				ext::behavior_tree::DecoratorData decoratorData;
				if (CheckJsonExists(behaviorTreeJson, "childs"))
				{
					if(behaviorTreeJson["name"] == "InverterDecorator")
					{
						decorator->nodeType = Node::NodeType::INVERTER_DECORATOR;
					}else if(behaviorTreeJson["name"] == "RepeatUntilFailDecorator")
					{
						decorator->nodeType = Node::NodeType::REPEAT_UNTIL_FAIL_DECORATOR;
					}else if(behaviorTreeJson["name"] == "SucceederDecorator")
					{
						decorator->nodeType = Node::NodeType::SUCCEEDER_DECORATOR;
					}else
					{
						std::ostringstream oss;
						oss << "[Error] The node's name is unkown : " << behaviorTreeJson["name"];
						Log::GetInstance()->Error(oss.str());

						return nullptr;
					}

					for (auto& childJson : behaviorTreeJson["childs"])
					{
						if (CheckJsonExists(childJson, "type"))
						{
							const NodeType nodeType = childJson["type"];

							switch (nodeType)
							{
							case NodeType::NONE:
							{
								std::ostringstream oss;
								oss << "[Error] No type specified for root node : " << childJson;
								Log::GetInstance()->Error(oss.str());
							}
							break;
							case NodeType::LEAF:
								decoratorData.m_Child = AddLeafNodeFromJson(childJson, decorator);
								break;
							case NodeType::COMPOSITE:
								decoratorData.m_Child = AddCompositeNodeFromJson(childJson, decorator);
								break;
							case NodeType::DECORATOR:
								decoratorData.m_Child = AddDecoratorNodeFromJson(childJson, decorator);
								break;
							default:;
							}
						}
					}
				}
				decorator->m_Datas = std::make_unique<ext::behavior_tree::DecoratorData>(decoratorData);
			}
		}

		return decorator;
	}

	void BehaviorTreeEditor::DisplayNode(Node::ptr node)
	{
		/*if (ImGui::TreeNode("Trees"))
		{
			for (int i = 0; i < 5; i++) {
				if (ImGui::TreeNode((void*)(intptr_t)i, "Child %d", i))
				{
					ImGui::Text("blah blah");
					ImGui::SameLine();
					if (ImGui::SmallButton("button")) {};
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}*/
		std::string nodeName = "";
		switch(node->nodeType)
		{
		case Node::NodeType::SEQUENCE_COMPOSITE:
			nodeName += "Sequence Composite";

			if (ImGui::TreeNode(nodeName.c_str()))
			{
				for (const auto& child : static_cast<ext::behavior_tree::CompositeData*>(node->m_Datas.get())->m_Children)
				{
					DisplayNode(child);
				}
				ImGui::TreePop();
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
				DisplayNode(static_cast<ext::behavior_tree::RepeaterData*>(node->m_Datas.get())->m_Child);
				ImGui::TreePop();
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
			break;
		case Node::NodeType::MOVE_TO_LEAF:
			ImGui::Text("Move to leaf");
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
			break;
		default: ;
		}
	}
}
