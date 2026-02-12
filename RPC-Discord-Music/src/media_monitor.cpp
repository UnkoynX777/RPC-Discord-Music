#include "media_monitor.h"
#include "logger.h"
#include <winrt/Windows.Foundation.Collections.h>
#include <iostream>
#include <thread>

std::string to_string(winrt::hstring const& value) {
    return winrt::to_string(value);
}

MediaMonitor::MediaMonitor() {
}

MediaMonitor::~MediaMonitor() {
}

void MediaMonitor::Initialize() {
    try {
        auto operation = GlobalSystemMediaTransportControlsSessionManager::RequestAsync();
        manager = operation.get();

        if (manager) {
            Logger::Get().Log(LogLevel::INFO, "Media Session Manager acquired");
            
            manager.CurrentSessionChanged({ this, &MediaMonitor::OnSessionsChanged });
            
            UpdateSession();
        } else {
            Logger::Get().Log(LogLevel::ERR, "Failed to acquire Media Session Manager");
        }
    } catch (winrt::hresult_error const& ex) {
        Logger::Get().Log(LogLevel::ERR, "WinRT Error: " + to_string(ex.message()));
    }
}

void MediaMonitor::OnSessionsChanged(GlobalSystemMediaTransportControlsSessionManager const& sender, Windows::Foundation::IInspectable const& args) {
    UpdateSession();
}

void MediaMonitor::UpdateSession() {
    try {
        auto session = manager.GetCurrentSession();
        if (session) {
            std::lock_guard<std::mutex> lock(infoMutex);
            if (currentSession != session) {
                currentSession = session;
                Logger::Get().Log(LogLevel::INFO, "New Media Session Detected: " + to_string(session.SourceAppUserModelId()));

                currentSession.MediaPropertiesChanged({ this, &MediaMonitor::OnMediaPropertiesChanged });
                currentSession.PlaybackInfoChanged({ this, &MediaMonitor::OnPlaybackInfoChanged });
            }
        } else {
            std::lock_guard<std::mutex> lock(infoMutex);
            currentInfo.isPlaying = false;
            currentSession = nullptr;
        }
        
        UpdateMediaProperties();
        
    } catch (...) {
        Logger::Get().Log(LogLevel::ERR, "Error updating session");
    }
}

void MediaMonitor::OnMediaPropertiesChanged(GlobalSystemMediaTransportControlsSession const& sender, MediaPropertiesChangedEventArgs const& args) {
    UpdateMediaProperties();
}

void MediaMonitor::OnPlaybackInfoChanged(GlobalSystemMediaTransportControlsSession const& sender, PlaybackInfoChangedEventArgs const& args) {
    UpdateMediaProperties();
}

void MediaMonitor::UpdateMediaProperties() {
    auto session = currentSession;
    if (!session) return;

    std::thread([this, session]() {
        try {
            auto infoOp = session.TryGetMediaPropertiesAsync();
            auto info = infoOp.get();
            
            auto playback = session.GetPlaybackInfo();

            std::lock_guard<std::mutex> lock(infoMutex);
            currentInfo.title = to_string(info.Title());
            currentInfo.artist = to_string(info.Artist());
            currentInfo.album = to_string(info.AlbumArtist()); 
            
            auto status = playback.PlaybackStatus();
            currentInfo.isPlaying = (status == GlobalSystemMediaTransportControlsSessionPlaybackStatus::Playing);
            
            if (currentInfo.isPlaying) {
                 if (currentInfo.startTime == 0) {
                     currentInfo.startTime = std::time(nullptr);
                 }
            } else {
                currentInfo.startTime = 0;
            }

            Logger::Get().Log(LogLevel::INFO, "Media Update: Title='" + currentInfo.title + 
                "', Artist='" + currentInfo.artist + 
                "', Status=" + (currentInfo.isPlaying ? "Playing" : "Paused"));
            
        } catch (winrt::hresult_error const& ex) {
            Logger::Get().Log(LogLevel::ERR, "Error reading media properties: " + to_string(ex.message()));
        } catch (...) {
            Logger::Get().Log(LogLevel::ERR, "Unknown error reading media properties");
        }
    }).detach();
}

MediaInfo MediaMonitor::GetCurrentMediaInfo() {
    std::lock_guard<std::mutex> lock(infoMutex);
    return currentInfo;
}

void MediaMonitor::Update() {
    if (!currentSession) {
        UpdateSession();
    }
}
