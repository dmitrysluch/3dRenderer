#pragma once
#include <vector>
#include <Eigen/Dense>

namespace renderer {
using std::vector;
struct SubMesh {
    vector<Vector3f> vertices;
    vector<Vector3f> normals;
    vector<Vector2f> texcoords;
    vector<int> indices;
};
}