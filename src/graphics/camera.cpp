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
#include <graphics/graphics2d.h>
#include <Graphics/camera.h>
#include <utility/file_utility.h>

#include <utility/log.h>
#include <engine/engine.h>
#include <engine/config.h>
#include <engine/transform2d.h>

#include <imgui.h>
#include <imgui-SFML.h>	

namespace sfge
{
	Camera::Camera() : TransformRequiredComponent(nullptr) , Offsetable(sf::Vector2f())
	{
		
	}

	Camera::Camera(Transform2d* transform, sf::Vector2f offset, sf::Vector2f size)
	: TransformRequiredComponent(transform), Offsetable(offset)	{
		m_View.setSize(size);
	}

	float Camera::GetRotation()
	{
		return m_View.getRotation();
	}

	void Camera::SetRotation(float angle)
	{
		m_View.setRotation(angle);
	}

	Vec2f Camera::GetPosition()
	{
		return  m_View.getCenter();
	}

	void Camera::SetPosition(Vec2f position)
	{
		m_View.setCenter(position);
	}

	void Camera::Update(float dt, sf::RenderWindow& window)
	{
		//sf::Vector2u sizeWindow = window.getSize();
		//sf::Vector2i positionWindow = window.getPosition();

		//sf::Vector2f PositionCamera = GetPosition();
		//float velocityCamera = 150.0f;

		////system cursor
		//sf::Vector2i positionCursor = sf::Mouse::getPosition();
		//positionWindow = window.getPosition();

		//if (positionCursor.x >= positionWindow.x && positionCursor.x <= positionWindow.x + sizeWindow.x
		//	&& positionCursor.y >= positionWindow.y && positionCursor.y <= positionWindow.y + sizeWindow.y)
		//{

		//	if (positionCursor.y < positionWindow.y + sizeWindow.y / 100 * 10)
		//	{
		//		std::cout << PositionCamera.x << std::endl;
		//		PositionCamera -= sf::Vector2f(0, velocityCamera*dt);
		//	}

		//	if (positionCursor.y > positionWindow.y + sizeWindow.y / 100 * 90)
		//	{
		//		std::cout << PositionCamera.x << std::endl;
		//		PositionCamera += sf::Vector2f(0, velocityCamera*dt);
		//	}

		//	if (positionCursor.x < positionWindow.x + sizeWindow.x / 100 * 10)
		//	{
		//		std::cout << PositionCamera.x << std::endl;
		//		PositionCamera -= sf::Vector2f(velocityCamera*dt, 0);
		//	}

		//	if (positionCursor.x > positionWindow.x + sizeWindow.x / 100 * 90)
		//	{
		//		std::cout << PositionCamera.x << std::endl;
		//		PositionCamera += sf::Vector2f(velocityCamera*dt, 0);
		//	}
		//}
		//SetPosition(PositionCamera);
		window.setView(m_View);
	}

	CameraManager::CameraManager(Engine& engine): SingleComponentManager(engine)
	{
		
	}

	CameraManager::~CameraManager()
	{
		
	}

	Camera* CameraManager::GetMainCamera()
	{
		return &(*m_cameras.begin());
	}

	Camera* CameraManager::AddComponent(Entity entity)
	{
		auto& camera = GetComponentRef(entity);
		auto& cameraInfo = GetComponentInfo(entity);

		camera.SetTransform(m_Transform2dManager->GetComponentPtr(entity));
		cameraInfo.camera = &camera;
		m_ComponentsInfo[entity - 1].SetEntity(entity);

		m_EntityManager->AddComponentType(entity, ComponentType::CAMERA);
		return &camera;
	}

	void CameraManager::Init()
	{
		SingleComponentManager::Init();
		m_GraphicsManager = m_Engine.GetGraphics2dManager();
		m_Transform2dManager = m_Engine.GetTransform2dManager();
		m_InputManager = m_Engine.GetInputManager();
	}


	void CameraManager::Update(float dt)
	{
		rmt_ScopedCPUSample(CameraUpdate, 0)
			for (auto i = 0u; i < m_Components.size(); i++)
			{
				if (m_EntityManager->HasComponent(i + 1, ComponentType::CAMERA))
				{
					if (i == currentCamera) 
					{
						m_Components[i].Update(dt, (*m_GraphicsManager->GetWindow()));
					}
				}
			}
	}

	void CameraManager::SetCameraCurrent(short newCurrent)
	{
		currentCamera = newCurrent;
	}

	void CameraManager::Reset()
	{
	}

	void CameraManager::Collect()
	{

	}

	void CameraManager::CreateComponent(json& componentJson, Entity entity)
	{
		auto * Camera = AddComponent(entity);
		const int index = Camera - &m_Components[0];
		auto* newCameraInfo = &m_ComponentsInfo[index];

		newCameraInfo->camera = Camera;

		Camera->SetTransform(m_Transform2dManager->GetComponentPtr(entity));
	}

	void CameraManager::DestroyComponent(Entity entity)
	{
		(void)entity;
	}

	void editor::CameraInfo::DrawOnInspector()
	{
		ImGui::Separator();
		ImGui::Text("Camera");
		//ImGui::LabelText("Camera name", "%s", name.c_str());
		//ImGui::InputInt("Texture Id", (int*)&textureId);
		if (camera)
		{
			float offset[2] = {
				camera->GetOffset().x,
				camera->GetOffset().y
			};
			ImGui::InputFloat2("Offset", offset);

			float position[2] =
			{
				camera->
				
				
				
				GetPosition().x,
				camera->GetPosition().y
			};
			ImGui::InputFloat2("Position", position);
		}
	}

	sf::View Camera::GetView()
	{
		return m_View;
	}
}