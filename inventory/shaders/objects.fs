#version 430

layout (location=0) out vec4 gPosition;
layout (location=1) out vec4 gAlbedo;
layout (location=2) out vec4 gNormal;


in vec2 fragTex;
in vec3 fragNor;
in vec3 fragPos;

uniform sampler2D albedo;

void main()
{
    //vec3 toLight = normalize(-1*sun.dir);	
	//float diffuse = max(dot(toLight,fragNor),0.1);
	//vec3 diffuseColor = diffuse*sun.col;
	vec4 color =  texture(albedo,fragTex);
	if(color.a < 0.1)
		discard;
	
	gPosition = vec4(fragPos,1);
	gAlbedo = color;
	gNormal = vec4(fragNor,1);

}