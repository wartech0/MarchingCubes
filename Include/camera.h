#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <glm/ext.hpp>

enum CameraDirection
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    FORWARD,
    BACK
};

class Camera
{
public:
    Camera();

    void Move(CameraDirection dir);

    void MouseOrient(int x, int y);
    void ChangeHeading(float deg);
    void ChangePitch(float deg);

    glm::mat4 GetMatrix();

private:
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;

    float m_cameraHeading;
    float m_cameraPitch;

    glm::vec3 m_cameraPos;
    glm::vec3 m_deltaPos;
    glm::vec3 m_lookAt;
    glm::vec3 m_direction;

    glm::vec3 m_up;
    glm::quat m_rotationQuat;

};

#endif // CAMERA_H
