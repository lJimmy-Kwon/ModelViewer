#include "modelview.h"
#include "ModelLoader/modelloader.h"

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

    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_program->bind();
    {
        m_object.bind();
        m_texture->bind();
        glDrawElements(GL_TRIANGLES, indices->size(), GL_UNSIGNED_INT, 0 );
        m_texture->release();
        m_object.release();
    }

    m_program->release();
}

void ModelView::update()
{
    QOpenGLWidget::update();
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

