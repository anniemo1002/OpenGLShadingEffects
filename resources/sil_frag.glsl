#version 330 core 
in vec3 fragNor;
in vec3 fragPos;
out vec4 color;
uniform vec3 Color;
void main()
{
	vec3 normal = normalize(fragNor);
	// Map normal in the range [-1, 1] to color in range [0, 1];
        vec3 eye = normalize((-1)*fragPos);
        float dp = dot(eye,normal);
        if (0<dp && dp<0.3){
            color = vec4(0.0,0.0,0.0,1.0);
         }
         else{
            color = vec4(1.0,1.0,1.0,1.0);
         }
	//vec3 Ncolor = 0.5*normal + 0.5;
	//color = vec4(Ncolor, 1.0);
}
