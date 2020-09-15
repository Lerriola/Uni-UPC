#version 330 core

out vec4 FragColor;
in vec3 fcolor;

void main() {
	if(fcolor == vec3(1,0,0) && int(gl_FragCoord.x) % 10 < 4) discard;
    FragColor = vec4(fcolor,1.);
}
