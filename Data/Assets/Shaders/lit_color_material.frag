#version 330 core

in vec3 norm;
in vec3 fragpos;

uniform vec3 matambient;
uniform vec3 matdiffuse;
uniform vec3 matspecular;    
uniform float matshininess;

uniform vec3 liposition;
uniform vec3 liambient;
uniform vec3 lidiffuse;
uniform vec3 lispecular;

uniform vec3 viewPos;

out vec4 FragColor;

void main() {
	//vec4 diffuseColor = vec4(myColor, 1.0f);

	//vec3 n = normalize(norm);
	//vec3 l = normalize(light);
	//float diffuseIntensity = clamp(dot(n, l) + 0.1, 0, 1);

	//FragColor = vec4((diffuseColor * diffuseIntensity).xyz,1.0f);
	
	
	// ambient
    vec3 ambient = liambient * matdiffuse;
  	
    // diffuse 
    vec3 normal = normalize(norm);
    vec3 lightDir = normalize(liposition - fragpos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = lidiffuse * (diff * matdiffuse);
    
    // specular
    vec3 viewDir = normalize(viewPos - fragpos);
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), matshininess);
    vec3 specular = lispecular * (spec * matspecular);  
	
        
    vec3 result = ambient + diffuse + specular ;
    FragColor = vec4(result, 1.0);
}