#pragma once
#include "App/SimpleSound.h"
#include <memory>
#include <string>
#include <unordered_map>

class AudioManager
{
  public:
    void LoadSimpleSound(const std::string &soundName, const std::string &path);
    CSimpleSound &GetSimpleSound(const std::string &soundName);
    void Shutdown();

  private:
    std::unordered_map<std::string, std::shared_ptr<CSimpleSound>> m_soundMap;
};
