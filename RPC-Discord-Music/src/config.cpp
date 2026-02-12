#include "config.h"
#include <Windows.h>
#include <filesystem>

ConfigManager::ConfigManager() {
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    std::filesystem::path exePath(path);
    configPath = (exePath.parent_path() / "config.ini").string();
}

void ConfigManager::Load() {
    config.enabled = GetPrivateProfileIntA("Settings", "Enabled", 1, configPath.c_str());
    config.showDetails = GetPrivateProfileIntA("Settings", "ShowDetails", 1, configPath.c_str());
    
    char buffer[256];
    GetPrivateProfileStringA("Settings", "LargeImageKey", "music_icon", buffer, 256, configPath.c_str());
    config.largeImageKey = buffer;
}

void ConfigManager::Save() {
    WritePrivateProfileStringA("Settings", "Enabled", config.enabled ? "1" : "0", configPath.c_str());
    WritePrivateProfileStringA("Settings", "ShowDetails", config.showDetails ? "1" : "0", configPath.c_str());
    WritePrivateProfileStringA("Settings", "LargeImageKey", config.largeImageKey.c_str(), configPath.c_str());
}
