#version 120

varying vec2 vTexCoord;

uniform sampler2D DiffuseTexture;
        
void main() {
    gl_FragColor = texture2D(DiffuseTexture, vTexCoord);
} 