#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 u_CamMatrix;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = u_CamMatrix * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform vec3 mtl_Kd;


void main()
{    
    FragColor = vec4(mtl_Kd, 1.0f);
}
