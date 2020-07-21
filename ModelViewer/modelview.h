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
    void setProjectionMatrix(QOpenGLShaderProgram* program, QVector3D translate, float degree, QVector3D rotationAxis);
    void setLightingProperties(QOpenGLShaderProgram *program, float ambientStrength = 0.1);

private:
    QOpenGLShaderProgram* program_object;
    QOpenGLShaderProgram* program_light;

    void checkInput();
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *e) override;

    Camera camera;
    QMap<QString, Model*> models;

    QVector3D m_globalLightPosition;
    QVector3D m_globalLightColor;

    friend class MainWindow;

signals:

};

#endif // MODELVIEW_H
