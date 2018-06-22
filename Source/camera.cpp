#include "camera.h"

Camera::Camera()
{
    m_up = glm::vec3(0, 1, 0);
    m_rotationQuat = glm::quat(1, 0, 0, 0);
    m_lookAt = glm::vec3(0, 0, 0);
    m_deltaPos = glm::vec3(0, 0, 0);
    m_cameraPos = glm::vec3(32, 16, 32);
    m_cameraHeading = 0.0f;
    m_cameraPitch = 0.0f;
}


void Camera::MouseOrient(int x, int y)
{
    ChangeHeading((float)-x * 0.1f);
    ChangePitch((float)-y * 0.1f);
}

void Camera::Move(CameraDirection dir)
{
    switch(dir)
    {
    case CameraDirection::UP:
        m_deltaPos += m_up * 0.001f;
        break;
    case CameraDirection::DOWN:
        m_deltaPos -= m_up * 0.001f;
        break;
    case CameraDirection::LEFT:
        m_deltaPos -= glm::cross(m_direction, m_up) * 0.001f;
        break;
    case CameraDirection::RIGHT:
        m_deltaPos += glm::cross(m_direction, m_up) * 0.001f;
        break;
    case CameraDirection::BACK:
        m_deltaPos -= m_direction * 0.001f;
        break;
    case CameraDirection::FORWARD:
        m_deltaPos += m_direction * 0.001f;
        break;
    }
}

glm::mat4 Camera::GetMatrix()
{
    m_direction = glm::normalize(m_lookAt - m_cameraPos);

    glm::vec3 axis = glm::cross(m_direction, m_up);
    glm::quat pitch_quat = glm::angleAxis(m_cameraPitch, axis);
    glm::quat heading_quat = glm::angleAxis(m_cameraHeading, m_up);

    glm::quat temp = glm::cross(pitch_quat, heading_quat);
    temp = glm::normalize(temp);

    m_direction = glm::rotate(temp, m_direction);
    m_cameraPos += m_deltaPos;

    m_lookAt = m_cameraPos + m_direction;

    m_deltaPos *= 0.85f;
    m_cameraHeading *= 0.5f;
    m_cameraPitch *= 0.5f;

    return glm::lookAt(m_cameraPos, m_lookAt, m_up);
}

void Camera::ChangeHeading(float deg)
{
    if (deg < -1.0f) {
        deg = -1.0f;
    } else if (deg > 1.0f) {
        deg = 1.0f;
    }

    if (m_cameraPitch > 90 && m_cameraPitch < 270 || (m_cameraPitch < -90 && m_cameraPitch > -270)) {
        m_cameraHeading -= deg;
    } else {
        m_cameraHeading += deg;
    }

    if (m_cameraHeading > 360.0f) {
        m_cameraHeading -= 360.0f;
    } else if (m_cameraHeading < -360.0f) {
        m_cameraHeading += 360.0f;
    }
}

void Camera::ChangePitch(float deg)
{

    if (deg < -1.0f) {
        deg = -1.0f;
    } else if (deg > 1.0f) {
        deg = 1.0f;
    }
    m_cameraPitch += deg;

    if (m_cameraPitch > 360.0f) {
        m_cameraPitch -= 360.0f;
    } else if (m_cameraPitch < -360.0f) {
        m_cameraPitch += 360.0f;
    }
}
