/*
MIT License

Copyright (c) 2017 SAE Institute Switzerland AG

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

#include <gtest/gtest.h>

#include <engine/engine.h>
#include <engine/scene.h>
#include <engine/component.h>
#include <engine/transform2d.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "graphics/graphics2d.h"

TEST(Gizmos, GizmoUI)
{

	sfge::Engine engine;

	std::unique_ptr<sfge::Configuration> initConfig = std::make_unique<sfge::Configuration>();
	initConfig->gravity.SetZero();
	initConfig->devMode = false;
	initConfig->maxFramerate = 0;
	engine.Init(std::move(initConfig));



		// création de la fenêtre
		sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

		// on fait tourner le programme tant que la fenêtre n'a pas été fermée
		while (window.isOpen())
		{
			// on traite tous les évènements de la fenêtre qui ont été générés depuis la dernière itération de la boucle
			sf::Event event;
			while (window.pollEvent(event))
			{
				// fermeture de la fenêtre lorsque l'utilisateur le souhaite
				if (event.type == sf::Event::Closed)
					window.close();
			}

			// effacement de la fenêtre en noir
			window.clear(sf::Color::Black);

			// c'est ici qu'on dessine tout
			// window.draw(...);

			sf::VertexArray quad(sf::Quads, 4);

			// on le définit comme un rectangle, placé en (10, 10) et de taille 100x100
			quad[0].position = sf::Vector2f(40, 40);
			quad[1].position = sf::Vector2f(140, 40);
			quad[2].position = sf::Vector2f(140, 140);
			quad[3].position = sf::Vector2f(40, 140);

			quad[0].color = sf::Color::Green;
			quad[0].color = sf::Color::Green;
			quad[0].color = sf::Color::Green;
			quad[0].color = sf::Color::Green;

			window.draw(quad);

			// fin de la frame courante, affichage de tout ce qu'on a dessiné
			window.display();
		}

		/*
		void PersonalDraw() {

			auto window = m_Graphics2DManager->GetWindow();

			window->draw(quad);
		}
		*/

		/*json sceneJson = {
		{ "name", "still testing and still having no clue of what I am doing" } };

	json systemJsonGizmoUIManager = {
		{ "systemClassName", "GizmoUIManager" } };

	sceneJson["systems"] = json::array({ systemJsonGizmoUIManager });

	sfge::SceneManager* sceneManager = engine.GetSceneManager();

	sceneManager->LoadSceneFromJson(sceneJson);

	engine.Start();*/
		
}

TEST(Gizmos, GizmoBuilding)
{
	sfge::Engine engine;

	std::unique_ptr<sfge::Configuration> initConfig = std::make_unique<sfge::Configuration>();
	initConfig->gravity.SetZero();
	engine.Init(std::move(initConfig));

	const auto config = engine.GetConfig();

	json sceneJson = {
		{ "name", "testing something" } };

	json systemJsonGizmoBuildingManager = {
		{ "systemClassName", "GizmoBuildingManager" } };

	sceneJson["systems"] = json::array({ systemJsonGizmoBuildingManager });

	sfge::SceneManager* sceneManager = engine.GetSceneManager();

	sceneManager->LoadSceneFromJson(sceneJson);

	engine.Start();
}

TEST(Gizmos, GizmoDwarf)
{
	sfge::Engine engine;

	std::unique_ptr<sfge::Configuration> initConfig = std::make_unique<sfge::Configuration>();
	initConfig->gravity.SetZero();
	engine.Init(std::move(initConfig));

	const auto config = engine.GetConfig();
	

	json sceneJson = {
		{ "name", "testing once again" } };

	json systemJsonGizmoDwarfManager = {
		{ "systemClassName", "GizmoDwarfManager" } };

	sceneJson["systems"] = json::array({ systemJsonGizmoDwarfManager });

	sfge::SceneManager* sceneManager = engine.GetSceneManager();

	sceneManager->LoadSceneFromJson(sceneJson);

	engine.Start();
}