/*
 * Wish Framework - Lightweight retained-mode C++20 UI Framework
 * Copyright (C) discord:onlyher_alex | github:alex210114 and Contributors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once
#include <windows.h>

namespace wish {
    //GL
    // Internal OpenGL context manager — users never see this
    // Creates WGL context, loads all GL functions via glad
    class GL {
    public:
        //call once after Win32 window is created
        static bool init(HWND hwnd);

        //call before drawing each frame
        static void beginFrame();

        //call after drawing each frame
        static void endFrame();

        //resize viewsport
        static void resize(int w, int h);

        //cleanup
        static void shutdown();

        //clear screen to color (r, g, b 0 - 255)
        static void clear(int r, int g, int b);

    private:
        static inline HWND sHwnd = nullptr;
        static inline HDC sHdc = nullptr;
        static inline HGLRC sHglrc = nullptr;
    };
}