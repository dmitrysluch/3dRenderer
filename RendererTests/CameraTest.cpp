#include "gtest/gtest.h"
#include "MathHelpers.h"

using namespace renderer;
using namespace Eigen;

#define TEST_EPS 1e-6


TEST(TestProjection, TestProjectionXY) {
    auto matrix = MathHelpers::GetProjectionMatrix(90.0 / 180 * EIGEN_PI, 2, 10, 1);
    Vector3f vec = (matrix * Vector4f(0, 0, 1, 1)).hnormalized();
    EXPECT_NEAR(vec.x(), 0, TEST_EPS);
    EXPECT_NEAR(vec.y(), 0, TEST_EPS);
    vec = (matrix * Vector4f(2, 1, 1, 1)).hnormalized();
    EXPECT_NEAR(vec.x(), 1, TEST_EPS);
    EXPECT_NEAR(vec.y(), 1, TEST_EPS);
    vec = (matrix * Vector4f(10, 5, 5, 1)).hnormalized();
    EXPECT_NEAR(vec.x(), 1, TEST_EPS);
    EXPECT_NEAR(vec.y(), 1, TEST_EPS);
    vec = (matrix * Vector4f(0.6, 0.7, 1, 1)).hnormalized();
    EXPECT_NEAR(vec.x(), 0.3, TEST_EPS);
    EXPECT_NEAR(vec.y(), 0.7, TEST_EPS);
}

TEST(TestProjection, TestProjectionZ) {
  auto matrix = MathHelpers::GetProjectionMatrix(90.0 / 180 * EIGEN_PI, 2, 10, 1);
  Vector3f vec = (matrix * Vector4f(0, 0, 1, 1)).hnormalized();
  EXPECT_NEAR(vec.z(), -1, TEST_EPS);
  vec = (matrix * Vector4f(0, 0, 10, 1)).hnormalized();
  EXPECT_NEAR(vec.z(), 1, TEST_EPS);
  vec = (matrix * Vector4f(0, 0, 20.0f/11, 1)).hnormalized();
  EXPECT_NEAR(vec.z(), 0, TEST_EPS);
}