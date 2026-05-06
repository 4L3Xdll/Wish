#ifndef UNICODE
#define UNICODE
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <dwmapi.h>
#include "wish/Window.h"
#include "wish/GL.h"

namespace wish {
    static WishWindowConfig g_config;

    LRESULT CALLBACK _wndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
        switch (msg) {
            case WM_DESTROY:
                wish::GL::shutdown();
                PostQuitMessage(0);
                return 0;

            case WM_NCCALCSIZE: {
                if (g_config.customFrame && wp) {
                    return 0;
                }
                return DefWindowProcW(hwnd, msg, wp, lp);
            }

            case WM_SIZE: {
                int w = GET_X_LPARAM(lp);
                int h = GET_Y_LPARAM(lp);
                wish::GL::resize(w, h);
                InvalidateRect(hwnd, nullptr, FALSE);
                _applyCornerRadius(hwnd, w, h, g_config.cornerRadius);
                return 0;
            }

            case WM_NCPAINT:
                // block DWM from painting the border
                if (g_config.customFrame) return 0;
                return DefWindowProcW(hwnd, msg, wp, lp);

            case WM_NCACTIVATE:
                // allow activation but skip default NC drawing
                if (g_config.customFrame)
                    return DefWindowProcW(hwnd, WM_NCACTIVATE, wp, -1);
                return DefWindowProcW(hwnd, msg, wp, lp);

            case WM_NCHITTEST: {
                if (!g_config.customFrame)
                    return DefWindowProcW(hwnd, msg, wp, lp);

                const LONG border = 12;
                RECT rect;
                GetWindowRect(hwnd, &rect);
                int x = GET_X_LPARAM(lp);
                int y = GET_Y_LPARAM(lp);

                // Corners
                if (x < rect.left + border && y < rect.top + border) return HTTOPLEFT;
                if (x >= rect.right - border && y < rect.top + border) return HTTOPRIGHT;
                if (x < rect.left + border && y >= rect.bottom - border) return HTBOTTOMLEFT;
                if (x >= rect.right - border && y >= rect.bottom - border) return HTBOTTOMRIGHT;

                // Edges
                if (x < rect.left + border) return HTLEFT;
                if (x >= rect.right - border) return HTRIGHT;
                if (y < rect.top + border) return HTTOP;
                if (y >= rect.bottom - border) return HTBOTTOM;

                // Drag area
                if (y < rect.top + g_config.dragAreaHeight)
                    return HTCAPTION;

                return HTCLIENT;
            }

            case WM_GETMINMAXINFO: {
                MINMAXINFO *mmi = reinterpret_cast<MINMAXINFO *>(lp);
                mmi->ptMinTrackSize.x = g_config.minWidth;
                mmi->ptMinTrackSize.y = g_config.minHeight;
                return 0;
            }

            case WM_ERASEBKGND:
                return 1;

            case WM_PAINT: {
                PAINTSTRUCT ps;
                BeginPaint(hwnd, &ps);

                //get background color components
                int r = GetRValue(g_config.bgColor);
                int g = GetGValue(g_config.bgColor);
                int b = GetBValue(g_config.bgColor);

                wish::GL::beginFrame();
                wish::GL::clear(r, g, b);
                wish::GL::endFrame();

                EndPaint(hwnd, &ps);
                return 0;
            }
        }
        return DefWindowProcW(hwnd, msg, wp, lp);
    }

    void _runWindow(const WishWindowConfig &config) {
        g_config = config;
        HINSTANCE hInstance = GetModuleHandle(nullptr);

        WNDCLASSEXW wc = {};
        wc.cbSize = sizeof(WNDCLASSEXW);
        wc.lpfnWndProc = _wndProc;
        wc.hInstance = hInstance;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.lpszClassName = L"WishWindow";
        wc.hbrBackground = nullptr;
        RegisterClassExW(&wc);

        DWORD style = config.customFrame
                          ? WS_POPUP | WS_MAXIMIZEBOX | WS_MINIMIZEBOX
                          : WS_OVERLAPPEDWINDOW;

        DWORD exStyle = config.customFrame
                            ? WS_EX_APPWINDOW
                            : 0;

        HWND hwnd = CreateWindowExW(
            exStyle,
            L"WishWindow",
            config.title,
            style,
            CW_USEDEFAULT, CW_USEDEFAULT,
            config.width, config.height,
            nullptr, nullptr,
            hInstance, nullptr
        );

        if (config.customFrame) {
            MARGINS margins = {0, 0, 0, 0};
            DwmExtendFrameIntoClientArea(hwnd, &margins);

            //corner radius
            //DWM_WINDOW_CORNER_PREFERENCE corner = config.cornerRadius > 0 ? DWMWCP_ROUND : DWMWCP_DONOTROUND;
            //DwmSetWindowAttribute(hwnd, DWMWA_WINDOW_CORNER_PREFERENCE, &corner, sizeof(corner));

            //remove border color
            COLORREF borderColor = DWMWA_COLOR_NONE;
            DwmSetWindowAttribute(hwnd, DWMWA_BORDER_COLOR, &borderColor, sizeof(borderColor));

            SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
        }

        // initialize OpenGL before showing the window because ShowWindow can send WM_SIZE
        if (!wish::GL::init(hwnd)) {
            MessageBoxW(hwnd, L"OpenGL initialization failed", L"Wish GL Error", MB_OK | MB_ICONERROR);
            DestroyWindow(hwnd);
            return;
        }
        wish::GL::resize(config.width, config.height);

        //apply radius instantly
        _applyCornerRadius(hwnd, config.width, config.height, config.cornerRadius);

        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);

        MSG msg = {};
        while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    void _applyCornerRadius(HWND hwnd, int width, int height, int radius) {
        if (config.)
       if (radius <= 0) {
           SetWindowRgn(hwnd, nullptr, TRUE);
           return;
       }

        //region uses diameter, not radius; so radius = radius * 2 = diameter
        HRGN region = CreateRoundRectRgn(0, 0, width + 1, height + 1, radius * 2, radius * 2);

        SetWindowRgn(hwnd, region, TRUE);
    }
}
