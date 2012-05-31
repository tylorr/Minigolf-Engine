#version 120

attribute vec3 VertexPosition;
attribute vec3 VertexNormal;
attribute vec2 TexCoord;

varying vec2 vTexCoord;

uniform mat4 MVP;
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

void main()
{        
	vTexCoord = TexCoord;

	// Convert position to clip coordinates and pass along
	gl_Position = MVP * vec4(VertexPosition, 1.0);
} 