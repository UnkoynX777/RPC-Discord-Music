#pragma once
#include <string>

struct AppConfig {
    bool enabled = true;
    bool showDetails = true;
    std::string largeImageKey = "music_icon";
};

class ConfigManager {
public:
    static ConfigManager& Get() {
        static ConfigManager instance;
        return instance;
    }

    void Load();
    void Save();
    
    AppConfig& Current() { return config; }

private:
    ConfigManager();
    AppConfig config;
    std::string configPath;
};
