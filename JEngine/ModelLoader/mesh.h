#ifndef MESH_H
#define MESH_H

#include <QOpenGLShaderProgram>

#include <QVector3D>
#include <QVector2D>
#include <QVector>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>


struct Vertex{
    QVector3D Position;
    QVector3D Normal;
    QVector2D TexCoords;
};

struct Texture{
    QOpenGLTexture* image;
    QString type;
    QString path;
    float shininess;
};


class Mesh
{
public:
    QVector<Vertex> vertices;
    QVector<unsigned int> indices;
    QVector<Texture> textures;

    Mesh(QVector<Vertex> vertices, QVector<unsigned int> indices,
         QVector<Texture> textures);
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
