#include <Windows.h>
#include <winrt/base.h>

#include "logger.h"
#include "config.h"
#include "discord_client.h"
#include "media_monitor.h"
#include "tray_icon.h"

bool running = true;

void QuitApp() {
    running = false;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    winrt::init_apartment();

    Logger::Get().Log(LogLevel::INFO, "Application Starting...");
    ConfigManager::Get().Load();

    if (!TrayIcon::Get().Initialize(hInstance, QuitApp)) {
        return 1;
    }

    DiscordClient::Get().Initialize();
    MediaMonitor::Get().Initialize();

    while (running) {
        TrayIcon::Get().Update();
        
        if (running) {
            MediaMonitor::Get().Update();
            DiscordClient::Get().RunCallbacks();

            MediaInfo info = MediaMonitor::Get().GetCurrentMediaInfo();
            if (info.isPlaying) {
                DiscordClient::Get().UpdatePresence(info.artist, info.title, info.startTime);
            } else {
                DiscordClient::Get().ClearPresence();
            }

            bool isEnabled = ConfigManager::Get().Current().enabled;
            TrayIcon::Get().UpdateTooltip(isEnabled ? "Enabled" : "Disabled");
        }

        Sleep(100);
    }

    DiscordClient::Get().Shutdown();
    TrayIcon::Get().Shutdown();
    ConfigManager::Get().Save();
    
    Logger::Get().Log(LogLevel::INFO, "Application Exited Cleanly");
    return 0;
}
