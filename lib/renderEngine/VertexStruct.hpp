/* Single translation unit to define a vertex struct.
 * Used because I have two different binaries (ObjLoader and Mesh)
 * who use it, and I want to avoid naming collision.
 */

#pragma once

#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"

// Defines all the necessary data for a vertex element in the VAO
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texcoord;
};
