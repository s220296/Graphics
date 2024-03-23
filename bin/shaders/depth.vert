//  a normal map phong style shader
#version 410

layout( location = 0 ) in vec4 Position;

uniform mat4 ProjectionViewModel;

out vec4 vPosition; // vec4 to multiple with mat4

void main()
{
    vPosition = ProjectionViewModel * Position;

    gl_Position = ProjectionViewModel * Position;
}