#pragma once

#include "glad.h"
#include "glm/ext/quaternion_trigonometric.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"
#include <GLFW/glfw3.h>
#include "Shader.hpp"

class Camera {
    private:
        glm::vec3 m_Position;
        // Current orientation
        glm::vec3 m_Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 m_OrientationBeforePress = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::mat4 m_CameraMatrix = glm::mat4(1.0f);

        int m_Width;
        int m_Height;
        float m_Speed = 0.1f;
        float m_Sensitivity = 100.0f;
        // Prevents the camera from jumping around when first clicking left click
        bool m_FirstClick = true;
        double m_MouseX, m_MouseY;
    public:
        Camera(int width, int height, glm::vec3 position);

        void SetWidth(int width);
        void SetHeight(int height);
        glm::vec3 GetOrientation() const;
        glm::vec3 GetPosition() const;

        /* Update Camera matrix with the current camera position, orientation and up directions.
         */
        void UpdateMatrix(const float fovDeg, const float nearPlane, const float farPlane);
        /* Export the camera matrix to the specified vertex shader
         */
        void Matrix(const Shader& shader, const char* uniform);
        void Inputs(GLFWwindow* window);
};
