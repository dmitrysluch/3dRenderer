#include "include/DirectionalLight.h"

#include "include/Kernel.h"
#include "include/Object.h"

using namespace renderer;

const string &DirectionalLight::GetName() const { return name_; }
const SceneTransform &DirectionalLight::GetTransform() const { return transform_; }
void DirectionalLight::SetTransform(const SceneTransform &transform) {
    transform_ = transform;
    kernel_->OnUpdate();
}
SceneTransformProxy DirectionalLight::TransformProxy() {
    return SceneTransformProxy(&transform_, [this]() { kernel_->OnUpdate(); });
}
const Vector3f &DirectionalLight::GetDirection() const { return direction_; }
void DirectionalLight::SetDirection(const Vector3f &direction) {
    direction_ = direction.normalized();
    kernel_->OnUpdate();
}
const Vector3f &DirectionalLight::GetColor() const { return color_; }
void DirectionalLight::SetColor(const Vector3f &color) {
    color_ = color;
    kernel_->OnUpdate();
}
LightDirCol DirectionalLight::GetDirAndColor() const { return LightDirCol(direction_, color_); }
DirectionalLight *DirectionalLight::InstantiateWithKernel(Kernel *kernel, string name, const SceneTransform &transform,
                                                          const Vector3f &direction, const Vector3f &color) {
    auto light = new DirectionalLight(kernel, std::move(name), transform, direction, color);
    kernel->lights_.push_back(unique_ptr<DirectionalLight>(light));
    kernel->OnUpdate();
    return light;
}
DirectionalLight *DirectionalLight::GetComponentInKernel(Kernel &kernel, const string &name) {
    auto iter = find_if(kernel.lights_.begin(), kernel.lights_.end(),
                        [name](const unique_ptr<DirectionalLight> &light) { return light->GetName() == name; });
    assert(iter != kernel.lights_.end() && "Light not found");
    return iter->get();
}
const DirectionalLight *DirectionalLight::GetComponentInKernel(const Kernel &kernel, const string &name) {
    auto iter = find_if(kernel.lights_.cbegin(), kernel.lights_.cend(),
                        [name](const unique_ptr<DirectionalLight> &light) { return light->GetName() == name; });
    assert(iter != kernel.lights_.cend() && "Light not found");
    return iter->get();
}