#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform float ambientStrength;
uniform vec3 lightPosition;
uniform vec3 lightColor;

void main()
{
    vec4 textureColor = texture(texture_diffuse1, TexCoords);
    vec3 ambient = ambientStrength * textureColor.rgb;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec4 result = vec4((ambient + diffuse), 1.0f) * textureColor;

    FragColor = result;
}
