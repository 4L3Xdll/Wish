wish_module(wish.platform
        SOURCES src/Window.cpp
        DEPS wish.core wish.render
)
if(WIN32)
    target_link_libraries(wish.platform PUBLIC
            user32 gdi32 dwmapi shell32 opengl32)
endif()