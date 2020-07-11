#ifndef MODELLOADER_H
#define MODELLOADER_H
#include <QDebug>
#include <QString>
#include <QVector>
#include <QVector3D>
#include <QSharedPointer>

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;

#include "DataStructures.inl"

class ModelLoader
{
public:
    ModelLoader();
    bool Load(QString filePath);
    void getBufferData(QVector<float> **vertices, QVector<float> **normals,
                       QVector<unsigned int> **indices);

    QSharedPointer<MaterialInfo> processMaterial(aiMaterial *material);
    QSharedPointer<Mesh> processMesh(aiMesh *mesh);


    QVector<float> m_vertices;
    QVector<float> m_normals;
    QVector<unsigned int> m_indices;

    QVector<QSharedPointer<MaterialInfo>> m_materials;
    QVector<QSharedPointer<Mesh>> m_meshes;

};

#endif // MODELLOADER_H
