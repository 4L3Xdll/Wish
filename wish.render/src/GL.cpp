/*
 * Wish Framework - Lightweight retained-mode C++20 UI Framework
 * Copyright (C) discord:onlyher_alex | github:alex210114 and Contributors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "wish/GL.h"
#include <glad/glad.h>
#include <stdexcept>

//WGL extension function pointers
typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC, HGLRC, const int*);
typedef BOOL (WINAPI* PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC, const int*, const FLOAT*, UINT, int*, UINT*);

#define WGL_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB     0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB      0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB  0x00000001

namespace wish {
    bool GL::init(HWND hwnd) {
        sHwnd = hwnd;
        sHdc = GetDC(hwnd);
        if (!sHdc) {
            MessageBoxW(nullptr, L"GetDC failed", L"Wish GL Error", MB_OK);
            return false;
        }

        //step1 : set pixel format
        PIXELFORMATDESCRIPTOR pfd = {};
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;
        pfd.cDepthBits = 24;
        pfd.cStencilBits = 8;

        int pf = ChoosePixelFormat(sHdc, &pfd);
        if (!pf) {
            MessageBoxW(nullptr, L"ChoosePixelFormat failed", L"Wish GL Error", MB_OK);
            return false;
        }
        if (!SetPixelFormat(sHdc, pf, &pfd)) {
            MessageBoxW(nullptr, L"SetPixelFormat failed", L"Wish GL Error", MB_OK);
            return false;
        }

        //step2 : create temp ctx to load wgl extensions
        HGLRC tempHglrc = wglCreateContext(sHdc);
        if (!tempHglrc) {
            MessageBoxW(nullptr, L"wglCreateContext failed", L"Wish GL Error", MB_OK);
            return false;
        }
        wglMakeCurrent(sHdc, tempHglrc);

        //step3 : load wgl extension for modern ctx creation
        auto wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)
        wglGetProcAddress("wglCreateContextAttribsARB");

        if (!wglCreateContextAttribsARB) {
            sHglrc = tempHglrc;
        } else {
            //step4 : create proper opengl 4.6 core context
            int attribs[] = {
                WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
                WGL_CONTEXT_MINOR_VERSION_ARB, 6,
                WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                0
            };

            sHglrc = wglCreateContextAttribsARB(sHdc, nullptr, attribs);
            if (!sHglrc) {
                sHglrc = tempHglrc;
            } else {
                //destroy temp context, active real one
                wglMakeCurrent(nullptr, nullptr);
                wglDeleteContext(tempHglrc);
                wglMakeCurrent(sHdc, sHglrc);
            }
        }

        //step5 : load all opengl functions via glad
        if (!gladLoadGL()) {
            return false;
        }

        return true;
    }

    void GL::clear(int r, int g, int b) {
        glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glFlush();
    }

    void GL::beginFrame() {
        //ready for draw calls
    }

    void GL::endFrame() {
        SwapBuffers(sHdc);
    }

    void GL::resize(int w, int h) {
        if (!sHglrc) {
            return;
        }

        glViewport(0, 0, w, h);
    }

    void GL::shutdown() {
        wglMakeCurrent(nullptr, nullptr);
        if (sHglrc) {wglDeleteContext(sHglrc); sHglrc = nullptr;}
        if (sHdc && sHwnd) {ReleaseDC(sHwnd, sHdc); sHdc = nullptr;}
    }
}
