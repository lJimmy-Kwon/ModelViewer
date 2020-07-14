#ifndef MODELVIEW_H
#define MODELVIEW_H

#include <QOpenGLWidget>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>

#include <QDebug>
#include <QApplication>

#include "Camera/camera.h"


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

    void printContextInformation();
private:
    QOpenGLVertexArrayObject m_object;
    QOpenGLShaderProgram* m_program;
    QOpenGLBuffer m_vertex;
    QOpenGLBuffer m_indices = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    QOpenGLTexture* m_texture;
    QOpenGLBuffer m_textureUVBuffer;


    ModelLoader* model;
    QVector<float> *vertices;
    QVector<float> *normals;
    QVector<unsigned int> *indices;
    QVector<QVector<float> > *textureUV;

    void checkInput();
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *e) override;

    Camera camera;

    friend class MainWindow;

signals:

};

#endif // MODELVIEW_H
