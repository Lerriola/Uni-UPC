#version 330 core

in vec3 vertex;
in vec3 color;
out vec3 fcolor;

uniform mat4 TG;
uniform int black;

void main()  {
	
	fcolor = color * black;
    gl_Position =  TG * vec4 (vertex, 1.0);
}
