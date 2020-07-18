#include "model.h"

Model::Model(QString path)
{
    loadModel(path);
}

void Model::Draw(QOpenGLShaderProgram &program)
{
    for(unsigned int i = 0; i < meshes.size(); i++){
        meshes[i].Draw(program);
    }
}

void Model::loadModel(QString path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.toStdString(),
                                             aiProcess_Triangulate |
                                             aiProcess_FlipUVs );
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        qDebug() << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
        return;
    }

    directory = QString::fromStdString( path.toStdString().substr(0, path.toStdString().find_last_of('/')));


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
            qDebug() << mesh->mTextureCoords[0][i].x;
            qDebug() << mesh->mTextureCoords[0][i].y;

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


    return Mesh(vertices, indices, textures);
}

QVector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, QString typeName)
{
    QVector<Texture> textures;

    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++){
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;

        for(unsigned int j = 0; j < textures_loaded.size(); j++){
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

            textures.push_back(texture);
        }
    }

    return textures;
}

