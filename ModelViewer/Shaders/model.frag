#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform float ambientStrength;
uniform float specularStrength;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 viewPos;


void main()
{
    vec4 textureColor = texture(texture_diffuse1, TexCoords);
    vec4 specMap = texture(texture_specular1, TexCoords);


    vec3 ambient = 0.3 * textureColor.rgb;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragPos);

    float diff = pow(max(dot(norm, lightDir), 0.0), 8);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    vec3 specular = vec3(specMap) * spec * lightColor;

    vec4 result = vec4((ambient + diffuse + specular), 1.0f) * textureColor;

    FragColor = result;
}
