#ifndef CAMERA_H
#define CAMERA_H


#include <QMatrix4x4>
#include <vector>


/* 相对于摄像机指向的移动方向，是一种抽象，以避开特定于窗口系统的输入方法 */
enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};


/* 常量区 */
const float YAW         =   -90.0f;
const float PITCH       =   0.0f;
const float SPEED       =   2.5f;   // 摄像机移动灵敏度
const float SENSITIVITY =   0.1f;   // 鼠标灵敏度
const float ZOOM_FOV    =   50.f;   // 摄像机视角
const float PI          =   3.1415926;


class Camera
{
public:
    Camera(QVector3D position, QVector3D up, float yaw, float pitch);
    Camera(float pos_x, float pos_y, float pos_z,
           float up_x, float up_y, float up_z,
           float yaw, float pitch);

    /* 移动摄像机（相对于摄像机指向平面 前后上下移动） */
    void moveCamera(Camera_Movement direction, float delta_time);

    /* 改变摄像机指向，需要x和y方向上的偏移量 */
    void changeCameraFront(float x_offset, float y_offset, bool constrain_pitch);

    /* 改变摄像机的镜头视角（缩放） */
    void changeCameraZoomFov(float offset);

    /* 返回 View 观察矩阵（世界） */
    QMatrix4x4 getViewMatrix();



private:
    /* 根据相机的（更新后的）Euler角度计算前矢量 */
     void updateCameraVectors();


public:
    /* 基底 */
    QVector3D position; // 摄像机位置
    QVector3D front;    // 摄像机指向
    QVector3D right;    // 摄像机右轴（摄像机空间的x轴正方向）
    QVector3D up;       // 摄像机上轴（摄像机空间的y轴正方向）
    QVector3D world_up; //

    /* 万向轴 */
    float yaw;
    float pitch;

    /* 配置参数 */
    float move_speed;
    float mouse_sensitivity;
    float zoom_fov;


private:

};


Camera::Camera(QVector3D position = QVector3D(0.0f, 0.0f, 0.0f),
               QVector3D up = QVector3D(0.0f, 1.0f, 0.0f),
               float yaw = YAW,
               float pitch = PITCH) :
    position(position),
    world_up(up),
    yaw(yaw),
    pitch(pitch)
{
    updateCameraVectors();
}


Camera::Camera(float pos_x, float pos_y, float pos_z,
               float up_x, float up_y, float up_z,
               float yaw, float pitch) :
    position(QVector3D(pos_x, pos_y, pos_z)),
    world_up(QVector3D(up_x, up_y, up_z)),
    yaw(yaw),
    pitch(pitch)
{
    updateCameraVectors();
}


/* 移动摄像机（相对于摄像机指向平面 前后上下移动） */
void Camera::moveCamera(Camera_Movement direction, float delta_time)
{
    float velocity = move_speed * delta_time;

    if      (Camera_Movement::FORWARD == direction)     position += (front * velocity);
    else if (Camera_Movement::BACKWARD == direction)    position -= (front * velocity);
    else if (Camera_Movement::RIGHT == direction)       position += (right * velocity);
    else if (Camera_Movement::LEFT == direction)        position -= (right * velocity);
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
        if (pitch >=  89.90f) pitch =  89.90f;
        if (pitch <= -89.90f) pitch = -89.90f;
    }

    updateCameraVectors();
}


/* 改变摄像机的镜头视角（缩放） */
void Camera::changeCameraZoomFov(float offset)
{
    zoom_fov -= offset;
    if      (zoom_fov <= 1.0f) zoom_fov = 1.0f;
    else if (zoom_fov >= 75.0f) zoom_fov = 75.0f;
}


/* 返回 View 观察矩阵（世界） */
void Camera::getViewMatrix()
{
    return QMatrix4x4.lookAt(position, position + front, up);
}


 /* 摄像机万向轴上偏转后重新计算 指向、右轴、上轴 */
void Camera::updateCameraVectors()
{
    QVector3D new_front;
    new_front.setX(cos(yaw*PI/180) * cos(pitch*PI/180));
    new_front.setY(sin(pitch*PI/180));
    new_front.setZ(sin(yaw*PI/180) * cos(pitch*PI/180));
    new_front.normalize();
    front = new_front;

    right = QVector3D::crossProduct(front, world_up);
    right.normalize();

    up = QVector3D::crossProduct(right, front);
    up.normalize();
}

#endif // CAMERA_H
