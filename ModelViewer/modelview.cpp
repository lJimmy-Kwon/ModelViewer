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

    // program for normal objects
    {
        program_object = new QOpenGLShaderProgram(); // only one variable to be allocated dynamicaly.
        program_object->addShaderFromSourceFile(QOpenGLShader::Vertex  , ":/Shaders/model.vert");
        program_object->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/model.frag");

        program_object->link();
    }

    // program for light source
    {
        program_light = new QOpenGLShaderProgram(); // only one variable to be allocated dynamicaly.
        program_light->addShaderFromSourceFile(QOpenGLShader::Vertex  , ":/Shaders/light.vert");
        program_light->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/light.frag");

        program_light->link();
    }

    models["ball"]  = new Model("/Users/jimmy/Desktop/Assets/Charactor/Character.fbx");
    models["light"] = new Model("/Users/jimmy/Desktop/Assets/Light/light.obj");
}

void ModelView::resizeGL(int w, int h)
{
    Q_UNUSED(w)
    Q_UNUSED(h)
}

void ModelView::setProjectionMatrix(QOpenGLShaderProgram* program,
                                    QVector3D translate = {0, 0, 0},
                                    float degree = 0,
                                    QVector3D rotationAxis = {0, 0, 0}){

    program->bind();
    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();
    modelMatrix.translate( translate );
    modelMatrix.rotate( degree, rotationAxis );

    QMatrix4x4 view = camera.getWorldToViewMatrix();

    QMatrix4x4 projection;
    projection.setToIdentity();
    projection.perspective(60.0f, ((float)width() / height()), 0.1f, 10.0f );

    program->setUniformValue("mMatrix", modelMatrix );
    program->setUniformValue("vMatrix", view );
    program->setUniformValue("pMatrix", projection );

    program->setUniformValue("normalMatrix", modelMatrix.inverted().transposed().toGenericMatrix<3, 3>());

    program->release();

}

void ModelView::setLightingProperties(QOpenGLShaderProgram* program,
                                      float ambientStrength){
    program->bind();
    program->setUniformValue("ambientStrength", ambientStrength);
    program->setUniformValue("lightPosition", m_globalLightPosition);
    program->setUniformValue("lightColor", m_globalLightColor);
    program->setUniformValue("viewPos", camera.getPosition());

    program->release();

}

void ModelView::paintGL()
{
    checkInput();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_globalLightPosition = {0, 2, 0};
    m_globalLightColor = {0.7, 0.7, 0.7};


    //cube
    {
        setProjectionMatrix(program_object, {0, 0, -2}, 90, {1, 0, 0});
        setLightingProperties(program_object, 0.4);
        models["ball"]->Draw(*program_object);
    }

    //light
    {
        setProjectionMatrix(program_light, m_globalLightPosition);
        models["light"]->Draw(*program_light);
    }


    QOpenGLWidget::update();
}

void ModelView::checkInput()
{
    Input::update();

    if( Input::keyPressed(Qt::Key_Escape)){

        QApplication::quit();

    }if(Input::keyPressed(Qt::Key::Key_5)){

        camera.moveForward();

    }if(Input::keyPressed(Qt::Key::Key_2)){

        camera.moveBackward();

    }if(Input::keyPressed(Qt::Key::Key_1)){

        camera.strafeLeft();

    }if(Input::keyPressed(Qt::Key::Key_3)){

        camera.movestrafeRight();

    }if(Input::keyPressed(Qt::Key::Key_4)){

        camera.moveUp();

    }if(Input::keyPressed(Qt::Key::Key_6)){

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


