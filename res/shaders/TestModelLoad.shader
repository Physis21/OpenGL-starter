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

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{    
    // Show the diffuse (color) map directly. The specular map is a lighting
    // mask, not a color — it should modulate highlights once lighting is added,
    // NOT be added to the color. Adding it washed the texture out.
    FragColor = texture(texture_diffuse1, TexCoords);
}
