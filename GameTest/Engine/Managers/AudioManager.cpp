#include "stdafx.h"
#include "AudioManager.h"

void AudioManager::LoadSimpleSound(const std::string& soundName, const std::string& path)
{
}

CSimpleSound& AudioManager::GetSimpleSound(const std::string& soundName)
{
	return *m_soundMap[soundName];
}

void AudioManager::Shutdown()
{
}
