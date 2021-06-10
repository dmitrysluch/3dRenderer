#pragma once
#include <Eigen/Dense>
#include <functional>

namespace renderer {
struct SceneTransform {
    Eigen::Vector3f position = {0, 0, 0};
    Eigen::Quaternionf rotation = Eigen::Quaternionf::Identity();
    Eigen::Vector3f scale = {1, 1, 1};
    [[nodiscard]] Eigen::Matrix4f GetTransformMatrix() const {
        Eigen::Projective3f transform;
        transform.fromPositionOrientationScale(position, rotation, scale);
        return transform.matrix();
    }
};
class SceneTransformProxy {
   public:
    SceneTransformProxy(SceneTransform *transform, std::function<void()> callback) : transform_(transform), callback_(std::move(callback)) {}
    void SetPosition(const Eigen::Vector3f &position) {
        transform_->position = position;
        if (callback_) callback_();
    }
    void SetRotation(const Eigen::Quaternionf &rotation) {
        transform_->rotation = rotation;
        if (callback_) callback_();
    }
    void SetScale(const Eigen::Vector3f &scale) {
        transform_->scale = scale;
        if (callback_) callback_();
    }
   private:
    SceneTransform *transform_;
    std::function<void()> callback_;
};
} // namespace renderer