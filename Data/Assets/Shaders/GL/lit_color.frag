#version 330 core

in vec3 norm;
in vec3 fragpos;

uniform vec3 viewPos;

uniform vec3 light;  
uniform vec3 myColor;  
uniform sampler2D ourTexture;

out vec4 FragColor;

void main() {

	//vec4 diffuseColor = vec4(myColor, 1.0f);

	//vec3 n = normalize(norm);
	//vec3 l = normalize(light);
	//float diffuseIntensity = clamp(dot(n, l) + 0.1, 0, 0.75);

	//ambient
	//vec4 ambientColor = diffuseColor * 0.2;

	//rim light
    //vec3 viewDir = normalize(viewPos - fragpos);
	//float cosTheta = abs( dot( viewDir, n) );
	//float fresnel = pow(1.0 - cosTheta, 4.0);
	
	//combine
	//vec4 almost = diffuseColor * (diffuseIntensity) ;
	//FragColor = vec4(almost.xyz,1.0f) + (vec4(1,1,1,1) * fresnel) + ambientColor;

	//color
	vec3 color = myColor;
	
	//ambient
	vec3 ambient = 0.5 * color;
	
	//diffuse
	vec3 lightDir = normalize(light - fragpos);
	vec3 normal = normalize(norm);
	float diff = clamp(max(dot(lightDir, normal), 0.0), 0, 0.6);
	vec3 diffuse = ambient;
	
	diffuse += diff * color;
	
	//if(diff < 0.5f)
	//{
		// Determine the final diffuse color based on the diffuse color and the amount of light intensity.
	//	diffuse += diff * color;
	//}
	
	diffuse = clamp(diffuse,0.0f,0.9f);
	
	// specular
    vec3 viewDir = normalize(viewPos - fragpos);
    vec3 reflectDir = reflect(-lightDir, normal);
	
	float spec = 0.0;
	
	vec3 halfwayDir = normalize(lightDir + viewDir);  
	spec = pow(max(dot(normal, halfwayDir), 0.0), 35.0);
	
	float cosTheta = abs( dot( viewDir, normal) );
	float fresnel = pow(1.0 - cosTheta, 4.0);
	
	vec3 specular = vec3(0.1) * spec; // assuming bright white light color
    FragColor = vec4(diffuse + specular + (vec3(1,1,1) * fresnel), 1.0) ;
}