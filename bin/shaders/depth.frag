// a normal map phong style shader
#version 410

out vec4 FragColour;

in vec4 vPosition;

uniform vec4 CameraPosition;
uniform float depthMultiplier;

void main()
{
    float depth = distance(CameraPosition, vPosition);
    depth *= depthMultiplier;
    depth = clamp(depth, 0, 1);
    // flip because further distances will be bigger values
    // I want closer things to be whiter
    //depth = 1 - depth;

    FragColour = vec4(depth, depth, depth, 1);
}