#include "camera.hpp"
#include <math.h>


Camera::Camera() noexcept
{
    position = QVector3D(0.0f, 0.0f, 5.0f);
    up = QVector3D(0.0f, 1.0f, 0.0f);
    zoom_fov = ZOOM_FOV;
    yaw = YAW;
    pitch = PITCH;
    move_speed = SPEED;
    mouse_sensitivity = SENSITIVITY;

    _updateCameraVectors();
}


Camera::Camera(QVector3D position   ,
               QVector3D up         ,
               float zoom_fov       ,
               float yaw            ,
               float pitch          ) noexcept :
    position(position),
    world_up(up),
    yaw(yaw),
    pitch(pitch),
    zoom_fov(zoom_fov)
{
    Q_ASSERT_X((zoom_fov>=MIN_ZOOM_FOV && zoom_fov<=MAX_ZOOM_FOV), "init Camera", "zoom_fov should between [MIN_ZOOM_FOV, MAX_ZOOM_FOV]");

    move_speed = SPEED;
    mouse_sensitivity = SENSITIVITY;

    _updateCameraVectors();
}


Camera::Camera(float pos_x, float pos_y, float pos_z,
               float up_x, float up_y, float up_z,
               float zoom_fov, float yaw, float pitch) noexcept :
    position(QVector3D(pos_x, pos_y, pos_z)),
    world_up(QVector3D(up_x, up_y, up_z)),
    yaw(yaw),
    pitch(pitch),
    zoom_fov(zoom_fov)
{
    Q_ASSERT_X((zoom_fov>=MIN_ZOOM_FOV && zoom_fov<=MAX_ZOOM_FOV), "init Camera", "zoom_fov should between [MIN_ZOOM_FOV, MAX_ZOOM_FOV]");

    move_speed = SPEED;
    mouse_sensitivity = SENSITIVITY;

    _updateCameraVectors();
}


/* 移动摄像机（相对于摄像机指向平面 前后上下移动） */
void Camera::moveCamera(Camera_Movement direction, float delta_time)
{
    float velocity = move_speed * delta_time;

    qDebug() <<"[INFO] Move camera: direction=" << direction << "delta_time=" << delta_time << " , velocity=" << velocity;

    if      (Camera_Movement::FORWARD   == direction)    position += (front * velocity);
    else if (Camera_Movement::BACKWARD  == direction)    position -= (front * velocity);
    else if (Camera_Movement::RIGHT     == direction)    position += (right * velocity);
    else if (Camera_Movement::LEFT      == direction)    position -= (right * velocity);
    else if (Camera_Movement::UP        == direction)    position += QVector3D(0.0f, velocity, 0.0f);
    else if (Camera_Movement::DOWN      == direction)    position -= QVector3D(0.0f, velocity, 0.0f);
    else    qDebug() << "[ERROR] When move camera";
}


/* 改变摄像机指向，需要x和y方向上的偏移量 */
void Camera::changeCameraFront(float x_offset, float y_offset, bool constrain_pitch = true)
{
    yaw += x_offset * mouse_sensitivity;
    pitch -= y_offset * mouse_sensitivity;

    /* 确保超出边界时，屏幕不会被翻转 */
    if (constrain_pitch)
    {
        if (pitch >= MAX_PITCH) pitch = MAX_PITCH;
        if (pitch <= MIN_PITCH) pitch = MIN_PITCH;
    }

        qDebug() << "[INFO] Change Camera Frount: x_offset=" << x_offset << ", y_offset=" << y_offset
                 << "yaw=" << yaw << ", pitch=" << pitch;

    _updateCameraVectors();
}


/* 改变摄像机的镜头视角（缩放） */
void Camera::changeCameraZoomFov(float offset)
{
    zoom_fov -= offset;

    if      (zoom_fov <= MIN_ZOOM_FOV) zoom_fov = MIN_ZOOM_FOV;
    else if (zoom_fov >= MAX_ZOOM_FOV) zoom_fov = MAX_ZOOM_FOV;

    qDebug() << "[INFO] Change Camera Zoom_Fov: offset=" << offset << ", new zoom_fov=" << zoom_fov;
}


/* 返回 View 观察矩阵（世界） */
QMatrix4x4 Camera::getViewMatrix()
{
    QMatrix4x4 view_materx;
    view_materx.lookAt(position, position + front, up);
    return view_materx;
}


 /* 摄像机万向轴上偏转后重新计算 指向、右轴、上轴 */
void Camera::_updateCameraVectors()
{
    QVector3D new_front;
    new_front.setX(cos(yaw*M_PI/180) * cos(pitch*M_PI/180));
    new_front.setY(sin(pitch*M_PI/180));
    new_front.setZ(sin(yaw*M_PI/180) * cos(pitch*M_PI/180));
    new_front.normalize();
    front = new_front;

    right = QVector3D::crossProduct(front, world_up);
    right.normalize();

    up = QVector3D::crossProduct(right, front);
    up.normalize();
}
