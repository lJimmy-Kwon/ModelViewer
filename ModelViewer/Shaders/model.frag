#version 330
in vec4 vColor;
out vec4 fColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    fColor = texture(ourTexture, TexCoord);
//    fColor = vec4(1.0f, 0.0f, 0.0f, 1.0f );
}
