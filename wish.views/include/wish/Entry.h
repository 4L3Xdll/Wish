#pragma once
#include "wish/Window.h"

// ─── WISH_APP ────────────────────────────────────────────────────────────────
// The entry point for every Wish application.
// Hides WinMain completely from the user.
//
// Usage:
//   #include <wish/Entry.h>
//
//   WISH_APP {
//       // your UI code here
//   }
//
#define WISH_APP                                                    \
void _wishMain();                                               \
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {       \
_wishMain();                                                \
return 0;                                                   \
}                                                               \
void _wishMain()