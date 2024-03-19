// a simple textured shader
#version 410

in vec2 vTexCoord;

uniform sampler2D colorTarget;
uniform int postProcessTarget;

out vec4 FragColour;

vec4 Default (vec2 texCoord)
{
    return texture(colorTarget, texCoord);
}

vec4 Distort (vec2 texCoord)
{
    vec2 mid = vec2(0.5f);
    float distanceFromCenterOfTheThing = distance(texCoord, mid);
    vec2 normalizedCoord = normalize(texCoord - mid);
    float bias = distanceFromCenterOfTheThing + sin(distanceFromCenterOfTheThing * 15) * 0.05f;
    vec2 newCoord = mid + bias * normalizedCoord;
    return texture(colorTarget, newCoord);
}

void main()
{
    // first we want to calculate the texel's size
    vec2 texSize = textureSize(colorTarget, 0);
    vec2 texelSize = 1.0f / texSize;

    // Set the scale
    vec2 scale = (texSize - texelSize) / texSize;
    vec2 texCoord = vTexCoord / scale + texelSize * 0.5f;

    if(postProcessTarget == 0)
    {
        FragColour = Default(texCoord);
    }
    else
    {
        FragColour = Distort(texCoord);
    }
}