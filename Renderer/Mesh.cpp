#include "include/Mesh.h"
renderer::Mesh::Mesh(std::vector<renderer::SubMesh> &&submeshes,
                     std::vector<std::shared_ptr<const renderer::BasicMaterial>> materials) {
    if (materials.size() == submeshes.size()) {
        for (auto &pnt : materials) {
            if (!pnt) {
                throw invalid_argument("Material pointer points to nullptr");
            }
        }
        swap(submeshes, submeshes_);
        swap(materials, default_materials_);
    } else {
        throw invalid_argument("Default materials size should be the same as submeshes size");
    }
}
std::shared_ptr<const renderer::BasicMaterial> renderer::Mesh::getDefaultMaterial(size_t i) const noexcept {
    if (!default_materials_.empty()) {
        return default_materials_[i];
    } else {
        return nullptr;
    }
}
