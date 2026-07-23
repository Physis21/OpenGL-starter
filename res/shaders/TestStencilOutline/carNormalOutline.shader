#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 CurPos;
out vec2 TexCoords;
out vec3 Normal;

uniform mat4 u_CamMatrix;

void main()
{
	CurPos = aPos;
	TexCoords = aTexCoords;    
	Normal = aNormal;
	gl_Position = u_CamMatrix * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec3 CurPos;
in vec2 TexCoords;
in vec3 Normal;

uniform vec3 mtl_Kd;
uniform mat4 u_CamMatrix;
uniform vec3 u_CamPos;

#define THRESHOLD 0.1

void main()
{    
	vec3 viewDir = normalize(-CurPos + u_CamPos);
	float dotProduct = dot(Normal, viewDir);
	if (dotProduct < THRESHOLD) {
		FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	} else {
		FragColor = vec4(mtl_Kd, 1.0f);
	}
}
