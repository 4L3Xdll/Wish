#pragma once
#include "wish/Window.h"

//dp literals
constexpr int operator""_dp(unsigned long long v) {
    return static_cast<int>(v);
}

//wish app
struct WishApp {
    wish::WishWindowConfig config;

    WishApp& title(const wchar_t* t) {config.title = t; return *this;}
    WishApp& size(int w, int h) {config.width = w; config.height = h; return *this;}
    WishApp& minSize(int w, int h) {config.minWidth = w; config.minHeight = h; return *this;}

    //default window - windows title bar
    WishApp& defaultWindow() {
        config.customFrame = false;
        return *this;
    }

    //custom window - no title bar you draw everything
    WishApp& customWindow(int dragArea = 32_dp) {
        config.customFrame = true;
        config.dragAreaHeight = dragArea;
        return *this;
    }

    //background color
    WishApp& background(int r, int g, int b) {
        config.bgColor = RGB(r, g, b);
        return *this;
    }

    //corner radius
    WishApp& cornerRadius(int r) {
        config.cornerRadius = wish::dp(r);
        return *this;
    }

    void run() {
        wish::_runWindow(config);
    }
};

// ─── WISH_APP macro ───────────────────────────────────────────────────────────
#define WISH_APP                                                        \
void _wishMain(WishApp& App);                                       \
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {           \
WishApp app;                                                    \
_wishMain(app);                                                 \
return 0;                                                       \
}                                                                   \
void _wishMain(WishApp& App)