#include <fstream>

#include "Camera.h"
#include "Renderer.h"

using namespace renderer;
using namespace Eigen;

// Better save space (not)
const int kX = 1000;
const int kY = 500;
const int kPixelSz = 4;
char rend_targ[kX * kY * kPixelSz];
char depth[kX * kY * 4];

int main(int argc, char **argv) {
  Vector2i vp(kX, kY);
  memset(rend_targ, 100, kX * kY * kPixelSz);
  fill(&reinterpret_cast<float *>(depth)[0],
       &reinterpret_cast<float *>(depth)[kX * kY], 1.f);
  vector<Vector3f> vertices = {{-2, -2, -2}, {2, 2, -2},  {-2, 2, -2},
                               {2, -2, -2},  {-2, -2, 2}, {2, 2, 2},
                               {-2, 2, 2},   {2, -2, 2}};
  vector<int> indices = {0, 1, 2, 0, 3, 1, 4, 6, 5, 4, 5, 7, 3, 5, 1, 3, 7, 5,
                         0, 2, 6, 0, 6, 4, 2, 5, 6, 2, 1, 5, 0, 4, 7, 0, 7, 3};
  IndicedTriangleRenderer rend;
  rend.SetRenderTarget(rend_targ);
  rend.SetDepthBuffer(depth);
  rend.SetVertexBuffer(&vertices);
  rend.SetIndexBuffer(&indices);
  rend.SetViewport(vp);
  rend.SetObjectTransform(Matrix4f::Identity());

  PerspectiveCamera camera;
  camera.SetFovAngleDeg(60);
  camera.SetViewport(vp);
  camera.SetFarClipPlane(30);
  camera.SetNearClipPlane(1);
  Projective3f transform = static_cast<Projective3f>(Translation3f(0, 1, -15));
  rend.SetEyeAndProjectionMatrices(transform.inverse().matrix(), camera.GetProjectionMatrix());
  rend.DrawIndexed();
  rend.SetObjectTransform(static_cast<Projective3f>(Translation<float, 3>(1, 1, -1)).matrix());
  rend.DrawIndexed();
  ofstream fout("out.raw", ios::binary | ios::out | ios::trunc);
  fout.write(rend_targ, kX * kY * kPixelSz);
  fout.close();
  fout.open("depth.raw", ios::binary | ios::out | ios::trunc);
  for (int i = 0; i < kX * kY; ++i) {
    float flt = reinterpret_cast<float *>(depth)[i];
    fout.put(255 * flt); 
  }
  fout.close();
}