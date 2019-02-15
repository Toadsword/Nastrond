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

#include <file_selector.h>

#include <limits>
#include <imgui.h>
#include <iostream>


namespace sfge::tools {
	void FileBrowserModal::GetFilesInPath(const fs::path& path, std::vector<File>& files) {
		files.clear();

		if (path.has_parent_path()) {
			files.push_back({
				"..",
				path.parent_path()
				});
		}

		for (const auto& dirEntry : fs::directory_iterator(path)) {
			const auto& dirPath = dirEntry.path();
			files.push_back({
				dirPath.filename().string(),
				dirPath
				});
		}
	}

	int FileBrowserModal::ClampSizeTToInt(const size_t data) {
		static const int max_int = std::numeric_limits<int>::max();
		return static_cast<int>(data > max_int ? max_int : data);
	}

	bool FileBrowserModal::VectorFileItemsGetter(void* data, int idx, const char** outText) {
		const std::vector<File>* v = reinterpret_cast<std::vector<File>*>(data);
		const auto elementCount = ClampSizeTToInt(v->size());
		if (idx < 0 || idx >= elementCount) return false;
		*outText = v->at(idx).alias.data();
		return true;
	}

	FileBrowserModal::FileBrowserModal():
		m_Title("File Browser"),
		m_OldVisibility(false),
		m_Selection(0),
		m_CurrentPath(fs::current_path()),
		m_CurrentPathIsDir(true) {
	}

	FileBrowserModal::FileBrowserModal(const char* title) :
		m_Title(title),
		m_OldVisibility(false),
		m_Selection(0),
		m_CurrentPath(fs::current_path()),
		m_CurrentPathIsDir(true) {
	}

	// Will return true if file selected.
	bool FileBrowserModal::Render(const bool isVisible, std::string& outPath)
	{
		auto result = false;

		if (m_OldVisibility != isVisible) {
			m_OldVisibility = isVisible;
			//Visiblity has changed.

			if (isVisible) {
				//Only run when the visibility state changes to visible.

				//Reset the path to the initial path.
				m_CurrentPath = fs::current_path();
				m_CurrentPath = m_CurrentPath.parent_path();
				m_CurrentPath.append("data");
				m_CurrentPath.append("behavior_tree");

				m_CurrentPathIsDir = true;

				//Update paths based on current path
				GetFilesInPath(m_CurrentPath, m_FilesInScope);

				//Make the modal visible.
				ImGui::OpenPopup(m_Title);
			}

		}

		auto isOpen = true;
		if (ImGui::BeginPopupModal(m_Title, &isOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize)) {

			if (ImGui::ListBox("##", &m_Selection, VectorFileItemsGetter, &m_FilesInScope, m_FilesInScope.size(), 10)) {

				//Update current path to the selected list item.
				m_CurrentPath = m_FilesInScope[m_Selection].path;
				m_CurrentPathIsDir = fs::is_directory(m_CurrentPath);

				//If the selection is a directory, repopulate the list with the contents of that directory.
				if (m_CurrentPathIsDir) {
					GetFilesInPath(m_CurrentPath, m_FilesInScope);
				}

			}

			//Auto resize text wrap to popup width.
			ImGui::PushItemWidth(-1);
			ImGui::TextWrapped(m_CurrentPath.string().data());
			ImGui::PopItemWidth();

			ImGui::Spacing();
			ImGui::SameLine(ImGui::GetWindowWidth() - 60);

			// Make the "Select" button look / act disabled if the current selection is a directory.
			if (m_CurrentPathIsDir) {

				static const ImVec4 disabledColor = { 0.3f, 0.3f, 0.3f, 1.0f };

				ImGui::PushStyleColor(ImGuiCol_Button, disabledColor);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, disabledColor);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disabledColor);

				ImGui::Button("Select");

				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();

			}
			else {

				if (ImGui::Button("Select")) {
					ImGui::CloseCurrentPopup();

					outPath = m_CurrentPath.string();
					result = true;
				}

			}

			ImGui::EndPopup();

		}

		return result;
	}
}
