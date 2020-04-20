#version 150 core

layout(points) 								in;
layout(line_strip, max_vertices = 11)	 	out;

in vec3 vColor[];
out vec3 fColor;

const float PI = 3.1415926;

void main()
{	
	fColor = vColor[0]; //Point has only one vertex //set the color before Emitting any vertex

	for(int i=0; i<=10; i++)
	{
		float ang = PI * 2.0 / 10.0 * i;
		
		vec4 offset = vec4( cos(ang)*0.3, -sin(ang)*0.4, 0.0, 0.0 );
		
		gl_Position = gl_in[0].gl_Position + offset;
		
		EmitVertex();
	}
	
	EndPrimitive();
}