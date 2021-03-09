#version 330 core

in vec3 norm;
in vec2 uv;

uniform vec3 light;  
uniform vec3 myColor;  
uniform sampler2D ourTexture;

out vec4 FragColor;

void main() {
	//vec4 diffuseColor = texture(ourTexture, uv);
	vec4 diffuseColor = vec4(myColor, 1.0f);

	vec3 n = normalize(norm);
	vec3 l = normalize(light);
	float diffuseIntensity = clamp(dot(n, l) + 0.1, 0, 1);

	FragColor = vec4((diffuseColor * diffuseIntensity).xyz,1.0f);
}