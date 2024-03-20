// a simple textured shader
#version 410

in vec2 vTexCoord;

uniform sampler2D colorTarget;
uniform int postProcessTarget;
uniform float fTime;

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

vec4 BoxBlur (vec2 texCoord)
{
    vec2 texel = 1.0f / textureSize(colorTarget, 0);
    vec4 color = texture(colorTarget, texCoord);

    color += texture(colorTarget, texCoord + texel * vec2(-1, 1));
    color += texture(colorTarget, texCoord + texel * vec2(-1, 0));
    color += texture(colorTarget, texCoord + texel * vec2(-1, -1));

    color += texture(colorTarget, texCoord + texel * vec2(0, 1));
    color += texture(colorTarget, texCoord + texel * vec2(0, -1));

    color += texture(colorTarget, texCoord + texel * vec2(1, 1));
    color += texture(colorTarget, texCoord + texel * vec2(1, 0));
    color += texture(colorTarget, texCoord + texel * vec2(1, -1));

    return color / 9;
}

vec4 EdgeDetection (vec2 texCoord)
{
    int edgeCount = 0; // needs to exceed 1
    float colorDifferenceThreshold = 0.1f;
    vec2 texel = 1.0f / textureSize(colorTarget, 0);
    
    vec4 color = texture(colorTarget, texCoord);
    if (distance(texture(colorTarget, texCoord + texel * vec2(-1, 1)), color) > colorDifferenceThreshold)
    { edgeCount++; }
    if (distance(texture(colorTarget, texCoord + texel * vec2(-1, 0)), color) > colorDifferenceThreshold)
    { edgeCount++; }
    if (distance(texture(colorTarget, texCoord + texel * vec2(-1, -1)), color) > colorDifferenceThreshold)
    { edgeCount++; }
    if (distance(texture(colorTarget, texCoord + texel * vec2(0, 1)), color) > colorDifferenceThreshold)
    { edgeCount++; }
    if (distance(texture(colorTarget, texCoord + texel * vec2(0, -1)), color) > colorDifferenceThreshold)
    { edgeCount++; }
    if (distance(texture(colorTarget, texCoord + texel * vec2(1, 1)), color) > colorDifferenceThreshold)
    { edgeCount++; }
    if (distance(texture(colorTarget, texCoord + texel * vec2(1, 0)), color) > colorDifferenceThreshold)
    { edgeCount++; }
    if (distance(texture(colorTarget, texCoord + texel * vec2(1, -1)), color) > colorDifferenceThreshold)
    { edgeCount++; }

    if(edgeCount > 1)
    { color = vec4(0,0,0,1); }

    return color;
}

vec4 Sepia (vec2 texCoord)
{
    vec4 color = texture(colorTarget, texCoord);
    const vec3 sepia = vec3( 0.8f, 0.58f, 0.5f);

    float sat = color.r;
    if (color.g > sat) sat = color.g;
    if (color.b > sat) sat = color.b;

    float diff = sat - 0.5f;
    sat = clamp(0.5f + diff * 1.2f, 0.1f, 0.9f);

    vec4 final = vec4(sat * sepia.r, sat * sepia.g, sat * sepia.b, color.a);

    return final;
}

vec4 Scanlines (vec2 texCoord)
{
    vec4 color = texture(colorTarget, texCoord);
    const float yMultiplier = 10.0f; // greater value = smaller lines and smaller white
    const float timeMultiplier = 3.0f; // greater value = lines move faster

    float move = sin(fTime * timeMultiplier + texCoord.y * yMultiplier);
    move += 0.9f; // greater value means more white and less grey
    move = clamp(move, 0.5f, 1.0f);

    return color * move;
}

vec4 Greyscale (vec2 texCoord)
{
    vec4 color = texture(colorTarget, texCoord);

    float sat = color.r;
    if (color.g > sat) sat = color.g;
    if (color.b > sat) sat = color.b;

    vec4 final = vec4(sat);
    final.a = 1.f;

    return final;
}

vec4 Invert (vec2 texCoord)
{
    vec4 color = texture(colorTarget, texCoord);
    vec4 final = vec4(1 - color.r, 1 - color.g, 1 - color.b, color.a);

    return final;
}

vec4 Pixilize (vec2 texCoord)
{ // Not ideal, weird lines everywhere
    const float pixelizeAmount = 10.f;
    vec2 texel = 1.0f / textureSize(colorTarget, 0);

    vec2 fullTexCoord = texCoord * textureSize(colorTarget, 0);
    
    vec2 newCoord = texCoord - texel * (mod(fullTexCoord, pixelizeAmount));

    vec4 color = texture(colorTarget, newCoord);

    return color;
}

vec4 Posterize (vec2 texCoord)
{
    const float colorCount = 4.f; // for each R, G, B

    vec4 color = texture(colorTarget, texCoord);
    float alpha = color.a;

    color *= colorCount;
    color = floor(color);
    color /= colorCount;
    color.a = alpha;

    return color;
}

void main()
{
    // first we want to calculate the texel's size
    vec2 texSize = textureSize(colorTarget, 0);
    vec2 texelSize = 1.0f / texSize;

    // Set the scale
    vec2 scale = (texSize - texelSize) / texSize;
    vec2 texCoord = vTexCoord / scale + texelSize * 0.5f;

    switch(postProcessTarget)
    {
        case 0: //Default
        FragColour = Default(texCoord);
        break;
        case 1: //Distort
        FragColour = Distort(texCoord);
        break;
        case 2: //Box Blur
        FragColour = BoxBlur(texCoord);
        break;
        case 3: //Edge Detection
        FragColour = EdgeDetection(texCoord);
        break;
        case 4: //Sepia
        FragColour = Sepia(texCoord);
        break;
        case 5: //Scanlines
        FragColour = Scanlines(texCoord);
        break;
        case 6: //Grey scale
        FragColour = Greyscale(texCoord);
        break;
        case 7: //Invert
        FragColour = Invert(texCoord);
        break;
        case 8: //Pixilizer
        FragColour = Pixilize(texCoord);
        break;
        case 9: //Posterization
        FragColour = Posterize(texCoord);
        break;
        case 10: //Distance Fog
        FragColour = Default(texCoord);
        break;
        case 11: //Depth of Field
        FragColour = Default(texCoord);
        break;
    }
}