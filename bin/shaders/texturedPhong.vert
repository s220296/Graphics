// a simple textured shader
#version 410

layout( location = 0 ) in vec4 Position;
layout( location = 1 ) in vec4 Normal;
layout( location = 2 ) in vec2 TexCoord;

out vec2 vTexCoord;
out vec3 vNormal;
out vec4 vPosition; // vec4 to multiple with mat4

uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;

uniform mat4 ProjectionViewModel;

void main()
{
    vPosition = ModelMatrix * Position;
    vNormal = NormalMatrix * Normal.xyz;
    vTexCoord = TexCoord;
    gl_Position = ProjectionViewModel * Position;
}