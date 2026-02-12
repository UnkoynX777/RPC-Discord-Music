#pragma once
#include <string>
#include <memory>
#include "../libs/discord-rpc/include/discord_rpc.h"

class DiscordClient {
public:
    static DiscordClient& Get() {
        static DiscordClient instance;
        return instance;
    }

    void Initialize();
    void Shutdown();
    void UpdatePresence(const std::string& state, const std::string& details, int64_t startTime = 0);
    void ClearPresence();
    void RunCallbacks();

    bool IsConnected() const { return connected; }

private:
    DiscordClient();
    ~DiscordClient();

    bool connected = false;
    const char* APPLICATION_ID = "1471474478155563145";
    
    static void OnReady(const DiscordUser* request);
    static void OnDisconnected(int errorCode, const char* message);
    static void OnErrored(int errorCode, const char* message);
    
    DiscordEventHandlers handlers;
};
