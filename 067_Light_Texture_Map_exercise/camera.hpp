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
    RIGHT,
    UP,
    DOWN
};


/* 常量区 */
const float YAW         =  -90.0f;
const float PITCH       =   0.0f;
const float MIN_PITCH   =  -89.0f;
const float MAX_PITCH   =   89.0f;
const float SPEED       =   2.5f;   // 摄像机移动灵敏度
const float SENSITIVITY =   0.1f;   // 鼠标灵敏度
const float ZOOM_FOV    =   50.f;   // 摄像机视角
const float MIN_ZOOM_FOV=   1.0f;
const float MAX_ZOOM_FOV=   90.0f;


class Camera
{
public:
    /* 注意，如果构造器都带有默认参数，那么会造成调用模糊，因为参数中的带有默认参数，编译器不知道调用哪一个 */
    Camera() noexcept;
    Camera(QVector3D position,
           QVector3D up = QVector3D(0.0f, 1.0f, 0.0f),
           float zoom_fov       = ZOOM_FOV,
           float yaw            = YAW,
           float pitch          = PITCH) noexcept;
    Camera(float pos_x, float pos_y, float pos_z,
           float up_x           = 0.0f, float up_y  = 1.0f, float up_z  = 0.0f,
           float zoom_fov       = ZOOM_FOV,
           float yaw            = YAW,  float pitch = PITCH) noexcept;

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
     void _updateCameraVectors();

// ======================================================================

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


#endif // CAMERA_H
