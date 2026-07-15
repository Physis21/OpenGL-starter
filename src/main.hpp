#pragma once

#include <iostream>
#include <string>
#include "glad.h" // always include before GLFW
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

/* My libraries */
#include "GLErrorManager.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"

/* Other source files */
#include "tests/Test.hpp"
#include "tests/TestClearColor.hpp"
#include "tests/TestTexture2D.hpp"
#include "tests/TestTexture3D.hpp"
#include "tests/TestObjLoad.hpp"
#include "tests/TestCamera.hpp"
#include "tests/TestLighting.hpp"
#include "tests/TestSpecMap.hpp"
#include "tests/TestTypesOfLights.hpp"
