// a normal map phong style shader
#version 410

out vec4 FragColour;

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vTangent;
in vec3 vBiTangent;

// For the light source
uniform vec3 ambientLight; // also seen as Ia
uniform vec3 diffuseLight; // also seen as Id
uniform vec3 specularLight;// also seen as Is

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

uniform vec3 LightDirection;

// Model material colors
uniform vec3 Ka; // Ambient material color
uniform vec3 Kd; // Diffuse material color
uniform vec3 Ks; // Specular material color
uniform float Ns; // Specular Power

uniform vec4 CameraPosition;

const int MAX_LIGHTS = 4;
uniform int NumberOfLights;
uniform vec3 PointLightColors[MAX_LIGHTS];
uniform vec3 PointLightPositions[MAX_LIGHTS];

vec3 Diffuse(vec3 direction, vec3 color, vec3 normal)
{
    return color * max(0, dot(normal, -direction));
}

vec3 Specular(vec3 direction, vec3 color, vec3 normal, vec3 view)
{
    vec3 R = reflect(direction, normal);
    float specularTerm = pow(max(0, dot(R, view)), Ns);
    return specularTerm * color;
}

void main()
{
    vec3 N = normalize(vNormal);
    vec3 T = normalize(vTangent);
    vec3 B = normalize(vBiTangent);
    vec3 L = normalize(LightDirection);

    mat3 TBN = mat3(T,B,N);

    vec3 texDiffuse = texture( diffuseTexture, vTexCoord ).rgb;
    vec3 texSpecular = texture( specularTexture, vTexCoord ).rgb;
    vec3 texNormal = texture( normalTexture, vTexCoord ).rgb;

    N = normalize(TBN * (texNormal * 2 - 1));

// calculate the lambert term (this is the negative light direciton)
    float lambertTerm = max(0, min(1, dot(N, -L)));

// calculate the view vector
    vec3 V = normalize(CameraPosition.xyz - vPosition.xyz);

// calculate the reflection vector
    vec3 R = reflect(L, N);

    vec3 specularTotal = Specular(L, specularLight, N, V);
    vec3 diffuseTotal = Diffuse(L, diffuseLight, N);

    for (int i = 0; i < NumberOfLights; i++)
    {
        vec3 direction = vPosition.xyz - PointLightPositions[i];
        float lightDistance = length(direction);
        direction = direction / lightDistance;

        // Use the inverse square law to set the intensity of the lights
        vec3 color = PointLightColors[i] / (lightDistance * lightDistance);

        diffuseTotal += Diffuse(direction, color, N);
        specularTotal += Specular(direction, color, N, V);
    }

    vec3 ambient = ambientLight * Ka * texDiffuse;
    vec3 diffuse = Kd * texDiffuse * diffuseTotal;
    vec3 specular = Ks * texSpecular * specularTotal;

    FragColour = vec4(ambient + diffuse + specular, 1);
}