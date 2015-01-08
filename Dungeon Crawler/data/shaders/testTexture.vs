#version 330
layout(location = 0) in vec3 vp;
layout(location = 1) in vec2 texcoord;
out vec2 Texcoord;

void main() {
	Texcoord = texcoord;
	gl_Position = vec4(vp, 1.0);
}