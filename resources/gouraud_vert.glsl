#version  330 core
layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNor;
uniform mat4 P;
uniform mat4 MV;
uniform float Shine;
uniform vec3 MatDif;
uniform vec3 MatAmb;
uniform vec3 MatSpec;
uniform vec3 lightPos;
out vec3 gcolor;
void main()
{
        vec3 new_vertNor = (MV*vec4(normalize(vertNor),0.0)).xyz;        
        vec4 new_vertPos = MV*vertPos;
        //vec3 Ln = normalize(lightPos - new_vertPos.xyz);
        //gcolor = MatDif*(max(0,dot(new_vertNor,Ln)))+MatAmb;
	gl_Position = P * new_vertPos;
	vec3 normal = normalize(new_vertNor);
        vec3 L =  (vec4(lightPos,0.0)).xyz- new_vertPos.xyz;
        vec3 v = (-1)*new_vertPos.xyz;
        vec3 H = normalize(normalize(L)+normalize(v));
        gcolor = MatDif*(max(0,dot(normal,normalize(L))))+(MatSpec*pow(max(0,dot(normal,H)),Shine))+MatAmb;
}
