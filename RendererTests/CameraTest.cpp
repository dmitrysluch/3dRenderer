#include "gtest/gtest.h"
#include "Camera.h"

using namespace renderer;

#define TEST_EPS 1e-6

class CameraTest : public testing::Test {
protected:
    PerspectiveCamera camera_;
    void SetUp() override {
        camera_.SetFarClipPlane(10);
        camera_.SetNearClipPlane(1);
        camera_.SetAspectRatio(2);
        camera_.SetFovAngleDeg(90);
    }
};

TEST_F(CameraTest, TestProjectionXY) {
    auto matrix = camera_.GetProjectionMatrix();
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

TEST_F(CameraTest, TestProjectionZ) {
  auto matrix = camera_.GetProjectionMatrix();
  Vector3f vec = (matrix * Vector4f(0, 0, 1, 1)).hnormalized();
  EXPECT_NEAR(vec.z(), -1, TEST_EPS);
  vec = (matrix * Vector4f(0, 0, 10, 1)).hnormalized();
  EXPECT_NEAR(vec.z(), 1, TEST_EPS);
  vec = (matrix * Vector4f(0, 0, 20.0f/11, 1)).hnormalized();
  EXPECT_NEAR(vec.z(), 0, TEST_EPS);
}