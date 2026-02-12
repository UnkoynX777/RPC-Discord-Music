#include "tray_icon.h"
#include "logger.h"
#include "config.h"
#include <shellapi.h>

#define IDM_EXIT 1001
#define IDM_TOGGLE 1002

TrayIcon::TrayIcon() : hWindow(NULL) {
    memset(&nid, 0, sizeof(nid));
}

TrayIcon::~TrayIcon() {
    Shutdown();
}

bool TrayIcon::Initialize(HINSTANCE hInstance, std::function<void()> quitCallback) {
    onQuit = quitCallback;

    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("RPCMusicTray");

    if (!RegisterClassEx(&wc)) {
        Logger::Get().Log(LogLevel::ERR, "Failed to register window class");
        return false;
    }

    hWindow = CreateWindowEx(0, TEXT("RPCMusicTray"), TEXT("Discord Music RPC"), 
                             0, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);

    if (!hWindow) {
        Logger::Get().Log(LogLevel::ERR, "Failed to create window");
        return false;
    }

    nid.cbSize = sizeof(nid);
    nid.hWnd = hWindow;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    lstrcpy(nid.szTip, TEXT("Discord Music RPC"));

    Shell_NotifyIcon(NIM_ADD, &nid);
    
    Logger::Get().Log(LogLevel::INFO, "Tray Icon Initialized");
    return true;
}

void TrayIcon::Shutdown() {
    if (hWindow) {
        Shell_NotifyIcon(NIM_DELETE, &nid);
        DestroyWindow(hWindow);
        hWindow = NULL;
    }
}

void TrayIcon::UpdateTooltip(const std::string& text) {
    std::string fullText = "Discord Music RPC";
    if (!text.empty()) {
        fullText += " (" + text + ")";
    }

    if (fullText.length() >= 128) {
        fullText = fullText.substr(0, 127);
    }

    int len = MultiByteToWideChar(CP_UTF8, 0, fullText.c_str(), -1, nid.szTip, 128);
    if (len == 0) {
        lstrcpy(nid.szTip, TEXT("Discord Music RPC"));
    }

    Shell_NotifyIcon(NIM_MODIFY, &nid);
}

void TrayIcon::Update() {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        
        if (msg.message == WM_QUIT) {
            if (onQuit) onQuit();
        }
    }
}

void TrayIcon::ShowContextMenu(POINT pt) {
    HMENU hMenu = CreatePopupMenu();
    
    bool enabled = ConfigManager::Get().Current().enabled;
    AppendMenu(hMenu, MF_STRING | (enabled ? MF_CHECKED : MF_UNCHECKED), IDM_TOGGLE, TEXT("Enable RPC"));
    AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hMenu, MF_STRING, IDM_EXIT, TEXT("Exit"));

    SetForegroundWindow(hWindow);
    TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hWindow, NULL);
    DestroyMenu(hMenu);
}

LRESULT CALLBACK TrayIcon::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_TRAYICON) {
        if (lParam == WM_RBUTTONUP) {
            POINT pt;
            GetCursorPos(&pt);
            TrayIcon::Get().ShowContextMenu(pt);
        }
    } else if (msg == WM_COMMAND) {
        switch (LOWORD(wParam)) {
            case IDM_EXIT:
                PostQuitMessage(0);
                break;
            case IDM_TOGGLE:
                {
                    auto& config = ConfigManager::Get().Current();
                    config.enabled = !config.enabled;
                    ConfigManager::Get().Save();
                    
                    TrayIcon::Get().UpdateTooltip(config.enabled ? "Enabled" : "Disabled");
                }
                break;
        }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
