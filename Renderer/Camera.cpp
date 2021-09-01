#include <memory>
#include "include/Camera.h"
#include "include/Kernel.h"

using namespace renderer;
using namespace Eigen;
using namespace std;

void Camera::SetFovAngle(float fov_angle) {
    fov_angle_ = fov_angle;
    kernel_->OnUpdate();
}
void Camera::SetFovAngleDeg(float fov_angle) {
    fov_angle_ = fov_angle * static_cast<float>(EIGEN_PI) / 180.f;
    kernel_->OnUpdate();
}
void Camera::SetAspectRatio(float aspect_ratio) {
    aspect_ratio_ = aspect_ratio;
    kernel_->OnUpdate();
}
void Camera::SetFarClipPlane(float far_clip_plane) {
    far_clip_plane_ = far_clip_plane;
    kernel_->OnUpdate();
}
void Camera::SetNearClipPlane(float near_clip_plane) {
    near_clip_plane_ = near_clip_plane;
    kernel_->OnUpdate();
}
const SceneTransform& Camera::GetTransform() const { return transform_; }
void Camera::SetTransform(const SceneTransform& transform) {
    transform_ = transform;
    kernel_->OnUpdate();
}
SceneTransformProxy Camera::TransformProxy() {
    return SceneTransformProxy(&transform_, [this](){ kernel_->OnUpdate(); });
}
Camera::Camera(Kernel *kernel, std::string name) : kernel_(kernel), name_(std::move(name)) {
    assert(name_.find_first_not_of(" \t\n\r") != std::string::npos && "Name must have not whitespace characters");
}
void Camera::Activate() {
    for (int i = 0; i < kernel_->cameras_.size(); ++i) {
        if (kernel_->cameras_[i].get() == this) {
            kernel_->active_camera_id_ = i;
            break;
        }
    }
    kernel_->OnUpdate();
}
Camera* Camera::InstantiateWithKernel(Kernel *kernel, string name) {
    auto cam = new Camera(kernel, std::move(name));
    kernel->cameras_.push_back(unique_ptr<Camera>(cam));
    if (kernel->cameras_.size() == 1) {
        kernel->active_camera_id_ = 0;
    }
    kernel->OnUpdate();
    return cam;
}
Camera *Camera::GetComponentInKernel(Kernel &kernel, const string& name) {
    auto iter = find_if(kernel.cameras_.begin(), kernel.cameras_.end(),
                        [name](const unique_ptr<Camera>& camera) { return camera->GetName() == name; });
    assert(iter != kernel.cameras_.end() && "Camera not found");
    return iter->get();
}
const Camera *Camera::GetComponentInKernel(const Kernel &kernel, const string& name) {
    auto iter = find_if(kernel.cameras_.cbegin(), kernel.cameras_.cend(),
                        [name](const unique_ptr<Camera>& camera) { return camera->GetName() == name; });
    assert(iter != kernel.cameras_.cend() && "Camera not found");
    return iter->get();
}
