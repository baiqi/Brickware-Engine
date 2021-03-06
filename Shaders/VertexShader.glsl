#version 330

#define pi 3.141592653589793238462643383279

//Attributes
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexCoord;

//Uniforms
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 rotationMatrix;

uniform mat4 depthBiasVP;
uniform mat4 pointDepthBiasVP;

//To be passed to the fragment shader
out vec3 worldNormal;
out vec3 worldPosition;

out vec2 texCoord;
out vec4 shadowCoord;

void main()
{
	vec4 pos4 = vec4(vPosition, 1.0f);
	mat4 worldMatrix = projectionMatrix * viewMatrix * modelMatrix;

	//Shading
	vec4 worldCoord4v = modelMatrix * pos4;
	mat3 rotation = mat3(rotationMatrix);

	worldNormal = rotation * vNormal;
	worldPosition = vec3(worldCoord4v.x / worldCoord4v.w,
		worldCoord4v.y / worldCoord4v.w,
		worldCoord4v.z / worldCoord4v.w);

	texCoord = vTexCoord;

	//Shadows
	mat4 shadowMVP = depthBiasVP * modelMatrix;
	shadowCoord = (shadowMVP * pos4);

	//Set position
	gl_Position = worldMatrix * pos4;
}
