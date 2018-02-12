#version 330 core 
in vec3 fragNor;
out vec4 color1;
in vec3 fragPos;

uniform vec3 MatAmb;
uniform vec3 MatDif;
uniform vec3 lightPos;
uniform float Shine;
uniform vec3 MatSpec;
void main()
{
	vec3 normal = normalize(fragNor);
        vec3 L = normalize(lightPos - fragPos);
        vec3 v = (-1)*fragPos;
        vec3 H = normalize(L+normalize(v));
        vec3 gcolor = MatDif*(max(0,dot(normal,normalize(L))))+(MatSpec*pow(max(0,dot(normal,H)),Shine))+MatAmb;
	//Map normal in the range [-1, 1] to color in range [0, 1];
	//vec3 Ncolor = 0.5*gcolor + vec3(0.5,0.5,0.5);
	color1 = vec4(gcolor, 1.0);
}
