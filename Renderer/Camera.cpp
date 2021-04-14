#include "include/Camera.h"
#include "include/Kernel.h"

using namespace renderer;
using namespace Eigen;

const SceneTransform& Camera::GetTransform() const { return transform_; }
void Camera::SetTransform(const SceneTransform& transform) {
    transform_ = transform;
    KernelAccessor()->OnUpdate();
}
SceneTransformProxy Camera::TransformProxy() {
    return SceneTransformProxy(&transform_, [](){ KernelAccessor()->OnUpdate(); });
}
