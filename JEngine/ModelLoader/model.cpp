#include "model.h"

Model::Model(QString path)
{
    loadModel(path);
    qDebug() << "has animation : " << scene->HasAnimations();
    m_startTime = (double)clock();
}

Model::~Model(){

    for(int i = 0; i < meshes.size(); i++){
        delete meshes[i].EBO;
        delete meshes[i].VAO;
        delete meshes[i].VBO;

        for(int j = 0; j < meshes[i].textures.size(); j++){
            delete meshes[i].textures[j].image;
        }
    }
}

void Model::Draw(QOpenGLShaderProgram &program)
{

    if(scene->HasAnimations()){

        float RunningTime = (float)((double)clock() - (double)m_startTime) / CLOCKS_PER_SEC * 10;

        program.bind();

        for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_boneLocation) ; i++) {
            char Name[128];
            memset(Name, 0, sizeof(Name));
            SNPRINTF(Name, sizeof(Name), "gBones[%d]", i);
            m_boneLocation[i] = program.uniformLocation(Name);
        }

        QVector<Matrix4f> Transforms;

        this->boneTransform(RunningTime, Transforms);

        for(uint i = 0 ; i < Transforms.size() ; i++) {
            glUniformMatrix4fv(m_boneLocation[i], 1, GL_TRUE, (const GLfloat*)Transforms[i]);
        }

        program.release();

    }

    for(int i = 0; i < meshes.size(); i++){
        meshes[i].Draw(program);
    }
}



void Model::loadModel(QString path)
{
    importer.SetPropertyBool(AI_CONFIG_IMPORT_REMOVE_EMPTY_BONES, true);
    scene = importer.ReadFile(path.toStdString(), aiProcess_Triangulate |
                               aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        qDebug() << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
        return;
    }

    directory = QString::fromStdString( path.toStdString().substr(0, path.toStdString().find_last_of('/')));

    m_GlobalInverseTransform = scene->mRootNode->mTransformation;
    m_GlobalInverseTransform.Inverse();

    processNode(scene->mRootNode, scene);

}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    for( unsigned int i = 0; i < node->mNumMeshes; i++){
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for(unsigned int i = 0; i < node-> mNumChildren; i++){
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    QVector<Vertex> vertices;
    QVector<unsigned int> indices;
    QVector<Texture> textures;

    QVector<VertexBoneData> Bones;
    Bones.resize(mesh-> mNumVertices);

    for(unsigned int i = 0; i < mesh-> mNumVertices; i++){
        Vertex vertex;
        QVector3D vector;
        vector.setX(mesh->mVertices[i].x);
        vector.setY(mesh->mVertices[i].y);
        vector.setZ(mesh->mVertices[i].z);

        vertex.Position = vector;

        vector.setX(mesh->mNormals[i].x);
        vector.setY(mesh->mNormals[i].y);
        vector.setZ(mesh->mNormals[i].z);

        vertex.Normal = vector;

        if(mesh->mTextureCoords[0]){
            QVector2D vec;
            vec.setX(mesh->mTextureCoords[0][i].x);
            vec.setY(mesh->mTextureCoords[0][i].y);
            vertex.TexCoords = vec;
        }
        else{
            vertex.TexCoords = QVector2D(0.0f, 0.0f);
        }

        vertices.push_back(vertex);


    }

    for(unsigned int i = 0; i < mesh->mNumFaces; i++){
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++){
            indices.push_back(face.mIndices[j]);
        }
    }

    if(mesh->mMaterialIndex >= 0 ){
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        QVector<Texture> diffuseMaps = loadMaterialTextures(material,
                                                            aiTextureType_DIFFUSE,
                                                            "texture_diffuse");
        textures.append(diffuseMaps);
        QVector<Texture> specularMaps = loadMaterialTextures(material,
                                                             aiTextureType_SPECULAR,
                                                             "texture_specular");
        textures.append(specularMaps);

    }


    {


        for(uint i = 0 ; i < mesh->mNumBones ; i++){
            uint BoneIndex = 0;
            QString BoneName(mesh->mBones[i]->mName.data);

            if( m_BoneMapping.find(BoneName) == m_BoneMapping.end()){
                BoneIndex = m_NumBones;
                m_NumBones++;
                BoneInfo bi;
                m_BoneInfo.push_back(bi);
            }
            else{
                BoneIndex = m_BoneMapping[BoneName];
            }

            m_BoneMapping[BoneName] = BoneIndex;
            m_BoneInfo[BoneIndex].BoneOffset = mesh->mBones[i]->mOffsetMatrix;

            for(uint j = 0 ; j < mesh->mBones[i]->mNumWeights ; j++){
                uint VertexID = mesh->mBones[i]->mWeights[j].mVertexId;
                float Weight = mesh-> mBones[i]->mWeights[j].mWeight;
                Bones[VertexID].AddBoneData(BoneIndex, Weight);
            }
        }
    }


    return Mesh(vertices, indices, textures, Bones);
}

