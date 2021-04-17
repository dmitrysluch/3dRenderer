#pragma once
#include <Eigen/Dense>
#include <vector>
#include <exception>

#include "BasicMaterial.h"
#include "MathHelpers.h"
#include "Buffer2D.h"
#include "Color.h"

namespace renderer {
using namespace Eigen;
using namespace std;
class Renderer {
   public:
    [[nodiscard]] const BasicMaterial *GetMaterial() const { return material_; }
    void SetMaterial(const BasicMaterial *material) { material_ = material; }
    void SetVertices(const vector<Vector3f> *vertices) {
        vertices_ = vertices;
        screen_space_vertices_.resize(vertices->size());
    }
    [[nodiscard]] const vector<Vector3f> *GetVertices() const { return vertices_; }
    [[nodiscard]] const vector<Vector3f> *GetNormals() const { return normals_; }
    void SetNormals(const vector<Vector3f> *normals) {
        normals_ = normals;
        transformed_normals_.resize(normals_->size());
        inverse_camera_space_z_.resize(normals_->size());
    }
    [[nodiscard]] const vector<Vector2f> *GetTexcoords() const { return texcoords_; }
    void SetTexcoords(const vector<Vector2f> *texcoords) {
        texcoords_ = texcoords;
        inverse_camera_space_z_.resize(texcoords_->size());
    }
    [[nodiscard]] const vector<int> *GetIndices() const { return indices_; }
    void SetIndices(const vector<int> *indices) { indices_ = indices; }
    void SetDepthBuffer(Buffer2D<float> *depth_buffer) { depth_buffer_ = depth_buffer; }
    [[nodiscard]] Buffer2D<float> *GetDepthBuffer() { return depth_buffer_; }
    void SetRenderTarget(Buffer2D<ColorRGBA32> *render_target) { render_target_ = render_target; }
    [[nodiscard]] Buffer2D<ColorRGBA32> *GetRenderTarget() { return render_target_; }
    void SetViewport(const Vector2i &viewport) {
        viewport_ = viewport;
        viewport_matrix_ << viewport_.x()/2, 0.f, 0.f, viewport_.x()/2, 0.f, viewport_.y()/2, 0.f, viewport_.y()/2, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f;
    }
    [[nodiscard]] Vector2i GetViewport() const { return viewport_; }
    void SetObjectTransform(const Matrix4f &object) { object_ = object; }
    [[nodiscard]] const Matrix4f &GetObjectTransform() const { return object_; }
    void SetEyeAndProjectionMatrices(const Matrix4f &eye, const Matrix4f &projection) {
        eye_ = eye;
        projection_ = projection;
    }
    void DrawIndexed();
    // Fills render target with specified color and depth with maximum depth
    void Clear(ColorRGBA32 color);
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

    const vector<Vector3f> *vertices_=nullptr;
    const vector<Vector3f> *normals_ = nullptr;
    const vector<Vector2f> *texcoords_ = nullptr;
    const vector<int> *indices_ = nullptr;
    
    vector<Vector3f> screen_space_vertices_;
    vector<Vector3f> transformed_normals_;
    vector<float> inverse_camera_space_z_;

    Buffer2D<float> *depth_buffer_=nullptr;
    Buffer2D<ColorRGBA32> *render_target_=nullptr;

    void TransformVertices();
    void DrawAllAfterTransformation();
    void DrawTriangle(int a, int b, int c);
    /// <summary>Transforms vertex attributes is they are used by material then draws pixel using material</summary>
    void DrawPixel(const Vector2i &view_port_vertex, float depth, const Vector3f &normal, const Vector2f &texcoord);
    static bool CullFace(const Vector3f &a, const Vector3f &b, const Vector3f &c);
    bool DepthAndOwnershipTest(const Vector2i &a, float depth);
};
}  // namespace renderer