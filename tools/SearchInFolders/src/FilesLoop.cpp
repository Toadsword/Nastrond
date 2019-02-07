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
Author : Fernand Battisti(battisti.fernand@gmail.com)
	Date : January 2019
	Project :Nastrond  */

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <string>
#include <engine/engine.h>
#include <input/input.h>
#include <vector>
#include "FilesLoop.h"
#include "search_in_folders.h"
#include <sys/types.h>
#include <dirent.h>

namespace sfge::tools
{
	void FilesLoop::Init(SearchInFolders* engine)
	{
		m_SearchInFolders = engine;
		m_IsInit = true;
	}


	void FilesLoop::Update(float dt)
	{
	}

	void FilesLoop::Draw()
	{
		if (!m_IsInit)
			return;

		ImGui::SetNextWindowPos(ImVec2(150.0f, 100.0f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(600.0f, 600.0f), ImGuiCond_FirstUseEver);
		if (ImGui::Begin(WINDOW_NAME, NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar))
		{
			DisplayMenuWindow();
			

		}
		ImGui::End();

	}

	void FilesLoop::DisplayMenuWindow()
	{
		if (ImGui::BeginMenu("Selection"))
		{
			std::vector<std::string> foldersNames;
			std::vector<std::string> filesNames;
				DisplayAllFoldersAndFiles(foldersNames,filesNames,true);
				ImGui::EndMenu();
			
		}
	}
	void read_directory(const std::string& name, std::vector<std::string>& filesNames, std::vector<std::string>& foldersNames)
	{
		DIR* dirp = opendir(name.c_str());
		struct dirent * dp;
		while (dirp!=nullptr&&(dp = readdir(dirp)) != nullptr)
		{
			if (dp->d_type == DT_DIR)
			{
				foldersNames.push_back(dp->d_name);
			}
			else
			{
				filesNames.push_back(dp->d_name);
			}
		}
		closedir(dirp);
	}
	void FilesLoop::DisplayAllFoldersAndFiles(std::vector<std::string> foldersNames, std::vector<std::string> filesNames,bool firstTime)
	{
		if(firstTime)
		read_directory("..", filesNames,foldersNames);
		if(ImGui::BeginCombo("Selection",""))
		{ 
		if (ImGui::BeginMenu("Files"))
		{
			if (filesNames.size() > 0)
			{
				for (size_t i = 0; i < filesNames.size(); i++)
				{
					ImGui::Text(filesNames.at(i).c_str());
				}
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Folders"))
		{
			bool closed = false;
			if (foldersNames.size() > 0)
			{

				for (size_t i = 0; i < foldersNames.size(); i++)
				{
					static const char* current_item = NULL;
					std::vector<std::string> foldersN;
					std::vector<std::string> filesN;





					bool is_selected = (current_item == foldersNames.at(i).c_str());
					if (ImGui::Selectable(foldersNames.at(i).c_str(), is_selected))
						current_item = foldersNames.at(i).c_str();

					ImGui::SetNextWindowPos(ImVec2(300.0f, 100.0f), ImGuiCond_Always);
					ImGui::SetNextWindowSize(ImVec2(600.0f, 600.0f), ImGuiCond_Always);
					if (ImGui::BeginMenu(foldersNames.at(i).c_str()))
					{
						read_directory(foldersNames.at(i), filesN, foldersN);
						DisplayAllFoldersAndFiles(foldersN, filesN, false);

						ImGui::EndMenu();
					}


				}

			}

			ImGui::EndMenu();

		}
		ImGui::EndCombo();
		}
	}

}