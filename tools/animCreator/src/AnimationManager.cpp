#include <AnimationManager.h>

#include <map>

void AnimationManager::Init()
{
	m_animation.clear();
}

bool AnimationManager::AddKey(short key)
{
	return AddKey(key, -1);
}

bool AnimationManager::AddKey(short key, short textureId)
{
	if (m_animation.find(key) == m_animation.end())
	{
		m_animation.insert(std::make_pair(key, textureId));
		return true;
	}
	return false;
}

bool AnimationManager::RemoveKey(short key)
{
	if (m_animation.find(key) != m_animation.end())
	{
		m_animation.erase(key);
		return true;
	}
	return false;
}

bool AnimationManager::SetTextureOnKey(short key, short textureId)
{
	if (m_animation.find(key) != m_animation.end())
	{
		m_animation[key] = textureId;
		return true;
	}
	return false;
}
