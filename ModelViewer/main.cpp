#include "mainwindow.h"
#include <QApplication>
#include <QOpenGLWidget>



int main(int argc, char *argv[])
{

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion( 3, 3 );
    format.setSamples(16);
    format.setProfile(QSurfaceFormat::CoreProfile);

    QSurfaceFormat::setDefaultFormat(format);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
