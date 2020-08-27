#ifndef MODEL_H
#define MODEL_H
#include <QString>
#include <QOpenGLShaderProgram>
#include <QVector>

#include "mesh.h"
#include "Math/matrix.h"
#include <sys/time.h>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>


class Model
{

public:
    explicit Model(QString path);
    ~Model();
    void Draw(QOpenGLShaderProgram &program);

private:
    QVector<Mesh> meshes;
    QString directory;

    Assimp::Importer importer;
    const aiScene* scene;

    void loadModel(QString path);
    void processNode(aiNode* node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    QVector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                          QString typeName);

    zero4x4 boneTransform(float timeInSeconds, QVector<zero4x4>& Transforms);
    void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const QMatrix4x4& ParentTransform);
    const aiNodeAnim *FindNodeAnim(const aiAnimation *pAnimation, QString NodeName);
    void CalcInterpolatedScaling(aiVector3D &Out, float AnimationTime, const aiNodeAnim *pNodeAnim);
    uint FindScaling(float AnimationTime, const aiNodeAnim *pNodeAnim);

    QVector<Texture> textures_loaded;
    QMatrix4x4 m_GlobalInverseTransform;
    double m_startTime = 0;

    uint FindRotation(float AnimationTime, const aiNodeAnim *pNodeAnim);
    void CalcInterpolatedRotation(aiQuaternion &Out, float AnimationTime, const aiNodeAnim *pNodeAnim);
    uint FindPosition(float AnimationTime, const aiNodeAnim *pNodeAnim);
    void CalcInterpolatedPosition(aiVector3D &Out, float AnimationTime, const aiNodeAnim *pNodeAnim);



    int m_NumBones = 0;
    QMap<QString,uint> m_BoneMapping; // maps a bone name to its index
    QVector<BoneInfo> m_BoneInfo;

};

#endif // MODEL_H
