#include "Renderer.h"

using namespace Eigen;

void renderer::IndicedTriangleRenderer::DrawIndexed() {
    if (!material_) {
        throw invalid_argument("Material not set");
    }
    if (!render_target_) {
        throw invalid_argument("Render target not set");
    }
    if (!depth_buffer_) {
        throw invalid_argument("Depth buffer not set");
    }
    if(!vertices_) {
        throw invalid_argument("Vertices not set");
    }
    if(!indices_) {
        throw invalid_argument("Indices not set");
    }
    if (material_->RequireNormals() && (!normals_ || normals_->size() != vertices_->size())) {
        throw invalid_argument("Normals not set or doesn't match vertices size");
    }
    if(material_->RequireTexcoords() && (!texcoords_ || texcoords_->size() != vertices_->size())) {
        throw invalid_argument("Texcoords not set or doesn't match vertices size");
    }
    TransformVertices();
    DrawAllAfterTransformation();
}

void renderer::IndicedTriangleRenderer::TransformVertices() {
    for (int i = 0; i < vertices_->size(); ++i) {
        auto camera_space_coords = eye_ * (object_ * (*vertices_)[i].homogeneous());
        if (material_->RequireNormals() || material_->RequireTexcoords()) {
            inverse_camera_space_z_[i] = 1 / camera_space_coords.z();
        }
        screen_space_vertices_[i] = (viewport_matrix_ * (projection_ * camera_space_coords)).hnormalized();
    }
    if (material_->RequireNormals()) {
        for (int i = 0; i < normals_->size(); ++i) {
            transformed_normals_[i] = object_.topLeftCorner(3, 3).transpose().inverse() * (*normals_)[i];
        }
    }
}


renderer::IndicedTriangleRenderer::VertexAttrs renderer::IndicedTriangleRenderer::VertexAttrs::Interpolate(
    const VertexAttrs& a, const VertexAttrs& b, float t, bool interpolate_normals, bool interpolate_texcoords) {
    VertexAttrs res(MathHelpers::Lerp(a.vertex_, b.vertex_, t));
    if (interpolate_normals || interpolate_texcoords) {
        res.inv_z_ = MathHelpers::Lerp(a.inv_z_, b.inv_z_, t);
    }
    if (interpolate_normals) {
        res.normal_ =
            MathHelpers::PerspInterpolate(a.normal_, a.inv_z_, b.normal_, b.inv_z_, res.inv_z_, t);
    }
    if (interpolate_texcoords) {
        res.texcoord_ =
            MathHelpers::PerspInterpolate(a.texcoord_, a.inv_z_, b.texcoord_, b.inv_z_, res.inv_z_, t);
    }
    return res;
}

