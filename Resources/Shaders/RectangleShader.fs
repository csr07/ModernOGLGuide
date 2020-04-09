#version 150 core
	
in vec3 Color;
in vec2 Texcoord;

out vec4 outColor;

//uniform sampler2D tex;
uniform sampler2D texKitten;
uniform sampler2D texPuppy;
//Exercise 1 Adding time for blending images
uniform float timeSin;
uniform float time;

void main()
{
	//outColor = vec4(Color, 1.0);
	//outColor = texture(tex, Texcoord);
	//outColor = texture(tex, Texcoord) * vec4(Color, 1.0);
	
	vec2 texref = Texcoord;
	
	//Exercise 2 causing reflection and water effect on the lower half of the images
	if(texref.y > 0.75)
	{
		texref.y = -texref.y + 1.5;	
	
		//Exercise 3 Distortion to simulate water	
		//texref.x = texref.x + sin(Texcoord.y *60 + time*5)/ 60.0; //from the course
		texref.x = texref.x + sin(Texcoord.y-0.75) * (sin(Texcoord.y *120 + time*10)/ 60.0); //zeromovement at reflection division
	}	
	
	vec4 colKitten = texture(texKitten, texref);
    vec4 colPuppy = texture(texPuppy, texref);
	
	
    //outColor = mix(colKitten, colPuppy, 0.5);
    outColor = mix(colKitten, colPuppy, timeSin);    
}