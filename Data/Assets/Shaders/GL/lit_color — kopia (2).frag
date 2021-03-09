#version 330 core

in vec3 norm;
in vec3 fragpos;

uniform vec3 viewPos;

uniform vec3 light;  
uniform vec3 myColor;  
uniform sampler2D ourTexture;

out vec4 FragColor;

void main() {

	vec4 diffuseColor = vec4(myColor, 1.0f);

	vec3 n = normalize(norm);
	vec3 l = normalize(light);
	//float diffuseIntensity = clamp(dot(n, l) + 0.1, 0, 1);
	float diffuseIntensity = clamp(dot(n, l) + 0.1, 0, 0.65);

	//ambient
	vec4 ambientColor = diffuseColor * 0.2;

	//rim light
    vec3 viewDir = normalize(viewPos - fragpos);
	//float cosTheta = abs( dot( normalize(viewDir), n) );
	float cosTheta = abs( dot( viewDir, n) );
	float fresnel = pow(1.0 - cosTheta, 4.0);
	
	//combine
	vec4 almost = diffuseColor * (diffuseIntensity) ;
	FragColor = vec4(almost.xyz,1.0f) + (vec4(1,1,1,1) * fresnel) + ambientColor;
}