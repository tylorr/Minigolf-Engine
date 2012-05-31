#version 120

varying vTexCoord;

uniform sampler2D base;
        
void main() {
    gl_FragColor = texture2D(base, vTexCoord);
} 