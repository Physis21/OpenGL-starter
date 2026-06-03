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
