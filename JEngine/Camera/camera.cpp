#include "camera.h"

Camera::Camera() : viewDirection(0.0f, 0.0f, -1.0f), UP(0.0f, 1.0f, 0.0f)
{

}
QMatrix4x4 Camera::getWorldToViewMatrix()
{
    temp.setToIdentity();
    temp.lookAt(position, position + viewDirection, UP);
    return temp;
}

void Camera::mouseUpdate(const QVector2D newMousePosition){

    QVector2D mouseDelta = newMousePosition - oldMousePosition;

    if( mouseDelta.length() > 10.0f ){
        oldMousePosition  = newMousePosition;
        return;
    }

    QVector3D toRotateAround = QVector3D::crossProduct( viewDirection, UP );

    QMatrix4x4 rotation;
    rotation.setToIdentity();
    rotation.rotate(-mouseDelta.x() * 0.5, UP);
    rotation.rotate(-mouseDelta.y() * 0.5, toRotateAround);

    viewDirection = rotation * viewDirection;
    oldMousePosition = newMousePosition;
}

const float MOVEMENT_SPEED = 0.0003f;
void Camera::moveForward()
{
    position += viewDirection * MOVEMENT_SPEED;
}

void Camera::moveBackward()
{
    position -= viewDirection * MOVEMENT_SPEED;
}

void Camera::strafeLeft()
{
    QVector3D toRotateAround = QVector3D::crossProduct( viewDirection, UP );
    position += -toRotateAround * MOVEMENT_SPEED;
}

void Camera::movestrafeRight()
{
    QVector3D toRotateAround = QVector3D::crossProduct( viewDirection, UP );
    position += toRotateAround * MOVEMENT_SPEED;
}

void Camera::moveUp()
{
    position += UP * MOVEMENT_SPEED;
}

void Camera::moveDown()
{
    position += -UP * MOVEMENT_SPEED;
}

QVector3D Camera::getPosition()
{
    return position;
}
