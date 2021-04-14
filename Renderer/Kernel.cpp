#include "include/Kernel.h"

#include <algorithm>
using std::string;
renderer::Kernel::Kernel(std::unique_ptr<renderer::IView> view) : view_(std::move(view)) {
    renderer_.SetRenderTarget(&view_->GetRenderTarget());
    renderer_.SetDepthBuffer(&view_->GetDepthBuffer());
}
renderer::Object* renderer::Kernel::GetObjectPtr(const string& name) {
    auto iter = find_if(objects_.cbegin(), objects_.cend(),
                   [name](const unique_ptr<Object>& object) { return object->GetName() == name; });
    assert(iter != objects_.cend() && "Object not found");
    return iter->get();
}
const renderer::Object* renderer::Kernel::GetObjectPtr(const string& name) const {
    auto iter = find_if(objects_.cbegin(), objects_.cend(),
                        [name](const unique_ptr<Object>& object) { return object->GetName() == name; });
    assert(iter != objects_.cend() && "Object not found");
    return iter->get();
}
renderer::Object& renderer::Kernel::GetObject(const std::string& name) { return *GetObjectPtr(name); }
const renderer::Object& renderer::Kernel::GetObject(const std::string& name) const { return *GetObjectPtr(name); }
void renderer::Kernel::OnUpdate() {
    for (const auto &object : objects_) {
        renderer_.SetObjectTransform(object->GetTransform().GetTransformMatrix());
        auto mesh = object->GetMesh();
        assert ((!object->GetMaterialOverrides().empty() || mesh->HasDefaultMaterials()) &&
               "Mesh doesn't provide default materials, set material overrides for object");
        auto materialOverrides = object->GetMaterialOverrides();
        for (int i = 0; i < mesh->Size(); ++i) {
            const BasicMaterial *materialPtr;
            if (!materialOverrides.empty()) {
                materialPtr = materialOverrides[i].get();
            } else {
                materialPtr = mesh->GetDefaultMaterial(i).get();
            }
            renderer_.SetMaterial(materialPtr);
            const auto &submesh = mesh->GetSubmesh(i);
            renderer_.SetVertices(&submesh.vertices);
            renderer_.SetIndices(&submesh.indices);
            if (materialPtr->RequireNormals()) {
                assert(!submesh.normals.empty() && "Material requires normals but mesh doesn't have them");
                renderer_.SetNormals(&submesh.normals);
            }
            if (materialPtr->RequireTexcoords()) {
                assert(!submesh.texcoords.empty() && "Material requires texcoords but mesh doesn't have them");
                renderer_.SetTexcoords(&submesh.texcoords);
            }
        }
        renderer_.DrawIndexed();
    }
}
