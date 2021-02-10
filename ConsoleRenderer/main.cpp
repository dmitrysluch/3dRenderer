#include <fstream>

#include "AssimpBindings.h"
#include "Mesh.h"
#include "Camera.h"
#include "DrawNormals.h"
#include "Renderer.h"
#include "TexturedUnlitMaterial.h"

using namespace renderer;
using namespace Eigen;

// Better save space (not)
const int kX = 1920;
const int kY = 1080;
const int kPixelSz = 4;
char rend_targ[kX * kY * kPixelSz];
char depth[kX * kY * 4];

int main(int argc, char **argv) {
  Vector2i vp(kX, kY);
  memset(rend_targ, 100, kX * kY * kPixelSz);
  fill(&reinterpret_cast<float *>(depth)[0],
       &reinterpret_cast<float *>(depth)[kX * kY], 1.f);
  vector<SubMesh> mesh = AssimpBindings::LoadMeshFromFile("mesh.fbx");
  IndicedTriangleRenderer rend;
  rend.SetRenderTarget(rend_targ);
  rend.SetDepthBuffer(depth);
  rend.SetViewport(vp);
  rend.SetObjectTransform(Matrix4f::Identity());

  PerspectiveCamera camera;
  camera.SetFovAngleDeg(60);
  camera.SetViewport(vp);
  camera.SetFarClipPlane(100);
  camera.SetNearClipPlane(1);
  Projective3f transform = static_cast<Projective3f>(Translation3f(0, 2, -12));
  rend.SetEyeAndProjectionMatrices(transform.inverse().matrix(),
                                   camera.GetProjectionMatrix());
  rend.SetObjectTransform(Matrix4f::Identity());
  for (const auto &m : mesh) {
    rend.SetVertexBuffer(&m.vertices);
    rend.SetNormals(&m.normals);
    rend.SetTexcoords(&m.texcoords);
    rend.SetIndexBuffer(&m.indices);
    rend.SetMaterial(m.material.get());
    rend.DrawIndexed();
  }

  // Texture2D texture_2d(500, 500);
  // ifstream fin("texture.raw", ios::binary | ios::in);
  // fin.read(texture_2d.RawData(), 500 * 500 * 4);
  // fin.close();
  // TexturedUnlitMaterial textured_mat(texture_2d);
  // rend.SetMaterial(&textured_mat);

  ofstream fout("frame.raw", ios::binary | ios::out | ios::trunc);
  fout.write(rend_targ, kX * kY * kPixelSz);
  fout.close();
  system("wsl -e rm -f ./frame.png");
  system(
      "wsl -e ffmpeg -f rawvideo -s 1920x1080 -pix_fmt rgba -i ./frame.raw "
      "./frame.png");
  system("wsl -e rm -f ./frame.raw");
}