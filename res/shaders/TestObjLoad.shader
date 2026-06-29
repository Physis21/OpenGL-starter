#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main() {
   gl_Position = u_MVP * position;
   v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

// Near and far could be taken as uniforms from code
float near = 0.1f;
float far = 20.0f;
float linearizeDepth(float depth) {
   return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) *(far - near));
}
float logisticDepth(float depth, float steepness, float offset) {
   float zVal = linearizeDepth(depth);
   return (1 / (1 + exp(-steepness * (zVal - offset))));
}

void main() {
   vec4 texColor = texture(u_Texture, v_TexCoord);
   float depth = logisticDepth(gl_FragCoord.z, 0.5, 5.0);
   color = vec4(vec3(texColor * (1.0 - depth)), 1.0);
};
