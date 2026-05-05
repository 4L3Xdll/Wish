function(wish_module MODULE_NAME)
    cmake_parse_arguments(ARG "" "" "SOURCES;DEPS" ${ARGN})
    add_library(${MODULE_NAME} STATIC ${ARG_SOURCES})
    target_include_directories(${MODULE_NAME} PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
    )
    if(ARG_DEPS)
        target_link_libraries(${MODULE_NAME} PUBLIC ${ARG_DEPS})
    endif()
    target_compile_features(${MODULE_NAME} PUBLIC cxx_std_20)
    if(MSVC)
        target_compile_options(${MODULE_NAME} PRIVATE /W4)
    else()
        target_compile_options(${MODULE_NAME} PRIVATE -Wall -Wextra)
    endif()
    target_compile_definitions(${MODULE_NAME} PUBLIC
            $<$<CONFIG:Debug>:WISH_DEBUG>
            $<$<CONFIG:Release>:WISH_RELEASE>
    )
endfunction()

function(wish_app APP_NAME)
    cmake_parse_arguments(ARG "" "" "SOURCES;DEPS" ${ARGN})
    add_executable(${APP_NAME} WIN32 ${ARG_SOURCES})
    target_link_libraries(${APP_NAME} PRIVATE
            wish.views
            wish.platform
            wish.render
            wish.core
            ${ARG_DEPS}
    )
    target_compile_features(${APP_NAME} PUBLIC cxx_std_20)
endfunction()