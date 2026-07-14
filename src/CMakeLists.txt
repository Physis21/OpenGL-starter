set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(OpenGL-starter)
target_sources(OpenGL-starter
    PRIVATE
        main.cpp 
        tests/Test.cpp
        tests/TestClearColor.cpp
        tests/TestTexture2D.cpp
        tests/TestTexture3D.cpp
        tests/TestObjLoad.cpp
        tests/TestCamera.cpp
        tests/TestLighting.cpp
        tests/TestSpecMap.cpp
    PRIVATE FILE_SET HEADERS FILES
        main.hpp
        tests/Test.hpp
        tests/TestClearColor.hpp
        tests/TestTexture2D.hpp
        tests/TestTexture3D.hpp
        tests/TestObjLoad.hpp
        tests/TestCamera.hpp
        tests/TestLighting.hpp
        tests/TestSpecMap.hpp
)
target_link_libraries(OpenGL-starter
    PRIVATE
        glad
        glfw
        glm::glm
        imgui
        RenderEngine
        tinyobjloader
)

