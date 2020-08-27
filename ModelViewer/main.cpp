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



//#include "Math/matrix.h"
//#include <QDebug>


//int main(int argc, char *argv[]){
//    Matrix4f a;
//    a.m[0][0] = 1 ; a.m[0][1] = 1 ; a.m[0][2] = 1; a.m[0][3] = 1;
//    a.m[1][0] = 7 ; a.m[1][1] = 4 ; a.m[1][2] = 2; a.m[1][3] = 2;
//    a.m[2][0] = 8 ; a.m[2][1] = 5 ; a.m[2][2] = 3; a.m[2][3] = 3;
//    a.m[3][0] = 9 ; a.m[3][1] = 6 ; a.m[3][2] = 4; a.m[3][3] = 4;

//    QMatrix4x4 b;



//    qDebug() << b;



//}
