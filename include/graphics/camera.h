//#pragma once
////#ifndef SFGE_CAMERA_H
////#define SFGE_CAMERA_H
//#include <vector>
//
//#include <engine/component.h>
//#include <editor/editor_info.h>
//#include <engine/transform2d.h>
//
//namespace sfge {
//	class EntityManager;
//	class CameraManager;
//
//	using CameraId = unsigned int;
//	const CameraId INVALID_CAMERA_ID = 0U;
//
//	class Camera
//	{
//		Camera();
//		Camera(Transform2d * transform, const sf::Vector2f offset);
//		~Camera();
//		/**
//		* \brief initialize the Sound class
//		*/
//		void Init(sf::RenderWindow& window);
//
//		Entity GetEntity();
//		void SetEntity(Entity newEntity);
//	protected:
//		sf::View m_View;
//		Entity m_Entity = INVALID_ENTITY;
//	};
//
//	namespace editor
//	{
//		struct CameraInfo : ComponentInfo, PathEditorComponent
//		{
//			void DrawOnInspector() override;
//			sfge::CameraId CameraId = sfge::INVALID_CAMERA_ID;
//			sfge::Camera* Camera = nullptr;
//		};
//	}
//
//	const size_t MAX_CAMERA = 3;
//
//	class CameraManager : public SingleComponentManager<Camera, editor::CameraInfo, ComponentType::CAMERA>
//	{
//	public:
//		CameraManager(Engine& engine);
//		~CameraManager();
//
//		void Init() override;
//
//		Camera* AddComponent(Entity entity) override;
//
//		//TODO use similar construction as pycomponent
//		//void CreateComponent(json& componentJson, Entity entity) override;
//		void DestroyComponent(Entity entity) override;
//		void Reset();
//		void Collect() override;
//
//		Camera* GetComponentPtr(Entity entity) override;
//
//	protected:
//		int GetFreeComponentIndex() override;
//		//SoundBufferManager* m_SoundBufferManager = nullptr;
//		std::vector<std::unique_ptr<Camera>> m_cameras{ INIT_ENTITY_NMB };
//	};
//
//
//}
