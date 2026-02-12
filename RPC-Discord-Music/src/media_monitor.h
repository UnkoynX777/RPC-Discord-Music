#pragma once

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Media.Control.h>
#include <winrt/Windows.Media.h>
#include <string>
#include <functional>
#include <mutex>

using namespace winrt;
using namespace Windows::Media::Control;

struct MediaInfo {
    std::string title;
    std::string artist;
    std::string album;
    int64_t startTime;
    bool isPlaying;
};

class MediaMonitor {
public:
    static MediaMonitor& Get() {
        static MediaMonitor instance;
        return instance;
    }

    void Initialize();
    void Update();
    
    MediaInfo GetCurrentMediaInfo();
    
private:
    MediaMonitor();
    ~MediaMonitor();

    GlobalSystemMediaTransportControlsSessionManager manager{ nullptr };
    GlobalSystemMediaTransportControlsSession currentSession{ nullptr };
    
    MediaInfo currentInfo;
    std::mutex infoMutex;

    void OnSessionsChanged(GlobalSystemMediaTransportControlsSessionManager const& sender, Windows::Foundation::IInspectable const& args);
    void OnMediaPropertiesChanged(GlobalSystemMediaTransportControlsSession const& sender, MediaPropertiesChangedEventArgs const& args);
    void OnPlaybackInfoChanged(GlobalSystemMediaTransportControlsSession const& sender, PlaybackInfoChangedEventArgs const& args);

    void UpdateSession();
    void UpdateMediaProperties();
};