QVector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, QString typeName)
{
    QVector<Texture> textures;

    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++){
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;

        for(int j = 0; j < textures_loaded.size(); j++){
            if(textures_loaded[j].path.compare(str.C_Str()) == 0){
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }

        if(!skip){
            Texture texture;
            texture.image = new QOpenGLTexture(QImage(directory + '/' + str.C_Str()));
            texture.type = typeName;
            texture.path = str.C_Str();
            mat->Get( AI_MATKEY_SHININESS, texture.shininess);

            //blender roughness of 0 ~ 1 maps to 0 ~ 900 of shininess when assimp imported it
            texture.shininess = (texture.shininess / 900) * 128;



            textures.push_back(texture);
        }
    }

    return textures;
}


void Model::boneTransform(float timeInSeconds, QVector<Matrix4f> &Transforms)
{
    Matrix4f Identity;
    Identity.InitIdentity();

    float TicksPerSecond = (float)(scene->mAnimations[0]->mTicksPerSecond != 0 ? scene->mAnimations[0]->mTicksPerSecond : 25.0f);
    float TimeInTicks = timeInSeconds * TicksPerSecond;
    float AnimationTime = fmod(TimeInTicks, (float)scene->mAnimations[0]->mDuration);

    ReadNodeHeirarchy(AnimationTime, scene->mRootNode, Identity);

    Transforms.resize(m_NumBones);

    for (uint i = 0 ; i < m_NumBones ; i++) {
        Transforms[i] = m_BoneInfo[i].FinalTransformation;
    }

}

void Model::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix4f& ParentTransform){
    QString NodeName(pNode->mName.data);

    const aiAnimation* pAnimation = scene->mAnimations[0];

    Matrix4f NodeTransformation(pNode->mTransformation);

    const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

    if (pNodeAnim) {
        // Interpolate scaling and generate scaling transformation matrix
        aiVector3D Scaling;
        CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
        Matrix4f ScalingM;
        ScalingM.InitScaleTransform(Scaling.x, Scaling.y, Scaling.z);

        // Interpolate rotation and generate rotation transformation matrix
        aiQuaternion RotationQ;
        CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
        Matrix4f RotationM = Matrix4f(RotationQ.GetMatrix());

        // Interpolate translation and generate translation transformation matrix
        aiVector3D Translation;
        CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
        Matrix4f TranslationM;
        TranslationM.InitTranslationTransform(Translation.x, Translation.y, Translation.z);

        // Combine the above transformations
        NodeTransformation = TranslationM * RotationM * ScalingM;
    }

    Matrix4f GlobalTransformation = ParentTransform * NodeTransformation;

    if (m_BoneMapping.find(NodeName) != m_BoneMapping.end()) {
        uint BoneIndex = m_BoneMapping[NodeName];
        m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;
    }

    for (uint i = 0 ; i < pNode->mNumChildren ; i++) {
        ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
    }

}

const aiNodeAnim* Model::FindNodeAnim(const aiAnimation* pAnimation, QString NodeName)
{
    for (uint i = 0 ; i < pAnimation->mNumChannels ; i++) {
        const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

        if (QString(pNodeAnim->mNodeName.data) == NodeName) {
            return pNodeAnim;
        }
    }

    return NULL;
}

void Model::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumScalingKeys == 1) {
        Out = pNodeAnim->mScalingKeys[0].mValue;
        return;
    }

    uint ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
    uint NextScalingIndex = (ScalingIndex + 1);
    assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
    float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
    const aiVector3D& End   = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}

uint Model::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumScalingKeys > 0);

    for (uint i = 0 ; i < pNodeAnim->mNumScalingKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
            return i;
        }
    }

    assert(0);

    return 0;
}

void Model::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    // we need at least two values to interpolate...
    if (pNodeAnim->mNumRotationKeys == 1) {
        Out = pNodeAnim->mRotationKeys[0].mValue;
        return;
    }

    uint RotationIndex = FindRotation(AnimationTime, pNodeAnim);
    uint NextRotationIndex = (RotationIndex + 1);
    assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
    float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
    const aiQuaternion& EndRotationQ   = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
    aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
    Out = Out.Normalize();
}

uint Model::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumRotationKeys > 0);

    for (uint i = 0 ; i < pNodeAnim->mNumRotationKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
            return i;
        }
    }

    assert(0);

    return 0;
}


void Model::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumPositionKeys == 1) {
        Out = pNodeAnim->mPositionKeys[0].mValue;
        return;
    }

    uint PositionIndex = FindPosition(AnimationTime, pNodeAnim);
    uint NextPositionIndex = (PositionIndex + 1);
    assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
    float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
    const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}

uint Model::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    for (uint i = 0 ; i < pNodeAnim->mNumPositionKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
            return i;
        }
    }

    assert(0);

    return 0;
}

