#include "Camera.hpp"

Camera::Camera(int width, int height, glm::vec3 position) 
: m_Width(width), m_Height(height), m_Position(position){

}

void Camera::SetWidth(int width) {
    m_Width = width;
}
void Camera::SetHeight(int height) {
    m_Height = height;
}

glm::vec3 Camera::GetOrientation() const {
    return m_Orientation;
}

void Camera::Matrix(float fovDeg, float nearPlane, float farPlane, const Shader& shader, const char* uniform) {
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    view = glm::lookAt(m_Position, m_Position + m_Orientation, m_Up);
    projection = glm::perspective(glm::radians(fovDeg), (float)m_Width / (float)m_Height, nearPlane, farPlane);

    // Exports the camera matrix to the Vertex Shader
    glUniformMatrix4fv(glGetUniformLocation(shader.GetRendererID(), uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}

void Camera::Inputs(GLFWwindow* window) {
    // Handles key inputs
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
	m_Position += m_Speed * m_Orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
	m_Position += m_Speed * -glm::normalize(glm::cross(m_Orientation, m_Up));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
	m_Position += m_Speed * -m_Orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
	m_Position += m_Speed * glm::normalize(glm::cross(m_Orientation, m_Up));
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
	m_Position += m_Speed * m_Up;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
	m_Position += m_Speed * -m_Up;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
	m_Speed = 0.4f;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
    {
	m_Speed = 0.1f;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS) 
    {
	glfwGetCursorPos(window, &m_MouseX, &m_MouseY);
	m_OrientationBeforePress = m_Orientation;
    }
    // Handles mouse inputs
    else if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) && m_FirstClick)
    {
	glfwGetCursorPos(window, &m_MouseX, &m_MouseY);
	// Hides mouse cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	m_FirstClick = false;
	m_OrientationBeforePress = m_Orientation;
    }
    else if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) && !m_FirstClick)
    {

	double mouseXAfterPress;
	double mouseYAfterPress;
	glfwGetCursorPos(window, &mouseXAfterPress, &mouseYAfterPress);

	// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
	// and then "transforms" them into degrees 
	float rotX = m_Sensitivity * (float)(mouseYAfterPress - m_MouseY) / m_Height;
	float rotY = m_Sensitivity * (float)(mouseXAfterPress - m_MouseX) / m_Width;
	std::cout << "m_MouseY = " << m_MouseY << ", m_MouseX = " << m_MouseX << "\n";
	std::cout << "mouseYAfterPress = " << mouseYAfterPress << ", mouseXAfterPress = " << mouseXAfterPress << "\n";
	std::cout << "rotX = " << rotX << ", rotY = " << rotY << "\n";

	// Calculates upcoming vertical change in the m_Orientation
	glm::vec3 newOrientation = glm::rotate(m_OrientationBeforePress, glm::radians(-rotX), glm::normalize(glm::cross(m_OrientationBeforePress, m_Up)));

	// Decides whether or not the next vertical m_Orientation is legal or not
	if (std::abs(glm::angle(newOrientation, m_Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
	{
		m_Orientation = newOrientation;
	}

	// Rotates the m_Orientation left and right
	m_Orientation = glm::rotate(m_Orientation, glm::radians(-rotY), m_Up);
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
	// Unhides cursor since camera is not looking around anymore
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	// Makes sure the next time the camera looks around it doesn't jump
	m_FirstClick = true;
    }
}
