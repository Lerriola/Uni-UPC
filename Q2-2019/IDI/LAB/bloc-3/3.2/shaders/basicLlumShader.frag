#version 330 core


uniform vec4 posFocus;
uniform vec3 colFocus;
uniform vec3 llumAmbient;

in vec3 matspecfrag;
in float matshinfrag;
in vec3 matambfrag;
in vec3 matdifffrag;

in vec4 vertexSCO;
in vec3 NormSCO;

out vec4 FragColor;


vec3 Lambert (vec3 NormSCO, vec3 L) 
{
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats

    // Inicialitzem color a component ambient
    vec3 colRes = llumAmbient * matambfrag;

    // Afegim component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colRes + colFocus * matdifffrag * dot (L, NormSCO);
    return (colRes);
}

vec3 Phong (vec3 NormSCO, vec3 L, vec4 vertSCO) 
{
    // Els vectors estan normalitzats

    // Inicialitzem color a Lambert
    vec3 colRes = Lambert (NormSCO, L);

    // Calculem R i V
    if ((dot(NormSCO,L) < 0) || (matshinfrag == 0))
      return colRes;  // no hi ha component especular

    vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
    vec3 V = normalize(-vertSCO.xyz); // perquè la càmera està a (0,0,0) en SCO

    if (dot(R, V) < 0)
      return colRes;  // no hi ha component especular
    
    float shine = pow(max(0.0, dot(R, V)), matshinfrag);
    return (colRes + matspecfrag * colFocus * shine); 
}

void main()
{	
	vec3 NormSCON = normalize(NormSCO);
	vec3 LSCON = normalize(posFocus.xyz-vertexSCO.xyz);
	FragColor = vec4(Phong(NormSCON,LSCON,vertexSCO),1);	
}
