#ifndef UNICODE
#define UNICODE
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "wish/Window.h"

namespace wish {

    LRESULT CALLBACK _wndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
        switch (msg) {
            case WM_CLOSE:   DestroyWindow(hwnd); return 0;
            case WM_DESTROY: PostQuitMessage(0);  return 0;
        }
        return DefWindowProcW(hwnd, msg, wp, lp);
    }

    void _runWindow(const wchar_t* title, int width, int height) {
        HINSTANCE hInstance = GetModuleHandle(nullptr);

        WNDCLASSEXW wc   = {};
        wc.cbSize        = sizeof(WNDCLASSEXW);
        wc.lpfnWndProc   = _wndProc;
        wc.hInstance     = hInstance;
        wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
        wc.lpszClassName = L"WishWindow";
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        RegisterClassExW(&wc);

        HWND hwnd = CreateWindowExW(
            0, L"WishWindow", title,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            width, height,
            nullptr, nullptr, hInstance, nullptr
        );

        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);

        MSG msg = {};
        while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }
}