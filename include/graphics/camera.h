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

namespace sfge
{
	class Camera : public TransformRequiredComponent, public Offsetable
	{
	public:
		Camera& operator=(const Camera&) = delete;

		Camera();
		Camera(Transform2d* transform, const sf::Vector2f offset, sf::Vector2f size);

		float GetRotation();
		void SetRotation(float angle);
		Vec2f GetPosition();
		void SetPosition(Vec2f position);
		sf::View GetView();

		void Update(float dt, sf::RenderWindow &window);

	protected:
		sf::View m_View;
	};
	/**
	*\author Dylan von Arx
	*/
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

		CameraManager& operator=(const Camera&) = delete;
		void Init() override;
		void Update(float dt) override;
		
		Camera* GetMainCamera();
		Camera* GetCameraCurrent();
		void SetCameraCurrent(short current);

		void Reset();
		void Collect() override;

		Camera* AddComponent(Entity entity) override;
		void CreateComponent(json& componentJson, Entity entity) override;
		void DestroyComponent(Entity entity) override;

	protected:
		Graphics2dManager* m_GraphicsManager;
		Transform2dManager* m_Transform2dManager;

		std::vector<Camera> m_cameras;
		short cameraCurrent = MAINCAMERA;
	};
}
#endif