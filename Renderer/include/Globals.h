#pragma once
#include <Eigen/Dense>
#include <vector>

namespace renderer {
struct LightDirCol {
    LightDirCol() = default;
    LightDirCol(const Eigen::Vector3f &dir, const Eigen::Vector3f &color) : dir(dir), color(color) {}
    Eigen::Vector3f dir;
    Eigen::Vector3f color;
};
struct Globals {
    std::vector<LightDirCol> lights;
    Eigen::Vector3f camera;
};
}  // namespace renderer