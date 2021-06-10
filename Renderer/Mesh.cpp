#include "include/Mesh.h"
renderer::Mesh::Mesh(std::vector<renderer::SubMesh> &&submeshes,
                     std::vector<std::shared_ptr<const renderer::BasicMaterial>> materials) {
    assert (materials.size() == submeshes.size() && "Default materials size should be the same as submeshes size");
    for (auto &pnt : materials) {
        assert(pnt && "Material pointer points to nullptr");
    }
    swap(submeshes, submeshes_);
    swap(materials, default_materials_);
}
std::shared_ptr<const renderer::BasicMaterial> renderer::Mesh::GetDefaultMaterial(size_t i) const noexcept {
    if (!default_materials_.empty()) {
        return default_materials_[i];
    } else {
        return nullptr;
    }
}
