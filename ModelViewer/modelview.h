#ifndef MODELVIEW_H
#define MODELVIEW_H

#include <QOpenGLWidget>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>

#include <QDebug>
#include <QApplication>
#include <QMap>

#include "Camera/camera.h"
#include "ModelLoader/model.h"



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
    void setProjectionMatrix(QVector3D translate, float degree, QVector3D rotationAxis);

private:
    QOpenGLShaderProgram* m_program;

    void checkInput();
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *e) override;

    Camera camera;
    QMap<QString, Model*> models;

    friend class MainWindow;

signals:

};

#endif // MODELVIEW_H
