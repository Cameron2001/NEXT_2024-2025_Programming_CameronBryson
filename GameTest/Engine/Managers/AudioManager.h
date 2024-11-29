#pragma once
#include <unordered_map>
#include "App/SimpleSound.h"
class AudioManager
{
public:
	void LoadSimpleSound(const std::string& soundName, const std::string& path);
	CSimpleSound& GetSimpleSound(const std::string& soundName);
	void Shutdown();
private:
	std::unordered_map<std::string, CSimpleSound> m_soundMap;
};

