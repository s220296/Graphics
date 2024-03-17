// a simple flat colour shader
#version 410

out vec4 FragColour;

in vec3 vNormal;

uniform vec3 LightDirection;

void main()
{
    vec3 N = normalize(vNormal);
    vec3 L = normalize(LightDirection);

// calculate the lambert term (this is the negative light direciton)
    float lambertTerm = max(0, min(1, dot(N, -L)));

    FragColour = vec4(lambertTerm, lambertTerm, lambertTerm, 1);
}