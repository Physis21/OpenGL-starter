#shader vertex
#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aColor;
// Texture Coordinates
layout (location = 2) in vec2 aTex;
// Normals (not necessarily normalized)
layout (location = 3) in vec3 aNormal;


// Outputs the color for the Fragment Shader
out vec3 v_Color;
// Outputs the texture coordinates to the Fragment Shader
out vec2 v_TexCoord;
// Outputs the normal for the Fragment Shader
out vec3 v_Normal;
// Outputs the current position for the Fragment Shader
out vec3 v_CrntPos;

// Imports the camera matrix from the main function
uniform mat4 u_CamMatrix;
// Imports the model matrix from the main function
uniform mat4 u_Model;


void main()
{
	// calculates current position
	v_CrntPos = vec3(u_Model * vec4(aPos, 1.0f));
	// Outputs the positions/coordinates of all vertices
	gl_Position = u_CamMatrix * vec4(v_CrntPos, 1.0);

	// Assigns the colors from the Vertex Data to "color"
	v_Color = aColor;
	// Assigns the texture coordinates from the Vertex Data to "texCoord"
	v_TexCoord = aTex;
	// Assigns the normal from the Vertex Data to "Normal"
	v_Normal = aNormal;
}

#shader fragment
#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;


// Imports the color from the Vertex Shader
in vec3 v_Color;
// Imports the texture coordinates from the Vertex Shader
in vec2 v_TexCoord;
// Imports the normal from the Vertex Shader
in vec3 v_Normal;
// Imports the current position from the Vertex Shader
in vec3 v_CrntPos;

// Gets the Texture Unit from the main function
uniform sampler2D u_Tex0;
uniform sampler2D u_Tex1;
// Gets the color of the light from the main function
uniform vec4 u_LightColor;
// Gets the position of the light from the main function
uniform vec3 u_LightPos;
// Gets the position of the camera from the main function
uniform vec3 u_CamPos;

void main()
{
	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(v_Normal);
	vec3 lightDirection = normalize(u_LightPos - v_CrntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(u_CamPos - v_CrntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	// outputs final color
	// here don't forget `.r` for the specular map texture calc, as we only want the first value.
	FragColor = (texture(u_Tex0, v_TexCoord) * (diffuse + ambient) + texture(u_Tex1, v_TexCoord).r * specular) * u_LightColor ;
}
