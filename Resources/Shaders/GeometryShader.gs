#version 150 core

layout(points) 								in;
layout(line_strip, max_vertices = 2)	 	out;

in vec3 vColor[];
out vec3 fColor;

void main()
{	
	fColor = vColor[0]; //Point has only one vertex //set the color before Emitting any vertex

	gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0);
	EmitVertex();
	
	gl_Position = gl_in[0].gl_Position + vec4(0.1, 0.0, 0.0, 0.0);
	EmitVertex();
	
	EndPrimitive();
}