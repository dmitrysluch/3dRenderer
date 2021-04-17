#include <memory>
#include "include/Camera.h"
#include "include/Kernel.h"

using namespace renderer;
using namespace Eigen;
using namespace std;

const SceneTransform& Camera::GetTransform() const { return transform_; }
void Camera::SetTransform(const SceneTransform& transform) {
    transform_ = transform;
    KernelAccessor()->OnUpdate();
}
SceneTransformProxy Camera::TransformProxy() {
    return SceneTransformProxy(&transform_, [](){ KernelAccessor()->OnUpdate(); });
}
Camera::Camera(std::string name) : name_(std::move(name)) {
    assert(name_.find_first_not_of(" \t\n\r") != std::string::npos && "Name must have not whitespace characters");
}
Camera* Camera::New(string name) {
    auto cam = new Camera(std::move(name));
    KernelAccessor accessor;
    accessor->cameras_.push_back(unique_ptr<Camera>(cam));
    if (accessor->cameras_.size() == 1) {
        accessor->active_camera_id_ = 0;
    }
    accessor->OnUpdate();
    return cam;
}
