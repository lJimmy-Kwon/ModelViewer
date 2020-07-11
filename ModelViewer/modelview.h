#ifndef MODELVIEW_H
#define MODELVIEW_H

#include <QOpenGLWidget>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

#include <QDebug>

class ModelLoader;

class ModelView : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit ModelView(QWidget *parent = nullptr);
    ~ModelView();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void update();
    void printContextInformation();
private:
    QOpenGLVertexArrayObject m_object;
    QOpenGLShaderProgram* m_program;
    QOpenGLBuffer m_vertex;
    QOpenGLBuffer m_indices = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);


    ModelLoader* model;
    QVector<float> *vertices;
    QVector<float> *normals;
    QVector<unsigned int> *indices;

signals:

};

#endif // MODELVIEW_H
