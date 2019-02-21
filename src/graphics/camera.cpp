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
#include "graphics/quadtree.h"

namespace sfge
{
	Camera::Camera()
	{
	}

	Camera::Camera(Transform2d* transform, sf::Vector2f size){
		m_View.setSize(size+sf::Vector2f(5000,5000));
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
		m_View.setCenter(sf::Vector2f(position.x, position.y));
	}

	void Camera::Update(float dt, sf::RenderWindow& window)
	{
		window.setView(m_View);

		sf::VertexArray triangle(sf::LineStrip, 5);
		triangle[0].position = sf::Vector2f(m_View.getCenter().x - m_View.getSize().x / 4, m_View.getCenter().y - m_View.getSize().y / 4);
		triangle[1].position = sf::Vector2f(m_View.getCenter().x + m_View.getSize().x / 4, m_View.getCenter().y - m_View.getSize().y / 4);
		triangle[2].position = sf::Vector2f(m_View.getCenter().x + m_View.getSize().x / 4, m_View.getCenter().y + m_View.getSize().y / 4);
		triangle[3].position = sf::Vector2f(m_View.getCenter().x - m_View.getSize().x / 4, m_View.getCenter().y + m_View.getSize().y / 4);
		triangle[4].position = sf::Vector2f(m_View.getCenter().x - m_View.getSize().x / 4, m_View.getCenter().y - m_View.getSize().y / 4);
		window.draw(triangle);
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

	Camera* CameraManager::
	AddComponent(Entity entity)
	{
		auto& camera = GetComponentRef(entity);
		auto& cameraInfo = GetComponentInfo(entity);

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
		//m_OcclusionManager = m_Engine.GetGraphics2dManager()->GetSpriteManager()->GetQuadtreeManager();
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
						
						sf::RenderWindow* win = m_GraphicsManager->GetWindow();
						Vec2f position = m_Components[i].GetPosition();
						Vec2f size = m_Components[i].GetView().getSize();
						size.x /= 4;
						size.y /= 4;
						if(aabbCamera.centre.position.x != position.x || aabbCamera.centre.position.y != position.y)
						{
								aabbCamera = AABBHash(PointHash(nullptr, sf::Vector2f(position)), PointHash(nullptr, sf::Vector2f(size)));
								m_Engine.GetGraphics2dManager()->GetSpriteManager()->GetHashTable()->CheckCamera(aabbCamera);

						}

						//if (aabbCamera.centre.x != position.x || aabbCamera.centre.y != position.y) {
						//	aabbCamera = AABBOcc(nullptr, PointOcc(position.x, position.y), PointOcc(size.x, size.y));
						//	//m_OcclusionManager->CheckCamera(aabbCamera);
						//}
						//m_OcclusionManager->Draw(win);
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
		Camera->SetPosition(sf::Vector2f(0.0f, 0.0f));
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
			float position[2] =
			{
				camera->GetPosition().x,
				camera->GetPosition().y
			};
		    ImGui::InputFloat2("Position", position);
		}
	}

	sf::View Camera::GetView()
	{
		return m_View;
	}

	void Camera::OnResize(Vec2f size)
	{
		float x = size.x;
		float y = size.y;
		m_View.reset(sf::FloatRect(0, 0, x , y));
	}
}
