//#include <sstream>
//#include <string>
//#include <list>
//#include <set>
//
//
//#include <graphics/graphics2d.h>
//#include <graphics/shape2d.h>
//#include <utility/json_utility.h>
//#include <utility/log.h>
//#include <engine/transform2d.h>
//#include <engine/engine.h>
//#include <imgui.h>
//#include <imgui-SFML.h>
//#include "graphics/camera.h"
//
//
//namespace sfge
//{
//
//	Entity Camera::GetEntity()
//	{
//		return m_Entity;
//	}
//	void Camera::SetEntity(Entity entity)
//	{
//		m_Entity = entity;
//	}
//
//	//void CameraManager::CreateComponent(json & componentJson, Entity entity)
//	//{
//
//	//	if (CheckJsonParameter(componentJson, "path", json::value_t::string))
//	//	{
//	//		const auto path = componentJson["path"].get<std::string>();
//	//		sf::View* view = nullptr;
//	//		if (FileExists(path))
//	//		{
//	//			auto* camera = AddComponent(entity);
//	//			if (camera == nullptr)
//	//			{
//	//				std::ostringstream oss;
//	//				oss << "All sound channels used";
//	//				Log::GetInstance()->Error(oss.str());
//	//				return;
//	//			}
//	//			camera->SetEntity(entity);
//	//			const int index = camera - &m_Components[0];
//	//			auto* cameraInfo = &m_ComponentsInfo[index];
//
//	//			if (CameraId != INVALID_CAMERA_ID)
//	//			{
//	//				cameraInfo->SetEntity(entity);
//	//				cameraInfo->path = path;
//	//				view = m_SoundBufferManager->GetSoundBuffer(CameraId);
//	//				//camera->SetBuffer(soundBuffer);
//	//				cameraInfo->CameraId = cameraId;
//	//			}
//	//			else
//	//			{
//	//				std::ostringstream oss;
//	//				oss << "Sound file " << path << " cannot be loaded";
//	//				Log::GetInstance()->Error(oss.str());
//	//			}
//	//		}
//	//		else
//	//		{
//	//			std::ostringstream oss;
//	//			oss << "Sound file " << path << " does not exist";
//	//			Log::GetInstance()->Error(oss.str());
//	//		}
//	//	}
//	//	else
//	//	{
//	//		Log::GetInstance()->Error("[Error] No Path for Sound");
//	//	}
//	//}
//
//	Camera* CameraManager::AddComponent(Entity entity)
//	{
//		Camera* camera = nullptr;
//		editor::CameraInfo* cameraInfo = nullptr;
//		const int id = GetFreeComponentIndex();
//		if (id != -1)
//		{
//			camera = &m_Components[id];
//			cameraInfo = &m_ComponentsInfo[id];
//
//			cameraInfo->Camera = camera;
//			m_EntityManager->AddComponentType(entity, ComponentType::CAMERA);
//			return camera;
//		}
//		return nullptr;
//	}
//
//	void CameraManager::DestroyComponent(Entity entity)
//	{
//		(void)entity;
//	}
//
//	void Camera::Init(sf::RenderWindow& window)
//	{
//		// Apply it
//		window.setView(m_View);
//	}
//
//	sfge::Camera::Camera() : Camera(nullptr, sf::Vector2f())
//	{
//	}
//
//	sfge::Camera::Camera(Transform2d * transform, const sf::Vector2f offset) :
//		Offsetable(offset), TransformRequiredComponent(transform){
//
//	}
//
//	sfge::Camera::~Camera()
//	{
//	}
//
//
//
//
//
//	void CameraManager::Init()
//	{
//		BasicComponentManager::Init();
//		m_Components.resize(MAX_CAMERA);
//		m_ComponentsInfo.resize(MAX_CAMERA);
//	}
//	void CameraManager::Reset()
//	{
//
//	}
//
//	void CameraManager::Collect()
//	{
//
//	}
//
//	Camera *CameraManager::GetComponentPtr(Entity entity)
//	{
//		for (auto& camera : m_Components)
//		{
//			if (camera.GetEntity() == entity)
//				return &camera;
//		}
//		return nullptr;
//	}
//
//	int CameraManager::GetFreeComponentIndex()
//	{
//		for (auto i = 0u; i < MAX_CAMERA; i++)
//		{
//			if (m_Components[i].GetEntity() == INVALID_ENTITY)
//			{
//				return i;
//			}
//		}
//		return -1;
//	}
//
//
//}
//
//	void sfge::editor::CameraInfo::DrawOnInspector()
//	{
//		ImGui::Separator();
//		ImGui::Text("Camera");
//
//		ImGui::LabelText("Camera Path", "%s", path.c_str());
//		ImGui::InputInt("Camera Id", reinterpret_cast<int*>(&Camera));
//
//	}
//