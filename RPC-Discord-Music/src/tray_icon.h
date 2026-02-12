#pragma once
#include <Windows.h>
#include <functional>

#define WM_TRAYICON (WM_USER + 1)

class TrayIcon {
public:
    static TrayIcon& Get() {
        static TrayIcon instance;
        return instance;
    }

    bool Initialize(HINSTANCE hInstance, std::function<void()> quitCallback);
    void Update();
    void UpdateTooltip(const std::string& text);
    void Shutdown();

    HWND GetWindowHandle() const { return hWindow; }

private:
    TrayIcon();
    ~TrayIcon();

    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    HWND hWindow;
    NOTIFYICONDATA nid;
    std::function<void()> onQuit;
    
    void ShowContextMenu(POINT pt);
};
