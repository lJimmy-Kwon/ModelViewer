#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QVector2D>

class Camera
{
    QVector3D position;
    QVector3D viewDirection;
    QMatrix4x4 temp;
    const QVector3D UP;

    QVector2D oldMousePosition;

public:
    Camera();
    void moveForward();
    void moveBackward();
    void strafeLeft();
    void movestrafeRight();
    void moveUp();
    void moveDown();


    void mouseUpdate(const QVector2D);
    QMatrix4x4 getWorldToViewMatrix();
    QVector3D getPosition();
};

#endif // CAMERA_H
