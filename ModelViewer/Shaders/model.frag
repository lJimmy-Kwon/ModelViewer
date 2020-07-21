#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform float ambientStrength;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform int shininess;


void main()
{
    //////////////////////////////////
    ////////// textures //////////////
    //////////////////////////////////
    vec4 textureColor = texture(texture_diffuse1, TexCoords);
    vec4 specMap = texture(texture_specular1, TexCoords);

    //////////////////////////////////
    ////////// vectors ///////////////
    //////////////////////////////////
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    //////////////////////////////////
    ////////// lighings //////////////
    //////////////////////////////////
    float amb = ambientStrength ;
    vec3 ambient = (amb * textureColor.rgb) * lightColor;

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * textureColor.rgb) * lightColor;

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = (specMap.rgb * spec) * lightColor;

    //////////////////////////////////
    /////////// results //////////////
    //////////////////////////////////
    vec4 result = vec4((ambient + diffuse + specular), 1.0f);
    FragColor = result;
}
