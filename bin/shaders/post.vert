// a simple textured shader
#version 410

layout( location = 0 ) in vec2 Position;

out vec2 vTexCoord;

void main()
{
    vTexCoord = Position * 0.5f + 0.5f;
    gl_Position = vec4( Position, 0, 1);
}