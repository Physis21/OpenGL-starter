#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 u_Model;
uniform mat4 u_CamMatrix;

void main()
{
	gl_Position = u_CamMatrix * u_Model * vec4(aPos, 1.0f);
}

#shader fragment
#version 330 core

out vec4 FragColor;

uniform vec4 u_LightColor;

void main()
{
	FragColor = u_LightColor;
}
