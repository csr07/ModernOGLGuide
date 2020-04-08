#version 150 core
	
in vec3 Color;
in vec2 Texcoord;

out vec4 outColor;

//uniform sampler2D tex;
uniform sampler2D texKitten;
uniform sampler2D texPuppy;
uniform float time;

void main()
{
	//outColor = vec4(Color, 1.0);
	//outColor = texture(tex, Texcoord);
	//outColor = texture(tex, Texcoord) * vec4(Color, 1.0);
	
	vec2 texref = Texcoord;
	
	//causing reflection on the lower half of the images
	if(texref.y > 0.5)
		texref.y = -texref.y + 1.0;
	
	vec4 colKitten = texture(texKitten, texref);
    vec4 colPuppy = texture(texPuppy, texref);
	
	
    //outColor = mix(colKitten, colPuppy, 0.5);
    outColor = mix(colKitten, colPuppy, time);
}