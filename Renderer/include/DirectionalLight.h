#pragma once
#include "Color.h"
#include "Globals.h"
#include "Transform.h"

namespace renderer {
class Kernel;
class DirectionalLight {
   public:
    DirectionalLight() = delete;
    [[nodiscard]] const std::string &GetName() const;
    [[nodiscard]] const Eigen::Vector3f &GetDirection() const;
    void SetDirection(const Eigen::Vector3f &direction);
    [[nodiscard]] const Eigen::Vector3f &GetColor() const;
    void SetColor(const Eigen::Vector3f &color);
    [[nodiscard]] LightDirCol GetDirAndColor() const;

   private:
    DirectionalLight(Kernel *kernel, std::string name, const Eigen::Vector3f &direction, const Eigen::Vector3f &color)
        : kernel_(kernel), name_(std::move(name)), direction_(direction), color_(color) {
        direction_.normalize();
    }
    static DirectionalLight *InstantiateWithKernel(Kernel *kernel, std::string name, const Eigen::Vector3f &direction,
                                                   const Eigen::Vector3f &color);
    static DirectionalLight *GetComponentInKernel(Kernel &kernel, const std::string &name);
    static const DirectionalLight *GetComponentInKernel(const Kernel &kernel, const std::string &name);

    Kernel *kernel_;
    std::string name_;
    Eigen::Vector3f direction_;
    Eigen::Vector3f color_;
    friend class Kernel;
};
}  // namespace renderer