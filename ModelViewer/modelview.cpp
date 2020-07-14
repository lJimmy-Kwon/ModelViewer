#include "modelview.h"
#include "ModelLoader/modelloader.h"
#include "Input/input.h"

QImage* img_path(QString path){
    return new QImage(path);
}


ModelView::ModelView(QWidget *parent) : QOpenGLWidget(parent)
{
    qDebug() << __func__;

    model = new ModelLoader();
    model->Load("/Users/jimmy/Desktop/plain.obj", ModelLoader::PathType::AbsolutePath);

    model->getBufferData(&vertices, &normals, &indices);
    model->getTextureData(&textureUV, 0, 0);

    qDebug() << " vertices : " << vertices->size();
    qDebug() << " normals : "  << normals->size();
    qDebug() << " indices : "  << indices->size();

    qDebug() << " textureUV : "  << textureUV[0].size();

    qDebug() << " verts ";
    qDebug() << vertices[0];

    qDebug() << " index ";
    qDebug() << indices[0];

    qDebug() << " textureUV ";
    qDebug() << textureUV[0][0];

}

ModelView::~ModelView()
{

}

void ModelView::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    m_texture = new QOpenGLTexture(*img_path("/Users/jimmy/Desktop/paint.png"));

    m_program = new QOpenGLShaderProgram(); // only one variable to be allocated dynamicaly.
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex  , ":/Shaders/model.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/model.frag");

    m_program->link();
    m_program->bind();

    // Create Buffer ( Do not release util VAO is created)
    m_vertex.create();
    m_vertex.bind();
    m_vertex.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertex.allocate( vertices->constData(), vertices->size() * sizeof(float));
    m_vertex.release();

    // create IndexBuffer
    m_indices.create();
    m_indices.bind();
    m_indices.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_indices.allocate( indices->constData(), indices->size() * sizeof(unsigned int));
    m_indices.release();

    m_textureUVBuffer.create();
    m_textureUVBuffer.bind();
    m_textureUVBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_textureUVBuffer.allocate( textureUV[0][0].constData(), textureUV[0][0].size() * sizeof(float));
    m_textureUVBuffer.release();

    // Create Vertex Array Object
    m_object.create();
    m_object.bind();
    {
        m_vertex.bind();
        m_program->enableAttributeArray(0);
        m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

        m_textureUVBuffer.bind();
        m_program->enableAttributeArray(1);
        m_program->setAttributeBuffer(1, GL_FLOAT, 0, 2, 0);

        m_indices.bind();
    }

    // Release ( unbind ) all
    m_object.release();
    m_indices.release();
    m_vertex.release();
    m_textureUVBuffer.release();
    m_program->release();

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
        m_object.bind();
        m_texture->bind();
        glDrawElements(GL_TRIANGLES, indices->size(), GL_UNSIGNED_INT, 0 );
        m_texture->release();
        m_object.release();
    }

    m_program->release();

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


