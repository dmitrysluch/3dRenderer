#pragma once
#include <Eigen/Dense>
#include <memory>
#include <vector>

#include "BasicMaterial.h"
#include "SubMesh.h"

namespace renderer {

using namespace Eigen;
using namespace std;
/// Class representing a mesh (or 3d model object).
/// Mesh can contain several submeshes, each with different material
/// Meshes are immutable, and are supposed to be shared between several objects on the scene
class Mesh {
   public:
    Mesh() = delete;
    Mesh(const Mesh &) = delete;
    Mesh &operator=(const Mesh &) = delete;
    explicit Mesh(vector<SubMesh> &&submeshes) { swap(submeshes, submeshes_); }
    explicit Mesh(vector<SubMesh> &&submeshes, vector<shared_ptr<const BasicMaterial>> materials);
    [[nodiscard]] size_t size() const noexcept { return submeshes_.size(); }
    [[nodiscard]] const SubMesh &getSubmesh(size_t i) const noexcept { return submeshes_[i]; }
    /// Returns default material for submesh. Nullptr if no default material provided
    [[nodiscard]] shared_ptr<const BasicMaterial> getDefaultMaterial(size_t i) const noexcept;

   private:
    vector<SubMesh> submeshes_;
    vector<shared_ptr<const BasicMaterial>> default_materials_;
};
}  // namespace renderer
