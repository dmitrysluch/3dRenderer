#pragma once
#include <Eigen/Dense>
#include <vector>

namespace renderer {
using namespace Eigen;
using namespace std;
class IndicedTriangleRenderer {
   public:
    void SetVertexBuffer(const vector<Vector3f> *vertices) {
        vertices_ = vertices;
        window_space_vertices_.resize(vertices->size());
    }
    const vector<Vector3f> *GetVertexBuffer() const { return vertices_; }
    void SetIndexBuffer(const vector<int> *indices) { indices_ = indices; }
    const vector<int> *GetIndexBuffer() const { return indices_; }
    void SetDepthBuffer(char *depth_buffer) { depth_buffer_ = depth_buffer; }
    char *GetDepthBuffer() const { return depth_buffer_; }
    void SetRenderTarget(char *render_target) { render_target_ = render_target; }
    char *GetRenderTarget() const { return render_target_; }
    void SetViewport(const Vector2i &viewport) {
        viewport_ = viewport;
        viewport_matrix_ << viewport_.x()/2, 0.f, 0.f, viewport_.x()/2, 0.f, viewport_.y()/2, 0.f, viewport_.y()/2, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f;
    }
    Vector2i GetViewport() const { return viewport_; }
    void SetObjectTransform(const Matrix4f &object) { object_ = object; }
    const Matrix4f &GetObjectTransform() const { return object_; }
    void SetEyeAndProjectionMatrices(const Matrix4f &eye, const Matrix4f &projection) {
        eye_ = eye;
        projection_ = projection;
    }
    void DrawIndexed();

   private:
    Matrix4f object_, eye_, projection_, viewport_matrix_;
    Vector2i viewport_;
    const vector<Vector3f> *vertices_=nullptr;
    const vector<int> *indices_=nullptr;
    vector<Vector3f> window_space_vertices_;
    char *depth_buffer_=nullptr;
    char *render_target_=nullptr;

    void TransformVertices();
    void DrawPixels();
    void DrawPixel(const Vector2i &view_port_vertex, float depth);
    bool CullFace(const Vector3f &a, const Vector3f &b, const Vector3f &c);
    bool DepthAndOwnershipTest(const Vector2i &a, float depth);
    template <typename T>
    static T Lerp(const T &from, const T &to, float t) {
        return from * (1.f - t) + to * t;
    }
};
}  // namespace renderer