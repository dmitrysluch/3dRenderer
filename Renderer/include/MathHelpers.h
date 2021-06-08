#pragma once
#include <Eigen/Dense>
namespace renderer {
class MathHelpers {
   public:
    template <typename T>
    static T Lerp(const T &from, const T &to, float t) {
        return from * (1.f - t) + to * t;
    }
    template <typename T>
    static T PerspInterpolate(const T &from, float from_inv_z, const T &to, float to_inv_z, float inv_z, float t) {
        T corr = Lerp(static_cast<T>(from * from_inv_z), static_cast<T>(to * to_inv_z), t) / inv_z;
        return corr;
    }
    static float Clamp(float val) { return val > 1.0f ? 1.0f : (val < 0.0f ? 0.0f : val); }
    template <typename T, int col, int row>
    static Eigen::Matrix<T, col, row> Clamp(const Eigen::Matrix<T, col, row> &mat) {
        Eigen::Matrix<T, col, row> res = mat;
        for (int i = 0; i < res.cols(); ++i) {
            for (int j = 0; j < res.cols(); ++j) {
                res(i, j) = Clamp(res(i, j));
            }
        }
        return res;
    }
    static Eigen::Matrix4f GetProjectionMatrix(float fov_angle, float aspect_ratio, float far_clip_plane,
                                               float near_clip_plane);
};
}  // namespace renderer