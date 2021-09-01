#include "include/Kernel.h"

#include <algorithm>

using std::string;
renderer::Kernel::Kernel(std::unique_ptr<renderer::IView> view) : view_(std::move(view)) {
    renderer_.SetRenderTarget(&view_->GetRenderTarget());
    renderer_.SetDepthBuffer(&view_->GetDepthBuffer());
}
void renderer::Kernel::OnUpdate() {
    if (active_) {
        Render();
    }
}
void renderer::Kernel::Render() {
    assert(!cameras_.empty() && "You need at least one camera to renderer the scene");
    assert(active_camera_id_ >= 0 && active_camera_id_ < cameras_.size() && "Invalid active camera");
    renderer_.Clear({240, 250, 255, 100});  // TODO: pass color as param
    renderer_.SetEyeAndProjectionMatrices(
        cameras_[active_camera_id_]->GetTransform().GetTransformMatrix().inverse().matrix(),
        cameras_[active_camera_id_]->GetProjectionMatrix());
    renderer_.SetNearPlane(cameras_[active_camera_id_]->GetNearClipPlane());
    Globals globals;
    globals.camera = cameras_[active_camera_id_]->GetTransform().position;
    globals.lights.resize(lights_.size());
    for (int i = 0; i < lights_.size(); ++i) {
        globals.lights[i] = lights_[i]->GetDirAndColor();
    }
    renderer_.SetGlobals(&globals);
    for (const auto& object : objects_) {
        renderer_.SetObjectTransform(object->GetTransform().GetTransformMatrix());
        auto mesh = object->GetMesh();
        assert((!object->GetMaterialOverrides().empty() || mesh->HasDefaultMaterials()) &&
               "Mesh doesn't provide default materials, set material overrides for object");
        auto materialOverrides = object->GetMaterialOverrides();
        for (int i = 0; i < mesh->Size(); ++i) {
            const BasicMaterial* materialPtr;
            if (!materialOverrides.empty()) {
                materialPtr = materialOverrides[i].get();
            } else {
                materialPtr = mesh->GetDefaultMaterial(i).get();
            }
            renderer_.SetMaterial(materialPtr);
            const auto& submesh = mesh->GetSubmesh(i);
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
    view_->Blit();
}
void renderer::Kernel::SetActive(bool active) {
    active_ = active;
    if (active_) {
        Render();
    }
}
