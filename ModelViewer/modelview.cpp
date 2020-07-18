#include "modelview.h"
#include "Input/input.h"

QImage* img_path(QString path){
    return new QImage(path);
}


ModelView::ModelView(QWidget *parent) : QOpenGLWidget(parent)
{
    qDebug() << __func__;
}

ModelView::~ModelView()
{

}

void ModelView::initializeGL()
{
    glEnable(GL_DEPTH_TEST | GL_CULL_FACE);

    m_program = new QOpenGLShaderProgram(); // only one variable to be allocated dynamicaly.
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex  , ":/Shaders/model.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/model.frag");

    m_program->link();

    model = new Model("/Users/jimmy/Desktop/Scene.obj");
}

void ModelView::resizeGL(int w, int h)
{
    Q_UNUSED(w)
    Q_UNUSED(h)
}

void ModelView::paintGL()
{
    checkInput();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_program->bind();
    {
        {
            QMatrix4x4 modelTransformMatrix;
            modelTransformMatrix.setToIdentity();
            modelTransformMatrix.perspective(60.0f, ((float)width() / height()), 0.1f, 10.0f );
            modelTransformMatrix = modelTransformMatrix * camera.getWorldToViewMatrix();

            QMatrix4x4 projectionMatrix;
            projectionMatrix.setToIdentity();
            projectionMatrix.translate( 0.0f, 0.0f, -1.0f);
            projectionMatrix.rotate( 0.0f, QVector3D(1.0f, 0.0f, 0.0f) );

            projectionMatrix = modelTransformMatrix * projectionMatrix;
            m_program->setUniformValue("projectionMatrix", projectionMatrix );
        }
    }
    m_program->release();

    model->Draw(*m_program);

    QOpenGLWidget::update();
}

void ModelView::checkInput()
{
    Input::update();

    if( Input::keyPressed(Qt::Key_Escape)){

        QApplication::quit();

    }else if(Input::keyPressed(Qt::Key::Key_5)){

        camera.moveForward();

    }else if(Input::keyPressed(Qt::Key::Key_2)){

        camera.moveBackward();

    }else if(Input::keyPressed(Qt::Key::Key_1)){

        camera.strafeLeft();

    }else if(Input::keyPressed(Qt::Key::Key_3)){

        camera.movestrafeRight();

    }else if(Input::keyPressed(Qt::Key::Key_4)){

        camera.moveUp();

    }else if(Input::keyPressed(Qt::Key::Key_6)){

        camera.moveDown();

    }
}

void ModelView::keyPressEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat())
    {
        event->ignore();
    }
    else
    {
        Input::registerKeyPress( event->key() );
    }
}

void ModelView::keyReleaseEvent(QKeyEvent *event)
{

    if(event->isAutoRepeat())
    {
        event->ignore();
    }
    else
    {
        Input::registerKeyRelease(event->key());
    }

}

void ModelView::mousePressEvent(QMouseEvent *event)
{

  Input::registerMousePress(event->button());

}

void ModelView::mouseReleaseEvent(QMouseEvent *event)
{

  Input::registerMouseRelease(event->button());

}

void ModelView::mouseMoveEvent(QMouseEvent* e){
    camera.mouseUpdate(QVector2D(e->x(), e->y()));
}

void ModelView::printContextInformation()
{
    QString glType;
    QString glVersion;
    QString glProfile;

    // Get Version Information
    glType = "OpenGL";
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

    // Get Profile Information
  #define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
    switch (format().profile())
    {
      CASE(NoProfile);
      CASE(CoreProfile);
      CASE(CompatibilityProfile);
    }
  #undef CASE

    // qPrintable() will print our QString w/o quotes around it.
    qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}


