#include "include/Camera.h"

using namespace Eigen;
Matrix4f renderer::PerspectiveCamera::GetProjectionMatrix() const {
    Matrix4f projection;
    projection << 1 / tanf(fov_angle_ / 2) / aspect_ratio_, 0.f, 0.f, 0.f, 0.f, 1 / tanf(fov_angle_ / 2), 0.f, 0.f, 0.f,
        0.f, (far_clip_plane_ + near_clip_plane_) / (far_clip_plane_ - near_clip_plane_),
        2 * near_clip_plane_ * far_clip_plane_ / (near_clip_plane_ - far_clip_plane_), 0.f, 0.f, 1.f, 0.f;
    return projection;
}
