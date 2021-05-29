#include "include/Object.h"
#include "include/Kernel.h"

using namespace renderer;

Object * Object::InstantiateWithKernel(Kernel *kernel, string name, shared_ptr<Mesh> mesh, MaterialVec material_overrides) {
    auto obj = new Object(kernel, std::move(name), std::move(mesh), std::move(material_overrides));
    kernel->objects_.push_back(unique_ptr<Object>(obj));
    kernel->OnUpdate();
    return obj;
}
Object * Object::InstantiateWithKernel(Kernel* kernel, string name, shared_ptr<Mesh> mesh) {
    return InstantiateWithKernel(kernel, std::move(name), std::move(mesh), MaterialVec());
}
const shared_ptr<Mesh>& Object::GetMesh() const { return mesh_; }
void Object::SetMesh(const shared_ptr<Mesh>& mesh) {
    mesh_ = mesh;
    kernel_->OnUpdate();
}
const MaterialVec& Object::GetMaterialOverrides() const { return material_overrides_; }
void Object::SetMaterialOverrides(const MaterialVec& material_overrides) {
    assert(material_overrides.empty() || material_overrides.size() == mesh_->Size());
    material_overrides_ = material_overrides;
    kernel_->OnUpdate();
}
const string& Object::GetName() const { return name_; }
void Object::SetMaterialOverride(int id, const shared_ptr<const BasicMaterial> &material) {
    assert(material && "Shared ptr must have value");
    assert(!material_overrides_.empty());
    material_overrides_[id] = material;
    kernel_->OnUpdate();
}
const SceneTransform& Object::GetTransform() const { return transform_; }
void Object::SetTransform(const SceneTransform& transform) {
    transform_ = transform;
    kernel_->OnUpdate();
}
SceneTransformProxy Object::TransformProxy() {
    return SceneTransformProxy(&transform_, [this](){ kernel_->OnUpdate(); });
}
