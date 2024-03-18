// a simple textured shader
#version 410

out vec4 FragColour;

in vec4 vPosition;
in vec3 vNormal;

// For the light source
uniform vec3 ambientLight; // also seen as Ia
uniform vec3 diffuseLight; // also seen as Id
uniform vec3 specularLight;// also seen as Is

uniform vec3 LightDirection;

// Model material colors
uniform vec3 Ka; // Ambient material color
uniform vec3 Kd; // Diffuse material color
uniform vec3 Ks; // Specular material color
uniform float Ns; // Specular Power

uniform vec4 CameraPosition;

in vec2 vTexCoord;

uniform sampler2D diffuseTexture;

void main()
{
    vec3 N = normalize(vNormal);
    vec3 L = normalize(LightDirection);

// calculate the lambert term (this is the negative light direciton)
    float lambertTerm = max(0, min(1, dot(N, -L)));

// calculate the view vector
    vec3 V = normalize(CameraPosition.xyz - vPosition.xyz);

// calculate the reflection vector
    vec3 R = reflect(L, N);

    float specularTerm = pow(max(0, dot(R, V)), Ns);

    vec3 ambientTotal = ambientLight * Ka;
    vec3 diffuseTotal = diffuseLight * Kd * lambertTerm;
    vec3 specularTotal = specularLight * Ks * specularTerm;

    vec4 finalColour = texture( diffuseTexture, vTexCoord );
    finalColour *= vec4(ambientTotal + diffuseTotal + specularTotal, 1);

    FragColour = finalColour;
}