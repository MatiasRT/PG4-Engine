#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;

void main(){
	float colorR = 1.0f;
	float colorG = 0.0f;
	float colorB = 1.0f;
	color = texture( myTextureSampler, UV ).rgb;
	
	if (color.r == colorR && color.g == colorG && color.b == colorB){
		discard;
	} else {
		// Output color = color of the texture at the specified UV
		color = texture( myTextureSampler, UV ).rgb;
	}
}