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
Author : Fernand Battisti (battisti.fernand@gmail.com)
Date : January 2019
Project : Nastrond
*/

#ifndef FILESLOOP_H
#define FILESLOOP_H

#include <SFML/Graphics.hpp>


namespace sfge::tools
{
	/**
 * Prototype
 */
	
	class SearchInFolders;
	class ToolEngine;
	//This will allow to call the tools directly from a central executable called SFGE_EDITOR
	class FilesLoop
	{
		public:
			bool windowEnded = false;
			const char* WINDOW_NAME  = "Search Tool";
			/**
			 * \brief Constructor of FilesLoop.
			 */
			FilesLoop() {};

			/**
			 * \brief Initializes the FilesLoop, creating the required variables needed to function.
			 */
			void Init(SearchInFolders* engine);
			/*
	* \brief Called every graphic frame (dt depends on the use of VSync or not, controllable in the Configuration)
	*/
			void Update(float dt);


			void Draw();
			/**
			 * \brief Display the menu of the application.
			 */
			void DisplayMenuWindow();

			void DisplayAllFoldersAndFiles(std::vector<std::string> foldersNames, std::vector<std::string> filesNames, bool firstTime);

			void DisplayAllFoldersAndFilesInMenu(std::vector<std::string> filesN, std::vector<std::string> foldersN);

			



			
			
			sf::RenderWindow* GetWindow();

			/**
		 * \brief Initialized state of the graphics.
		 */
			bool m_IsInit = false;

			/* -------------------------------------- Others ----------------------------------- */
			/**
			 * \brief Reference to the Engine.
			 */
			SearchInFolders* m_SearchInFolders = nullptr;
			/**
			 * \brief Reference to the graphic window.
			 */
			sf::RenderWindow* m_Window = nullptr;
	};
}
#endif // ifndef FILESLOOP_H