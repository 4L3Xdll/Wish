#pragma once
#include <windows.h>

namespace wish {

    //dp = density independent pixels
    //for now 1dp = 1px, later we will scale by dpi alr
    constexpr int dp(int value) {return value;}

    struct WishWindowConfig {
        const wchar_t* title = L"Wish";
        int width = 1024;
        int height = 768;
        int minWidth = 400;
        int minHeight = 300;
        bool customFrame = false;
        bool enableDpiScaling = false;
        int dragAreaHeight = dp(32);
        int cornerRadius = dp(0);
        COLORREF bgColor = RGB(255, 255, 255);
    };

    void _runWindow(const WishWindowConfig& config);
    void _applyCornerRadius(HWND hwnd, int width, int height, int radius);
}