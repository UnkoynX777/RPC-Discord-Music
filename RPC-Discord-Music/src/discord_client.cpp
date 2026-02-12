#include "discord_client.h"
#include "logger.h"
#include "config.h"
#include <iostream>
#include <cstring>

DiscordClient::DiscordClient() {
    memset(&handlers, 0, sizeof(handlers));
}

DiscordClient::~DiscordClient() {
    Shutdown();
}

void DiscordClient::Initialize() {
    handlers.ready = OnReady;
    handlers.disconnected = OnDisconnected;
    handlers.errored = OnErrored;

    Discord_Initialize(APPLICATION_ID, &handlers, 1, nullptr);
    Logger::Get().Log(LogLevel::INFO, "Discord RPC Initialized");
}

void DiscordClient::Shutdown() {
    if (connected) {
        Discord_Shutdown();
        connected = false;
        Logger::Get().Log(LogLevel::INFO, "Discord RPC Shutdown");
    }
}

void DiscordClient::RunCallbacks() {
    Discord_RunCallbacks();
}

void DiscordClient::UpdatePresence(const std::string& state, const std::string& details, int64_t startTime) {
    if (!ConfigManager::Get().Current().enabled) {
        ClearPresence();
        return;
    }

    DiscordRichPresence presence;
    memset(&presence, 0, sizeof(presence));

    presence.state = state.c_str();
    if (ConfigManager::Get().Current().showDetails) {
        presence.details = details.c_str();
    }
    
    if (startTime > 0) {
        presence.startTimestamp = startTime;
    }

    presence.largeImageKey = ConfigManager::Get().Current().largeImageKey.c_str();
    presence.largeImageText = "Listening to Music";

    Discord_UpdatePresence(&presence);
}

void DiscordClient::ClearPresence() {
    Discord_ClearPresence();
}

void DiscordClient::OnReady(const DiscordUser* request) {
    DiscordClient::Get().connected = true;
    Logger::Get().Log(LogLevel::INFO, std::string("Discord Ready: ") + request->username);
}

void DiscordClient::OnDisconnected(int errorCode, const char* message) {
    DiscordClient::Get().connected = false;
    Logger::Get().Log(LogLevel::ERR, std::string("Discord Disconnected: ") + message);
}

void DiscordClient::OnErrored(int errorCode, const char* message) {
    Logger::Get().Log(LogLevel::ERR, std::string("Discord Error: ") + message);
}
