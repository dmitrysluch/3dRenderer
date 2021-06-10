#pragma once
#include <Eigen/Dense>
#include <memory>

#include "Globals.h"

namespace renderer {
using namespace Eigen;

/// Basic class for creating material
/// Materials must be immutable, otherwise it would be impossible to detect material changing and rerender scene
class BasicMaterial {
   public:
    virtual ~BasicMaterial() = default;
    [[nodiscard]] virtual bool RequireNormals() const = 0;
    [[nodiscard]] virtual bool RequireTexcoords() const = 0;
    [[nodiscard]] virtual bool RequireWorldPos() const = 0;
    [[nodiscard]] virtual Vector4f DrawPixel(const Vector2i& window_space_vertex, const Vector3f& normal,
                                             const Vector2f& texcoord, const Vector3f& world_pos,
                                             const Globals* globals) const = 0;
};
using MaterialVec = std::vector<std::shared_ptr<const BasicMaterial>>;
}  // namespace renderer
