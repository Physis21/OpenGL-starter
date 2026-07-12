#shader vertex
#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aColor;
// Texture Coordinates
layout (location = 2) in vec2 aTex;


// Outputs the color for the Fragment Shader
out vec3 v_Color;
// Outputs the texture coordinates to the fragment shader
out vec2 v_TexCoord;

// Imports the camera matrix from the main function
uniform mat4 u_CamMatrix;

void main()
{
	// Outputs the positions/coordinates of all vertices
	gl_Position = u_CamMatrix * vec4(aPos, 1.0);
	// Assigns the colors from the Vertex Data to "color"
	v_Color = aColor;
	// Assigns the texture coordinates from the Vertex Data to "texCoord"
	v_TexCoord = aTex;
}

#shader fragment
#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;


// Inputs the color from the Vertex Shader, unused here
in vec3 v_Color;
// Inputs the texture coordinates from the Vertex Shader
in vec2 v_TexCoord;

// Gets the Texture Unit from the main function
uniform sampler2D u_Texture;


void main()
{
	FragColor = texture(u_Texture, v_TexCoord);
}
