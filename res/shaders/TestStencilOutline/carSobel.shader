#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;


uniform mat4 u_CamMatrix;

void main()
{
	gl_Position = u_CamMatrix * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

uniform vec3 mtl_Kd;

void main()
{    
	// yields gradient in x
	mat3 gx = mat3(-1., 0., 1., -2., 0., 2., -1., 0., 1.);
	// yields gradient in y
	mat3 gy = mat3(-1., -2., -1., 0., 0., 0., 1., 2., 1.);
	vec3 edgeX = gx * mtl_Kd;
	vec3 edgeY = gy * mtl_Kd;
	vec3 sobel = sqrt(edgeX * edgeX + edgeY * edgeY);
	FragColor = vec4(vec3(1.0) - sobel, 1.0);
}
