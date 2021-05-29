#pragma once
#include <Eigen/Dense>
#include <string>

#include "MathHelpers.h"
#include "Transform.h"

namespace renderer {
class Kernel;
class Camera {
   public:
    Camera() = delete;
    [[nodiscard]] float GetFovAngle() const { return fov_angle_; }
    void SetFovAngle(float fov_angle) { fov_angle_ = fov_angle; }
    [[nodiscard]] float GetFovAngleDeg() const { return fov_angle_ / static_cast<float>(EIGEN_PI) * 180.f; }
    void SetFovAngleDeg(float fov_angle) { fov_angle_ = fov_angle * static_cast<float>(EIGEN_PI) / 180.f; }
    [[nodiscard]] float GetAspectRatio() const { return aspect_ratio_; }
    void SetAspectRatio(float aspect_ratio) { aspect_ratio_ = aspect_ratio; }
    void SetViewport(Eigen::Vector2i viewport) { SetAspectRatio(static_cast<float>(viewport.x()) / viewport.y()); }
    [[nodiscard]] float GetFarClipPlane() const { return far_clip_plane_; }
    void SetFarClipPlane(float far_clip_plane) { far_clip_plane_ = far_clip_plane; }
    [[nodiscard]] float GetNearClipPlane() const { return near_clip_plane_; }
    void SetNearClipPlane(float near_clip_plane) { near_clip_plane_ = near_clip_plane; }
    [[nodiscard]] Eigen::Matrix4f GetProjectionMatrix() const {
        return MathHelpers::GetProjectionMatrix(fov_angle_, aspect_ratio_, far_clip_plane_, near_clip_plane_);
    };
    [[nodiscard]] const SceneTransform &GetTransform() const;
    [[nodiscard]] SceneTransformProxy TransformProxy();
    void SetTransform(const SceneTransform &transform);
    [[nodiscard]] const std::string &GetName() const { return name_; }

   private:
    Camera(Kernel *kernel, std::string name);
    static Camera *InstantiateWithKernel(Kernel *kernel, std::string name);

    Kernel *kernel_;
    std::string name_;
    SceneTransform transform_;
    float fov_angle_ = 30, aspect_ratio_ = 16.f / 9, far_clip_plane_ = 100, near_clip_plane_ = 10;

    friend class Kernel;
};
}  // namespace renderer