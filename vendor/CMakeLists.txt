include(FetchContent)

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
