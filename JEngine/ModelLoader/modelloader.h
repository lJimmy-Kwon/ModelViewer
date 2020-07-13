#ifndef MODELLOADER_H
#define MODELLOADER_H
#include <string>
#include <QMatrix4x4>
#include <vector>
#include <QFile>
#include <QSharedPointer>
#include <QDir>

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;

#include "DataStructures.inl"

class ModelLoader
{
public:
    enum PathType {
        RelativePath,
        AbsolutePath
    };

    ModelLoader(bool transformToUnitCoordinates = true);
    bool Load(QString filePath, PathType pathType);
    void getBufferData( QVector<float> **vertices, QVector<float> **normals,
                        QVector<unsigned int> **indices);

    void getTextureData( QVector<QVector<float> > **textureUV,                   // For texture mapping
                         QVector<float> **tangents, QVector<float> **bitangents);// For normal mapping

    QSharedPointer<Node> getNodeData() { return m_rootNode; }

    // Texture information
    int numUVChannels() { return m_textureUV.size(); }
    int numUVComponents(int channel) { return m_textureUVComponents.at(channel); }
private:
    QSharedPointer<MaterialInfo> processMaterial(aiMaterial *mater);
    QSharedPointer<Mesh> processMesh(aiMesh *mesh);
    void processNode(const aiScene *scene, aiNode *node, Node *parentNode, Node &newNode);

    void transformToUnitCoordinates();
    void findObjectDimensions(Node *node, QMatrix4x4 transformation, QVector3D &minDimension, QVector3D &maxDimension);

    QVector<float> m_vertices;
    QVector<float> m_normals;
    QVector<unsigned int> m_indices;

    QVector<QVector<float> > m_textureUV; // multiple channels
    QVector<float> m_tangents;
    QVector<float> m_bitangents;
    QVector<unsigned int > m_textureUVComponents; // multiple channels

    QVector<QSharedPointer<MaterialInfo> > m_materials;
    QVector<QSharedPointer<Mesh> > m_meshes;
    QSharedPointer<Node> m_rootNode;
    bool m_transformToUnitCoordinates;
};
#endif // MODELLOADER_H
