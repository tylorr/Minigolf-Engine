#version 120

varying vec3 LightIntensity;
        
void main() {
    gl_FragColor = vec4(LightIntensity, 1.0);
	//gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
} 