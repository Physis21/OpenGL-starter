#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 texCoord;

out vec2 v_TexCoord;
out vec4 v_Normal;

uniform mat4 u_MVP;
uniform mat4 u_Model;

void main() {
   gl_Position = u_MVP * position;
   v_TexCoord = texCoord;
   v_Normal = u_Model * normal;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Normal;

uniform sampler2D u_Texture;
uniform vec4 u_LightDirection;

float phongSimple(vec4 lightDirection, vec4 normal) {
   vec3 n = normalize(normal.xyz);
   vec3 ld = normalize(lightDirection.xyz);
   float lightFactor = max(dot(n, -ld), 0.0f);
   return (lightFactor + 0.1) / 1.1;
}

void main() {
   float lightFactor = phongSimple(u_LightDirection, v_Normal);
   vec4 texColor = texture(u_Texture, v_TexCoord);
   color = vec4(texColor.xyz * lightFactor, texColor.w);
};
