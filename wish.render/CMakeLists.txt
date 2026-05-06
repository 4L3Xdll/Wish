wish_module(wish.render SOURCES
        src/Renderer.cpp
        src/GL.cpp
        src/glad.cpp
    DEPS
        wish.core
)

if(WIN32)
    target_link_libraries(wish.render PUBLIC opengl32)
endif()
target_include_directories(wish.render PUBLIC include)