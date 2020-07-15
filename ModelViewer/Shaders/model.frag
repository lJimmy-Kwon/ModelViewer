#version 330
in vec4 vColor;
out vec4 fColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;

//uniform sampler2D texture_diffuse1;
//uniform sampler2D texture_diffuse2;
//uniform sampler2D texture_diffuse3;
//uniform sampler2D texture_specular1;
//uniform sampler2D texture_specular2;

void main()
{
    fColor = texture(ourTexture, TexCoord);
//    fColor = vec4(1.0f, 0.0f, 0.0f, 1.0f );
}
