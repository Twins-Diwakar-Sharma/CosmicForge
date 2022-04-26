#version 430

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

struct Camera
{
	vec4 spin;
	vec3 pos;
};

uniform mat4 projection;
uniform Camera cam;

uniform sampler2D heightMap;
uniform int heightMapSize;

uniform int size;
uniform int tesselatedSize;
uniform vec2 step;
uniform vec2 position;

out vec3 fragWorldPos;

const float threshold=1.5f;


// debugs
out vec2 fragAbsSteppedObjectPos;
out float doMode;

out vec2 fragAbsSteppedObjectPosFRAG;
out float doModeFRAG;
out vec3 fragWorldPosFRAG;

vec4 quatRotate(vec4 action, vec4 victim)
{
	float ar = action.w;	float br = victim.w;
	vec3 av = action.xyz;	vec3 bv = victim.xyz;
	return vec4(ar*bv + br*av + cross(av,bv), ar*br - dot(av,bv));
}

float getHeightFromTexture(float x, float z)
{
    // rescale x,z from [-heightMapSize/2,+heightMapSize/2] in [0,1] scale
    x = x + heightMapSize/2.0;
    z = -z; // because -ve z is top
    z = z + heightMapSize/2.0;
    x = x/float(heightMapSize);
    z = z/float(heightMapSize);

    // get heightMapValue
    vec4 heightMapColor = texture(heightMap,vec2(x,z));
    float heightMapValue = heightMapColor.r;

    // rescale heightMapValue from [0,1] to [-heightMapSize/2,+heightMapSize/2]
    heightMapValue = 2*heightMapValue;
    heightMapValue = heightMapValue - 1;
    heightMapValue = heightMapValue * heightMapSize/2;

    return heightMapValue;
}

void merge(int index)
{
    vec3 objectPos = gl_in[index].gl_Position.xyz;

    vec3 worldPos =  objectPos + vec3(position.x,0,position.y);
    worldPos.y = getHeightFromTexture(worldPos.x,worldPos.z);
    
    vec2 sizedPos = objectPos.xz;
    vec2 steppedSizedPos = sizedPos + tesselatedSize*step;
    vec2 absSteppedSizedPos = abs(steppedSizedPos);
    vec2 absSteppedObjectPos = absSteppedSizedPos/float(size);
    doMode = 0;
    if(absSteppedObjectPos.y >= threshold && int(mod(absSteppedSizedPos.x,2*tesselatedSize)) == tesselatedSize)
    {
        doMode = 1;
        float alpha = (absSteppedObjectPos.y - threshold)/(2.0 - threshold);
        float heightLeft = getHeightFromTexture(worldPos.x - tesselatedSize, worldPos.z);
        float heightRight = getHeightFromTexture(worldPos.x + tesselatedSize, worldPos.z);
        float averageHeight = (heightLeft+heightRight)/2.0;
        worldPos.y = (1.0 - alpha)*worldPos.y + alpha*averageHeight;
    }
    if(absSteppedObjectPos.x >= threshold && int(mod(absSteppedSizedPos.y,2*tesselatedSize)) == tesselatedSize)
    {
        doMode = 1;
        float alpha = (absSteppedObjectPos.x - threshold)/(2.0 - threshold);
        float heightFront = getHeightFromTexture(worldPos.x, worldPos.z - tesselatedSize);
        float heightBack = getHeightFromTexture(worldPos.x, worldPos.z + tesselatedSize);
        float averageHeight = (heightFront+heightBack)/2.0;
        worldPos.y = (1.0 - alpha)*worldPos.y + alpha*averageHeight;
    }

    worldPos.y = 0;
    
	vec3 viewPos = worldPos.xyz - cam.pos;
	vec4 quatView = vec4(viewPos,0);
	vec4 spinQuat = vec4(-cam.spin.xyz, cam.spin.w);
	vec4 spinQuatInv = vec4(cam.spin);

	quatView = quatRotate(quatView, spinQuatInv);
	quatView = quatRotate(spinQuat, quatView);

	vec4 projectedPos = projection * vec4(quatView.xyz,1.0);
	gl_Position = projectedPos;

    fragWorldPos = worldPos;
    fragAbsSteppedObjectPos = abs((objectPos.xz)/float(size));

    EmitVertex();
}

void main()
{
    merge(0);
    merge(1);
    merge(2);
    EndPrimitive();
}