void renderer::IndicedTriangleRenderer::DrawTriangle(int ind_a, int ind_b, int ind_c)
{
    VertexAttrs a(screen_space_vertices_[ind_a]);
    VertexAttrs b(screen_space_vertices_[ind_b]);
    VertexAttrs c(screen_space_vertices_[ind_c]);
    if (!CullFace(a.vertex_, b.vertex_, c.vertex_)) {
        return;
    }
    if (material_->RequireNormals() || material_->RequireTexcoords()) {
        a.inv_z_=inverse_camera_space_z_[ind_a];
        b.inv_z_ = inverse_camera_space_z_[ind_b];
        c.inv_z_ = inverse_camera_space_z_[ind_c];
        if (material_->RequireNormals()) {
            a.normal_ = transformed_normals_[ind_a];
            b.normal_ = transformed_normals_[ind_b];
            c.normal_ = transformed_normals_[ind_c];
        }
        if (material_->RequireTexcoords()) {
            a.texcoord_ = (*texcoords_)[ind_a];
            b.texcoord_ = (*texcoords_)[ind_b];
            c.texcoord_ = (*texcoords_)[ind_c];
        }
    }
    if (a.vertex_.y() < b.vertex_.y()) {
        swap(a, b);
    }
    if (a.vertex_.y() < c.vertex_.y()) {
        swap(a, c);
    }
    if (b.vertex_.y() < c.vertex_.y()) {
        swap(b, c);
    }
    VertexAttrs d = VertexAttrs::Interpolate(a, c, (b.vertex_.y() - a.vertex_.y()) / (c.vertex_.y() - a.vertex_.y()), material_->RequireNormals(), material_->RequireTexcoords());
    if (d.vertex_.x() < b.vertex_.x()) {
        swap(b, d);
    }
    for (int i = a.vertex_.y(); i >= b.vertex_.y(); --i) {
        const float ty = (i - a.vertex_.y()) / (b.vertex_.y() - a.vertex_.y());
        VertexAttrs from = VertexAttrs::Interpolate(a, b, ty, material_->RequireNormals(), material_->RequireTexcoords());
        VertexAttrs to = VertexAttrs::Interpolate(a, d, ty, material_->RequireNormals(), material_->RequireTexcoords());
        for (int j = from.vertex_.x(); j <= to.vertex_.x(); ++j) {
            const float tx = (j - from.vertex_.x()) / (to.vertex_.x() - from.vertex_.x());
            VertexAttrs attrs = VertexAttrs::Interpolate(from, to, tx, material_->RequireNormals(), material_->RequireTexcoords());
            DrawPixel(Vector2i(j, i), attrs.vertex_.z(), attrs.normal_, attrs.texcoord_);
           
        }
    }
     for (int i = ceil(c.vertex_.y()); i <= b.vertex_.y(); ++i) {
        float ty = (i - c.vertex_.y()) / (b.vertex_.y() - c.vertex_.y());
        VertexAttrs from = VertexAttrs::Interpolate(c, b, ty, material_->RequireNormals(), material_->RequireTexcoords());
        VertexAttrs to = VertexAttrs::Interpolate(c, d, ty, material_->RequireNormals(), material_->RequireTexcoords());
        for (int j = ceil(from.vertex_.x()); j <= to.vertex_.x(); ++j) {
            const float tx = (j - from.vertex_.x()) / (to.vertex_.x() - from.vertex_.x());
            VertexAttrs attrs = VertexAttrs::Interpolate(from, to, tx, material_->RequireNormals(), material_->RequireTexcoords());
            DrawPixel(Vector2i(j, i), attrs.vertex_.z(), attrs.normal_, attrs.texcoord_);
        }
    }
}
void renderer::IndicedTriangleRenderer::DrawAllAfterTransformation() {
    for (int k = 0; k < indices_->size(); k += 3) {
        DrawTriangle((*indices_)[k], (*indices_)[k + 1], (*indices_)[k + 2]);
    }
}

// Coordinates should already be homogenous.
bool renderer::IndicedTriangleRenderer::CullFace(const Vector3f& a, const Vector3f& b, const Vector3f& c) {
    Vector3f d1 = b - a, d2 = c - a;
    return d1.x() * d2.y() - d2.x() * d1.y() >= 0;
}

void renderer::IndicedTriangleRenderer::DrawPixel(const Vector2i &window_space_vertex, float depth, const Vector3f &normal, const Vector2f &texcoord) {
    if (DepthAndOwnershipTest(window_space_vertex, depth)) {
         Vector4f color = material_->DrawPixel(globals_, window_space_vertex, normal, texcoord);
        const int offset = (viewport_.y() - window_space_vertex.y() - 1) * viewport_.x() + window_space_vertex.x();
        reinterpret_cast<float*>(depth_buffer_)[offset] = depth;
        render_target_[offset * 4] = MathHelpers::Clamp(color.x()) * 255;
        render_target_[offset * 4 + 1] = MathHelpers::Clamp(color.y()) * 255;
        render_target_[offset * 4 + 2] = MathHelpers::Clamp(color.z()) * 255;
        render_target_[offset * 4 + 3] = MathHelpers::Clamp(color.w()) * 255;
    }
}

bool renderer::IndicedTriangleRenderer::DepthAndOwnershipTest(const Vector2i& a, float depth) {
    const int offset = (viewport_.y() - a.y() - 1) * viewport_.x() + a.x();
    return a.x() >= 0 && a.x() < viewport_.x() && a.y() >= 0 && a.y() < viewport_.y() && depth >= -1.f && depth <= 1.f && depth < reinterpret_cast<float*>(depth_buffer_)[offset];
}
