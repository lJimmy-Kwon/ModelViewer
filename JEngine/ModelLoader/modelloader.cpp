#include "modelloader.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>


ModelLoader::ModelLoader()
{

}

bool ModelLoader::Load(QString filePath)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filePath.toStdString(),
            aiProcess_GenSmoothNormals      |
            aiProcess_CalcTangentSpace      |
            aiProcess_Triangulate           |
            aiProcess_JoinIdenticalVertices |
            aiProcess_SortByPType );

    if(!scene)
    {
        qDebug() << "Error loading file: (assimp:) " << importer.GetErrorString();
        return false;
    }

    if(scene->HasMaterials())
    {
        qDebug() << "Material loading..";
        for(unsigned int ii=0; ii<scene->mNumMaterials; ++ii)
        {
            QSharedPointer<MaterialInfo> mater = processMaterial(scene->mMaterials[ii]);
            m_materials.push_back(mater);
        }
    }

    if(scene->HasMeshes())
    {
        for(unsigned int ii=0; ii<scene->mNumMeshes; ++ii)
        {
            m_meshes.push_back(processMesh(scene->mMeshes[ii]));
        }
    }

    else
    {
        qDebug() << "Error: No meshes found";
        return false;
    }

    return true;
}

void ModelLoader::getBufferData(QVector<float> **vertices, QVector<float> **normals, QVector<unsigned int> **indices)
{
    if(vertices != 0)
        *vertices = &m_vertices;

    if(normals != 0)
        *normals = &m_normals;

    if(indices != 0)
        *indices = &m_indices;
}

QSharedPointer<MaterialInfo> ModelLoader::processMaterial(aiMaterial *material)
{
    QSharedPointer<MaterialInfo> mater(new MaterialInfo);

    aiString mname;
    material->Get( AI_MATKEY_NAME, mname);
    if(mname.length > 0)
        mater->Name = mname.C_Str();

    int shadingModel;
    material->Get( AI_MATKEY_SHADING_MODEL, shadingModel );

    qDebug() << "shadingModel " << shadingModel;


    if(shadingModel != aiShadingMode_Phong && shadingModel != aiShadingMode_Gouraud)
    {
        qDebug() << "This mesh's shading model is not implemented in this loader, setting to default material";
        mater->Name = "DefaultMaterial";
    }
    else
    {
        aiColor3D dif (0.f,0.f,0.f);
        aiColor3D amb (0.f,0.f,0.f);
        aiColor3D spec (0.f,0.f,0.f);
        float shine = 0.0;

        material->Get( AI_MATKEY_COLOR_AMBIENT, amb);
        material->Get( AI_MATKEY_COLOR_DIFFUSE, dif); //->Get(<material-key>,<where-to-store>))
        material->Get( AI_MATKEY_COLOR_SPECULAR, spec);
        material->Get( AI_MATKEY_SHININESS, shine);

        mater->Ambient  = QVector3D(amb.r, amb.g, amb.b);
        mater->Diffuse  = QVector3D(dif.r, dif.g, dif.b);
        mater->Specular = QVector3D(spec.r, spec.g, spec.b);
        mater->Shininess = shine;

        qDebug() << "Ambient " << mater->Ambient;
        qDebug() << "Diffuse " << mater->Diffuse;
        qDebug() << "Specular " << mater->Specular;

        mater->Ambient *= .2f;
        if( mater->Shininess == 0.0) mater->Shininess = 30;
    }

    return mater;
}

QSharedPointer<Mesh> ModelLoader::processMesh(aiMesh *mesh)
{
    QSharedPointer<Mesh> newMesh(new Mesh);
    newMesh->name = mesh->mName.length != 0 ? mesh->mName.C_Str() : "";
    newMesh->indexOffset = m_indices.size();
    unsigned int indexCountBefore = m_indices.size();
    int vertindexoffset = m_vertices.size()/3;

    // Get Vertices
    if(mesh->mNumVertices > 0)
    {
        for(uint ii=0; ii<mesh->mNumVertices; ++ii)
        {
            aiVector3D &vec = mesh->mVertices[ii];

            m_vertices.push_back(vec.x);
            m_vertices.push_back(vec.y);
            m_vertices.push_back(vec.z);
        }
    }

    // Get Normals
    if( mesh->HasNormals() )
    {
        for(uint ii=0; ii<mesh->mNumVertices; ++ii)
        {
            aiVector3D &vec = mesh->mNormals[ii];
            m_normals.push_back(vec.x);
            m_normals.push_back(vec.y);
            m_normals.push_back(vec.z);
        };
    }

    // Get Texture coordinates
    if(mesh->GetNumUVChannels() > 0)
    {
        if((unsigned int)m_textureUV.size() < mesh->GetNumUVChannels()) // Caution, assumes all meshes in this model have same number of uv channels
        {
            m_textureUV.resize(mesh->GetNumUVChannels());
            m_textureUVComponents.resize(mesh->GetNumUVChannels());
        }

        for( unsigned int ich = 0; ich < mesh->GetNumUVChannels(); ++ich)
        {
            m_textureUVComponents[ich] = mesh->mNumUVComponents[ich];

            for(uint iind = 0; iind<mesh->mNumVertices; ++iind)
            {
                m_textureUV[ich].push_back(    mesh->mTextureCoords[ich][iind].x);
                if(mesh->mNumUVComponents[ich] > 1)
                {
                    m_textureUV[ich].push_back(mesh->mTextureCoords[ich][iind].y);
                    if(mesh->mNumUVComponents[ich] > 2)
                    {
                        m_textureUV[ich].push_back(    mesh->mTextureCoords[ich][iind].z);
                    }
                }
            }
        }
    }

    // Get mesh indexes
    for(uint t = 0; t<mesh->mNumFaces; ++t)
    {
        aiFace* face = &mesh->mFaces[t];
        if(face->mNumIndices != 3)
        {
            qDebug() << "Warning: Mesh face with not exactly 3 indices, ignoring this primitive." << face->mNumIndices;
            continue;
        }

        m_indices.push_back(face->mIndices[0]+vertindexoffset);
        m_indices.push_back(face->mIndices[1]+vertindexoffset);
        m_indices.push_back(face->mIndices[2]+vertindexoffset);
    }

    newMesh->indexCount = m_indices.size() - indexCountBefore;
    newMesh->material = m_materials.at(mesh->mMaterialIndex);

    return newMesh;
}
