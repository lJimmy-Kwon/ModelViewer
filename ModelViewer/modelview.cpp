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
    glEnable(GL_DEPTH_TEST);

    m_program = new QOpenGLShaderProgram(); // only one variable to be allocated dynamicaly.
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex  , ":/Shaders/model.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/model.frag");

    m_program->link();

    models["cube"] = new Model("/Users/jimmy/Desktop/Scene.obj");
}

void ModelView::resizeGL(int w, int h)
{
    Q_UNUSED(w)
    Q_UNUSED(h)
}

void ModelView::setProjectionMatrix(QVector3D translate = {0, 0, 0},
                                    float degree = 0,
                                    QVector3D rotationAxis = {0, 0, 0}){

    QMatrix4x4 projection;
    projection.setToIdentity();
    projection.perspective(60.0f, ((float)width() / height()), 0.1f, 10.0f );

    QMatrix4x4 view = camera.getWorldToViewMatrix();

    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();
    modelMatrix.translate( translate );
    modelMatrix.rotate( degree, rotationAxis );

    m_program->setUniformValue("pMatrix", projection );
    m_program->setUniformValue("vMatrix", view );
    m_program->setUniformValue("mMatrix", modelMatrix );

}

void ModelView::paintGL()
{
    checkInput();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_program->bind();
    {
        { //position
            setProjectionMatrix();
        }
        { //color
            QVector3D lightColor(1.0f, 1.0f, 1.0f);
            QVector3D toyColor(1.0f, 0.5f, 0.31f);
            QVector3D result = lightColor * toyColor;

        }
    }

    m_program->release();
    models["cube"]->Draw(*m_program);

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


