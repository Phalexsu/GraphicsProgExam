#ifndef PERSPECTIVECAMERA_H_
#define PERSPECTIVECAMERA_H_

#include "Camera.h"
#include "GLFWApplication.h"
#include <array>

class PerspectiveCamera : public Camera
{
public:

    struct Frustrum {
        float angle;
        float width;
        float height;
        float near;
        float far;
    };

public:
    PerspectiveCamera( int width, int height,
        const Frustrum& frustrum = { 45.0f,1.0f, 1.0f, 0.5f, -1.0f },
        const glm::vec3& position = glm::vec3(0.0f, 0.0f, 10.0f),
        const glm::vec3& lookAt = glm::vec3(0.0f, 0.0f, 0.0f),
        const glm::vec3& upVector = glm::vec3(0.0f, 1.0f, 0.0f)) {
        
        Position = position;
        LookAt = lookAt;
        UpVector = upVector;
        CameraFrustrum = frustrum;
        CameraFrustrum.height = height;
        CameraFrustrum.width = width;
        Camera::ViewMatrix = glm::lookAt(position, lookAt, upVector);
        Camera::ProjectionMatrix = glm::perspective(glm::radians(CameraFrustrum.angle),
            CameraFrustrum.width / CameraFrustrum.height, CameraFrustrum.near, CameraFrustrum.far);
        Camera::ViewProjectionMatrix = Camera::ProjectionMatrix * Camera::ViewMatrix;
    }
    ~PerspectiveCamera() = default;
    PerspectiveCamera(const PerspectiveCamera& camera) : Camera(camera)
    {
        this->LookAt = camera.LookAt;
        this->UpVector = camera.UpVector;
        this->CameraFrustrum = camera.CameraFrustrum;
    }

    void SetFrustrum(const Frustrum& frustrum)
    {
        this->CameraFrustrum = frustrum; this->RecalculateMatrix();
    }

    void SetLookAt(const glm::vec3& lookAt)
    {
        this->LookAt = lookAt; this->RecalculateMatrix();
    }

    void SetUpVector(const glm::vec3& upVector)
    {
        this->UpVector = upVector; this->RecalculateMatrix();
    }

protected:
    void RecalculateMatrix() {
        Camera::ViewMatrix = glm::lookAt(Camera::Position, LookAt, UpVector);
        Camera::ProjectionMatrix = glm::perspective(glm::radians(CameraFrustrum.angle), CameraFrustrum.width / CameraFrustrum.height, CameraFrustrum.near, CameraFrustrum.far);
        Camera::ViewProjectionMatrix =  Camera::ProjectionMatrix * Camera::ViewMatrix;
    }

protected:
    glm::vec3 LookAt;
    glm::vec3 UpVector;
    Frustrum CameraFrustrum;
};

#endif // PERSPECTIVECAMERA_H_
