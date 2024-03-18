// a classic phong style shader
#version 410

out vec4 FragColour;

in vec3 vNormal;

uniform vec3 ambient; // also seen as Ia
uniform vec3 diffuse; // also seen as Id
uniform vec3 specular;// also seen as Is

uniform vec3 LightDirection;

void main()
{
    vec3 N = normalize(vNormal);
    vec3 L = normalize(LightDirection);

// calculate the lambert term (this is the negative light direciton)
    float lambertTerm = max(0, min(1, dot(N, -L)));

    vec3 diffuseTotal = diffuse * lambertTerm;

    FragColour = vec4(diffuseTotal, 1);
}