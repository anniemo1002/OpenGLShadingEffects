#version 330 core 
//in vec3 fragNor;
in vec3 gcolor;
out vec4 color1;

void main()
{
	//vec3 normal = normalize(fragNor);
	// Map normal in the range [-1, 1] to color in range [0, 1];
	//vec3 Ncolor = 0.5*normal + 0.5;
	color1 = vec4(gcolor, 1.0);
}
