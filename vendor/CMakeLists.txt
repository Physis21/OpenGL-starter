include(FetchContent)

# Detect WSL, to set wayland support or not
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    file(READ "/proc/version" PROC_VERSION)
    if(PROC_VERSION MATCHES "Microsoft|WSL")
        set(IS_WSL TRUE)
    endif()
endif()

# Configure GLFW accordingly
if(IS_WSL)
    message(STATUS "WSL detected — disabling Wayland, enabling X11")
    # Store in the CMake cache `CMakeCache.txt` that persists between runs
    # "" is the docstring and FORCE tells to overwrite if the cache entry already exists
    set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_X11    ON  CACHE BOOL "" FORCE)
endif()

# stb library
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG        31c1ad37456438565541f4919958214b6e762fb4
)
FetchContent_MakeAvailable(stb)
# Compile stb_image into a static library via the wrapper
add_library(stb_image STATIC stb/stb_image.cpp)
# Tell the compiler where to find the header files for stb_image (-I search path flag)
target_include_directories(stb_image PUBLIC ${stb_SOURCE_DIR})

# glfw
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG 3.4)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# glad library to find OpenGL functions
add_subdirectory(glad)

# glm, OpenGL maths library
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0af55ccecd98d4e5a8d1fad7de25ba429d60e863
)
FetchContent_MakeAvailable(glm) # Now glm::glm is available

# ImGui
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui
  GIT_TAG b61e56346a92cfcaf1f43a545ca37b0b32239654 # v1.92.8-docking
)
FetchContent_MakeAvailable(imgui)
add_library(imgui)
target_include_directories(imgui # Include all header files from root and backend folder
    PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)
target_sources(imgui # include only relevant source files
    PRIVATE
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_link_libraries(imgui # imgui_impl_glfw requires glfw
    PRIVATE glfw
)
