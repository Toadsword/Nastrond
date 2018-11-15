#include <map>

#include <AnimationManager.h>

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

void AnimationManager::SetFPSSpeed(short newSpeed)
{
	if (newSpeed > 0.0f)
		m_animSpeed = newSpeed;
}

short AnimationManager::GetFPSSpeed()
{
	return m_animSpeed;
}

void AnimationManager::SetName(std::string newName)
{
	m_animName = newName;
}

std::string AnimationManager::GetName()
{
	return m_animName;
}

void AnimationManager::SetLooped(bool newLoop)
{
	m_looped = newLoop;
}

bool AnimationManager::GetLooped()
{
	return m_looped;
}

std::map<const short, short>& AnimationManager::GetAnim()
{
	return m_animation;
}
