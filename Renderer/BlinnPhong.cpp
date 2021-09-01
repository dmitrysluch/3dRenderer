#include "include/BlinnPhong.h"

namespace renderer {
Vector4f BlinnPhong::DrawPixel(const Vector2i &window_space_vertex, const Vector3f &normal, const Vector2f &texcoord,
                          const Vector3f &world_pos, const Globals *globals) const {
    auto albedo = BilinearSampler::SamplePixelBilinear(albedo_, texcoord);
    Vector3f diff{0, 0, 0}, spec{0, 0, 0};
    for (const auto& [dir, col] : globals->lights) {
        if (normal.dot(-dir) > 0.0f) {
            diff += Vector3f(col.x(), col.y(), col.z())* normal.dot(-dir);
            auto hw = (-dir, (globals->camera - world_pos).normalized()).normalized();
            spec += Vector3f(col.x(), col.y(), col.z()) *
                powf(max(0.0f, hw.dot(normal)), specular_power_);
        }
    }
    diff = MathHelpers::Clamp(diff);
    spec = MathHelpers::Clamp(spec);
    diff = diff.cwiseProduct(Vector3f(albedo.x(), albedo.y(), albedo.z()));
    diff += spec * specular_multiplier_;
    return Vector4f(diff.x(), diff.y(), diff.z(), albedo.w());
}
}  // namespace renderer