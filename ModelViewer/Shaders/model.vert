#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec4 BoneIDs;
layout (location = 4) in vec4 Weights;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;
uniform mat3 normalMatrix;
uniform mat4 gBones[100];

void main()
{

    mat4 BoneTransform = gBones[int(BoneIDs[0])] * Weights[0];
    BoneTransform += gBones[int(BoneIDs[1])] * Weights[1];
    BoneTransform += gBones[int(BoneIDs[2])] * Weights[2];
    BoneTransform += gBones[int(BoneIDs[3])] * Weights[3];

    vec4 pPos = BoneTransform * vec4(aPos, 1.0);
    vec4 pNormal = BoneTransform * vec4(aNormal, 1.0);

    TexCoords = aTexCoords;
    gl_Position = pMatrix * vMatrix * mMatrix * pPos;
    FragPos = vec3(mMatrix * pPos);
    Normal  = normalMatrix * vec3(pNormal);
}
