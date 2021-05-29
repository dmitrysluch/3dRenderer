#include <memory>
#include "include/Camera.h"
#include "include/Kernel.h"

using namespace renderer;
using namespace Eigen;
using namespace std;

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
Camera* Camera::InstantiateWithKernel(Kernel *kernel, string name) {
    auto cam = new Camera(kernel, std::move(name));
    kernel->cameras_.push_back(unique_ptr<Camera>(cam));
    if (kernel->cameras_.size() == 1) {
        kernel->active_camera_id_ = 0;
    }
    kernel->OnUpdate();
    return cam;
}
