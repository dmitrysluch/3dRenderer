#pragma once
#include "BasicMaterial.h"
#include "Mesh.h"
#include "Transform.h"

namespace renderer {
class Kernel;
class Object {
   public:
    Object() = delete;
    [[nodiscard]] const string &GetName() const;
    [[nodiscard]] const shared_ptr<Mesh> &GetMesh() const;
    void SetMesh(const shared_ptr<Mesh> &mesh);
    [[nodiscard]] const MaterialVec &GetMaterialOverrides() const;
    void SetMaterialOverrides(const MaterialVec &material_overrides);
    void SetMaterialOverride(int id, const shared_ptr<const BasicMaterial>& material);
    [[nodiscard]] const SceneTransform &GetTransform() const;
    [[nodiscard]] SceneTransformProxy TransformProxy();
    void SetTransform(const SceneTransform &transform);

   private:
    Object(Kernel *kernel, string name, shared_ptr<Mesh> mesh, MaterialVec material_overrides)
        : kernel_(kernel), name_(std::move(name)), mesh_(std::move(mesh)), material_overrides_(std::move(material_overrides)) {
        assert(mesh_ && "Pointer to mesh must have value");
        assert(name_.find_first_not_of(" \t\n\r") != std::string::npos && "Name must have not whitespace characters");
        assert(material_overrides_.empty() || material_overrides_.size() == mesh_->Size());
    }
    static Object *InstantiateWithKernel(Kernel *kernel, string name, shared_ptr<Mesh> mesh, MaterialVec material_overrides);
    static Object *InstantiateWithKernel(Kernel *kernel, string name, shared_ptr<Mesh> mesh);

    Kernel *kernel_;
    string name_;
    SceneTransform transform_;
    shared_ptr<Mesh> mesh_;
    MaterialVec material_overrides_;
    friend class Kernel;
};
}  // namespace renderer