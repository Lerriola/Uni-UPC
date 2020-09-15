#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;

out vec3 matspecfrag;
out float matshinfrag;
out vec3 matambfrag;
out vec3 matdifffrag;

out vec4 vertexSCO;
out vec3 NormSCO;

void main()
{  
    mat3 normMat = inverse(transpose(mat3(view * TG)));
    NormSCO = normMat*normal;
    vertexSCO = view*TG*vec4(vertex,1.0);

    matspecfrag = matspec;
    matshinfrag = matshin;
    matambfrag = matamb;
    matdifffrag = matdiff;

    gl_Position = proj * view * TG * vec4(vertex, 1.0);
}
