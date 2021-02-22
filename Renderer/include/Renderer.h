#pragma once
#include <Eigen/Dense>
#include <vector>
#include <exception>

#include "BasicMaterial.h"
#include "MathHelpers.h"

namespace renderer {
using namespace Eigen;
using namespace std;
class IndicedTriangleRenderer {
   public:
    const BasicMaterial *GetMaterial() const { return material_; }
    void SetMaterial(const BasicMaterial *material) { material_ = material; }
    const Globals *GetGlobals() const { return globals_; }
    void SetGlobals(const Globals *globals) { globals_ = globals; }
    void SetVertexBuffer(const vector<Vector3f> *vertices) {
        vertices_ = vertices;
        screen_space_vertices_.resize(vertices->size());
    }
    const vector<Vector3f> *GetVertexBuffer() const { return vertices_; }
    const vector<Vector3f> *GetNormals() const { return normals_; }
    void SetNormals(const vector<Vector3f> *normals) {
        normals_ = normals;
        transformed_normals_.resize(normals_->size());
        inverse_camera_space_z_.resize(normals_->size());
    }
    const vector<Vector2f> *GetTexcoords() const { return texcoords_; }
    void SetTexcoords(const vector<Vector2f> *texcoords) {
        texcoords_ = texcoords;
        inverse_camera_space_z_.resize(texcoords_->size());
    }
    const vector<int> *GetIndices() const { return indices_; }
    void SetIndices(const vector<int> *indices) { indices_ = indices; }
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
    struct VertexAttrs {
        Vector3f vertex_;
        Vector3f normal_;
        Vector2f texcoord_;
        float inv_z_;
        VertexAttrs(const Vector3f &vertex) : vertex_(vertex) {}
        static VertexAttrs Interpolate(const VertexAttrs& a, const VertexAttrs& b, float t, bool interpolate_normals,
                                       bool interpolate_texcoords);
    };

    Matrix4f object_, eye_, projection_, viewport_matrix_;
    Vector2i viewport_;

    const BasicMaterial *material_ = nullptr;
    const Globals *globals_ = nullptr;

    const vector<Vector3f> *vertices_=nullptr;
    const vector<Vector3f> *normals_ = nullptr;
    const vector<Vector2f> *texcoords_ = nullptr;
    const vector<int> *indices_ = nullptr;
    
    vector<Vector3f> screen_space_vertices_;
    vector<Vector3f> transformed_normals_;
    vector<float> inverse_camera_space_z_;

    char *depth_buffer_=nullptr;
    char *render_target_=nullptr;

    void TransformVertices();
    void DrawAllAfterTransformation();
    void DrawTriangle(int a, int b, int c);
    /// <summary>Transforms vertex attributes is they are used by material then draws pixel using material</summary>
    void DrawPixel(const Vector2i &view_port_vertex, float depth, const Vector3f &normal, const Vector2f &texcoord);
    bool CullFace(const Vector3f &a, const Vector3f &b, const Vector3f &c);
    bool DepthAndOwnershipTest(const Vector2i &a, float depth);
};
}  // namespace renderer