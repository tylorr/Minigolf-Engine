#version 120

attribute vec3 VertexPosition;
attribute vec3 VertexNormal;
        
varying vec3 LightIntensity;
        
uniform vec4 LightPosition; // Light position in eye coords.
uniform vec3 Kd; // Diffuse reflectivity
uniform vec3 Ld; // Light source intensity
        
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void getEyeSpace( out vec3 norm, out vec4 position )
{
	norm = normalize( NormalMatrix * VertexNormal);
	position = ModelViewMatrix * vec4(VertexPosition, 1.0);
}

vec3 diffuseOnly( vec4 position, vec3 norm )
{
	vec3 s = normalize(vec3(LightPosition - position));
	return Ld * Kd * max( dot(s, norm), 0.0 );
}
        
void main()
{
	vec3 eyeNorm;
	vec4 eyePosition;

	// Get the position and normal in eye space

	getEyeSpace(eyeNorm, eyePosition);

	LightIntensity = diffuseOnly(eyePosition, eyeNorm);
	//LightIntensity = Ld; // no lighting
        
	// Convert position to clip coordinates and pass along
	gl_Position = MVP * vec4(VertexPosition, 1.0);
} 