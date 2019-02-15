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
#ifndef SFGE_TOOLS_FILE_SELECTOR_H
#define SFGE_TOOLS_FILE_SELECTOR_H

#include <vector>
#include <string>
#include <experimental/filesystem> // C++-standard header file name
#include <filesystem> // Microsoft-specific implementation header file name

// With Visual Studio compiler, filesystem is still "experimental"
namespace fs = std::experimental::filesystem;

namespace sfge::tools {

	struct File final
	{
		std::string alias;
		fs::path path;
	};

	class FileBrowserModal final {
	public:
		FileBrowserModal();
		FileBrowserModal(const char* title);

		bool Render(bool isVisible, std::string& outPath);

	private:
		static int ClampSizeTToInt(const size_t data);

		static bool VectorFileItemsGetter(void* data, int idx, const char** outText);

		static void GetFilesInPath(const fs::path& path, std::vector<File>& files);


		const char* m_Title;

		bool m_OldVisibility;

		int m_Selection;

		fs::path m_CurrentPath;
		bool m_CurrentPathIsDir;

		std::vector<File> m_FilesInScope;

	};

};
#endif