#pragma once
#include <Eigen/Dense>
#include <vector>
#include <memory>
#include "BasicMaterial.h"

namespace renderer {

using namespace Eigen;
using namespace std;

struct SubMesh {
    unique_ptr<BasicMaterial> material = nullptr;
    vector<Vector3f> vertices;
    vector<Vector3f> normals;
    vector<Vector2f> texcoords;
    vector<int> indices;
};
}
