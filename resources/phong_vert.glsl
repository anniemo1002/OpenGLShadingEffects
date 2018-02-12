#version  330 core
layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNor;
uniform mat4 P;
uniform mat4 MV;
uniform float Shine;
uniform vec3 MatAmb;
uniform vec3 MatSpec;
uniform vec3 MatDif;
uniform vec3 lightPos;
out vec3 fragNor;
out vec3 fragPos;
void main()
{
        vec3 new_vertNor = (MV*vec4(vertNor,0.0)).xyz;
        
        vec4 new_vertPos = MV*vertPos;
        fragPos = new_vertPos.xyz;
	gl_Position = P * new_vertPos;
        fragNor = new_vertNor;
}
