#version 430 core

uniform mat4 model;
uniform mat4 mvp;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec4 weights;
layout(location = 4) in ivec4 joints;

out vec3 norm;
out vec3 fragpos;

void main() {

	fragpos = vec3(model * vec4(position, 1.0));
	norm = mat3(transpose(inverse(model))) * normal;  
	
	gl_Position = mvp * vec4(position, 1.0);
}
