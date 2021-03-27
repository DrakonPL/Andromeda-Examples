#version 430 core

uniform mat4 model;
uniform mat4 mvp;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec4 weights;
layout(location = 4) in ivec4 joints;

uniform mat4 pose[120];
uniform mat4 invBindPose[120];

out vec3 norm;
out vec3 fragpos;
out vec2 uv;


void main() {
	mat4 skin = (pose[joints.x] *  invBindPose[joints.x]) * weights.x;
	skin += (pose[joints.y] *  invBindPose[joints.y]) * weights.y;
	skin += (pose[joints.z] * invBindPose[joints.z]) * weights.z;
	skin += (pose[joints.w] * invBindPose[joints.w]) * weights.w;

	fragpos = vec3(model * skin * vec4(position, 1.0));
	norm = mat3(transpose(inverse(model * skin))) * normal;  
	uv = texCoord;
	
	gl_Position = mvp * skin * vec4(position, 1.0);
}
