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
#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

//STL
#include <string>
//Dependencies
#include <SFML/Graphics.hpp>
//tool_engine
#include <engine/component.h>
#include <engine/transform2d.h>
#include <editor/editor.h>
#include <graphics/texture.h>
#include "physics/physics2d.h"

namespace sfge
{
	/**
	*\author Dylan von Arx
	*/
	class Camera
	{
	public:
		Camera& operator=(const Camera&) = delete;

		/**
		 * \brief Constructor Camera empty
		 */
		Camera();

		/**
		 * \brief Constructor Camera
		 * \param SpriteManager*, Transform2d*, sf::vector3f size
		 */
		Camera(SpriteManager* sprite_manager, Transform2d* transform, sf::Vector2f size);

		/**
		 * \brief Get camera Rotation
		 * \return float Angle
		 */
		float GetRotation();
		
		/**
		 * \brief Get a Rotation of Camera
		 * \param Float angle
		 */
		void SetRotation(float angle);
		
		/**
		 * \brief Get camera position
		 * \return Vec2f position: [x, y]
		 */
		Vec2f GetPosition();
		
		/**
		 * \brief Set camera Position
		 * \param Vec2f postion: [x, y]
		 */
		void SetPosition(Vec2f position);
		
		/**
		 * \brief Change a display size of camera
		 * \param Vec2f size: [x, y]
		 */
		void OnResize(Vec2f size);
		
		/**
		 * \brief Get Camera View
		 * \return sf::view
		 */
		sf::View GetView();
		
		void Update(float dt, sf::RenderWindow &window);

		sf::Vector2i Position;
	protected:
		sf::View m_View;
		SpriteManager* m_SpriteManager;
	};

	namespace editor
	{
		struct CameraInfo : ComponentInfo
		{
			void DrawOnInspector() override;
			Camera* camera = nullptr;
			sf::Vector2f position;
		};
	}

	class CameraManager : public SingleComponentManager<Camera, editor::CameraInfo, ComponentType::CAMERA>
	{
		const short MAINCAMERA = 0;
	public:
		using SingleComponentManager::SingleComponentManager;

		CameraManager(Engine& engine);
		~CameraManager();

		CameraManager& operator=(const Camera&) = delete;
		
		/**
		 * \brief Init System and Get Managers need
		 */
		void Init() override;
		
		/**
		 * \brief Update CameraCurrent only
		 */
		void Update(float dt) override;
		
		Camera* GetMainCamera();
		Camera* GetCameraCurrent();
		void SetCameraCurrent(short newCurrent);

		void Reset();
		void Collect() override;

		Camera* AddComponent(Entity entity) override;
		void CreateComponent(json& componentJson, Entity entity) override;
		void DestroyComponent(Entity entity) override;
	protected:
		Graphics2dManager* m_GraphicsManager;
		Transform2dManager* m_Transform2dManager;
		InputManager* m_InputManager;
		std::vector<Camera> m_cameras;
		short currentCamera = MAINCAMERA;
	};
}
#endif