#ifndef MODEL_H
#define MODEL_H
#include <QString>
#include <QOpenGLShaderProgram>
#include <QVector>

#include "mesh.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>


class Model
{

public:
    explicit Model(QString path);
    void Draw(QOpenGLShaderProgram &program);

private:
    QVector<Mesh> meshes;
    QString directory;

    void loadModel(QString path);
    void processNode(aiNode* node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    QVector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                          QString typeName);

    QVector<Texture> textures_loaded;

};

#endif // MODEL_H
