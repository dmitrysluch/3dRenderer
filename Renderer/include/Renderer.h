#pragma once
#include <Eigen/Dense>
#include <vector>
#include <exception>
#include <algorithm>

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
    }
    [[nodiscard]] const vector<Vector2f> *GetTexcoords() const { return texcoords_; }
    void SetTexcoords(const vector<Vector2f> *texcoords) {
        texcoords_ = texcoords;
    }
    [[nodiscard]] const vector<int> *GetIndices() const { return indices_; }
    void SetIndices(const vector<int> *indices) { indices_ = indices; }
    void SetDepthBuffer(Buffer2D<float> *depth_buffer) { depth_buffer_ = depth_buffer; }
    [[nodiscard]] Buffer2D<float> *GetDepthBuffer() { return depth_buffer_; }
    void SetRenderTarget(Buffer2D<ColorRGBA32> *render_target) {
        render_target_ = render_target;
        viewport_matrix_ << render_target_->x()/2.f, 0.f, 0.f, render_target_->x()/2.f, 0.f, render_target_->y()/2.f, 0.f, render_target_->y()/2.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f;
    }
    [[nodiscard]] Buffer2D<ColorRGBA32> *GetRenderTarget() { return render_target_; }
    void SetObjectTransform(const Matrix4f &object) { object_ = object; }
    [[nodiscard]] const Matrix4f &GetObjectTransform() const { return object_; }
    void SetEyeAndProjectionMatrices(const Matrix4f &eye, const Matrix4f &projection) {
        eye_ = eye;
        projection_ = projection;
    }
    [[nodiscard]] float GetNearPlane() const { return near_plane_; }
    void SetNearPlane(float near_plane) { near_plane_ = near_plane; }
    void DrawIndexed();
    // Fills render target with specified color and depth with maximum depth
    void Clear(ColorRGBA32 color);
   private:
    struct VertexAttrs {
        Vector3f vertex_;
        Vector3f normal_;
        Vector2f texcoord_;
        float inv_z_;
        VertexAttrs() = default;
        VertexAttrs(const Vector3f &vertex) : vertex_(vertex) {}
        static VertexAttrs Interpolate(const VertexAttrs& a, const VertexAttrs& b, float t, bool interpolate_normals,
                                       bool interpolate_texcoords);
    };

    Matrix4f object_, eye_, projection_, viewport_matrix_;
    float near_plane_;

    const BasicMaterial *material_ = nullptr;

    const vector<Vector3f> *vertices_=nullptr;
    const vector<Vector3f> *normals_ = nullptr;
    const vector<Vector2f> *texcoords_ = nullptr;
    const vector<int> *indices_ = nullptr;
    
    vector<Vector4f> screen_space_vertices_;
    vector<Vector3f> transformed_normals_;

    Buffer2D<float> *depth_buffer_=nullptr;
    Buffer2D<ColorRGBA32> *render_target_=nullptr;

    void TransformVertices();
    void DrawAllAfterTransformation();
    /// Clips triangle on indices a, b, c
    /// @param [in] a, b, c - indices of triangle vertices
    /// @param [out] attrs - array of at least 6 elements, vertices and attributes of resulting triangle (or tw triangles)
    /// @returns Returns number of vertices in resulting polygon
    inline int ClipTriangle (int a, int b, int c, VertexAttrs *attrs) const;
    inline void DrawTriangle(int a, int b, int c);
    inline void DrawTriangleClipped(VertexAttrs &a, VertexAttrs &b, VertexAttrs &c);
    inline void DrawPixel(const Vector2i &view_port_vertex, float depth, const Vector3f &normal, const Vector2f &texcoord);
    static inline bool CullFace(const Vector3f &a, const Vector3f &b, const Vector3f &c);
    inline bool DepthTest(const Vector2i &a, float depth);
};
}  // namespace renderer