#include "include/MathHelpers.h"

Eigen::Matrix4f renderer::MathHelpers::GetProjectionMatrix(float fov_angle, float aspect_ratio, float far_clip_plane,
                                          float near_clip_plane) {
    Eigen::Matrix4f projection;
    projection << 1 / tanf(fov_angle / 2) / aspect_ratio, 0.f, 0.f, 0.f, 0.f, 1 / tanf(fov_angle / 2), 0.f, 0.f, 0.f,
        0.f, (far_clip_plane + near_clip_plane) / (far_clip_plane - near_clip_plane),
        2 * near_clip_plane * far_clip_plane / (near_clip_plane - far_clip_plane), 0.f, 0.f, 1.f, 0.f;
    return projection;
}