#pragma once
#include <Eigen/Dense>

namespace renderer {
using namespace Eigen;
class PerspectiveCamera {
   public:
    float GetFovAngle() const { return fov_angle_;}
    void SetFovAngle(float fov_angle) { fov_angle_ = fov_angle; }
    float GetFovAngleDeg() const { return fov_angle_ / EIGEN_PI * 180.f; }
    void SetFovAngleDeg(float fov_angle) { fov_angle_ = fov_angle * EIGEN_PI / 180.f; }
    float GetAspectRatio() const { return aspect_ratio_; }
    void SetAspectRatio(float aspect_ratio) { aspect_ratio_ = aspect_ratio; }
    void SetViewport(Vector2i viewport) { SetAspectRatio(static_cast<float>(viewport.x()) / viewport.y()); }
    float GetFarClipPlane() const { return far_clip_plane_; }
    void SetFarClipPlane(float far_clip_plane) { far_clip_plane_ = far_clip_plane; }
    float GetNearClipPlane() const { return near_clip_plane_; }
    void SetNearClipPlane(float near_clip_plane) { near_clip_plane_ = near_clip_plane; }
    Matrix4f GetProjectionMatrix() const;
   private:
    float fov_angle_ = 30, aspect_ratio_ = 16.f / 9, far_clip_plane_ = 100, near_clip_plane_ = 10;
};
}  // namespace renderer