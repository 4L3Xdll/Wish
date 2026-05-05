wish_module(wish.render SOURCES
        src/Renderer.cpp
        DEPS wish.core
)

if(WIN32)
    target_link_libraries(wish.render PUBLIC opengl32)
endif()