#ifndef MESH_H
#define MESH_H

#include <QOpenGLShaderProgram>

#include <QVector3D>
#include <QVector2D>
#include <QVector>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>

#include "Math/matrix.h"

struct VertexBoneData
{
    uint IDs[4] = {0, 0, 0, 0};
    float Weights[4] = {0, 0, 0, 0};

    VertexBoneData()
    {
        Reset();
    };

    void Reset()
    {
        memset(IDs, 0, 4);
        memset(Weights, 0, 4);
    }

    void AddBoneData(uint BoneID, float Weight){
        for (uint i = 0 ; i < 4 ; i++) {
            if (Weights[i] == 0.0) {
                IDs[i]     = BoneID;
                Weights[i] = Weight;
                return;
            }
        }
    }

    VertexBoneData(const VertexBoneData &data)
    {
        for(uint i = 0 ; i < 4 ; i++){
            this->IDs[i] = data.IDs[i];
            this->Weights[i] = data.Weights[i];
        }
    }

    VertexBoneData(VertexBoneData &data)
    {
        for(uint i = 0 ; i < 4 ; i++){
            this->IDs[i] = data.IDs[i];
            this->Weights[i] = data.Weights[i];
        }
    }


    VertexBoneData* operator=(VertexBoneData& data){

        for(uint i = 0 ; i < 4 ; i++){
            this->IDs[i] = data.IDs[i];
            this->Weights[i] = data.Weights[i];
        }
        return this;
    }

};



struct Vertex{
    QVector3D Position;
    QVector3D Normal;
    QVector2D TexCoords;

    VertexBoneData bones;

    Vertex* operator=(Vertex data){

        this->Position = data.Position;
        this->Normal = data.Normal;
        this->TexCoords = data.TexCoords;

        this->bones = data.bones;

        return this;
    }
};

struct Texture{
    QOpenGLTexture* image;
    QString type;
    QString path;
    float shininess;
};

struct BoneInfo
{
    Matrix4f BoneOffset;
    Matrix4f FinalTransformation;

    BoneInfo()
    {
        BoneOffset.SetZero();
        FinalTransformation.SetZero();
    }
};

class Mesh
{
public:
    QVector<Vertex> vertices;
    QVector<unsigned int> indices;
    QVector<Texture> textures;

    Mesh(QVector<Vertex> vertices, QVector<unsigned int> indices,
         QVector<Texture> textures, QVector<VertexBoneData> Bones);
    Mesh(const Mesh& mesh);
    ~Mesh();

    void Draw(QOpenGLShaderProgram &program);

    /* VAO, VBO, EBO, textures[i].image are should be deleted after draw */
    QOpenGLVertexArrayObject* VAO;
    QOpenGLBuffer* VBO;
    QOpenGLBuffer* EBO;


private:
    //render data


    void setupMesh();

};

#endif // MESH_H
