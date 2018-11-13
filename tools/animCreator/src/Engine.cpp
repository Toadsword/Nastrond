#include <Engine.h>

#include <GraphicsManager.h>
#include <TextureManager.h>
#include <AnimationManager.h>

void Engine::Init()
{
	//We don't want to initialize an already running Engine
	if (isRunning)
		return;

	m_graphicsManager = new GraphicsManager();
	m_window = m_graphicsManager->Init(this);

	m_animationManager = new AnimationManager();
	m_animationManager->Init();
	
	m_textureManager = new TextureManager();
	m_textureManager->Init();

	//Create every managers.
	isRunning = true;
}

void Engine::Start()
{
	while(isRunning && m_window != nullptr)
	{
		Update();
	}
	StopEngine();
}

void Engine::Update()
{
	int dt = m_clock.getElapsedTime().asMilliseconds();
	m_graphicsManager->Update(dt);

	m_clock.restart();
}

void Engine::ExitApplication()
{
	isRunning = false;
}

void Engine::StopEngine()
{
	m_graphicsManager->Stop();
	m_window = nullptr;

	delete(m_animationManager);
	delete(m_graphicsManager);
	delete(m_textureManager);
}

GraphicsManager* Engine::GetGraphicsManager()
{
	return m_graphicsManager;
}
TextureManager* Engine::GetTextureManager()
{
	return m_textureManager;
}
AnimationManager* Engine::GetAnimationManager()
{
	return m_animationManager;
}