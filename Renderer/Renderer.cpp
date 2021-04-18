#include "include/Renderer.h"

using namespace Eigen;

void renderer::Renderer::DrawIndexed() {
    assert(material_ && "Material not set");
    assert(render_target_ && "Render target not set");
    assert(depth_buffer_ && "Depth buffer not set");
    assert(render_target_->x() == depth_buffer_->x() && render_target_->y() == depth_buffer_->y() &&
           "Depth buffer dimentions doesn't match those of render target");
    assert(vertices_ && "Vertices not set");
    assert(indices_ && "Indices not set");
    assert((!material_->RequireNormals() || normals_ && normals_->size() == vertices_->size()) &&
           "Normals not set or doesn't match vertices size");
    assert((!material_->RequireTexcoords() || texcoords_ && texcoords_->size() == vertices_->size()) &&
           "Texcoords not set or doesn't match vertices size");
    TransformVertices();
    DrawAllAfterTransformation();
}

void renderer::Renderer::TransformVertices() {
#ifdef _OPENMP
    #pragma omp parallel for num_threads(8)
#endif
    for (int i = 0; i < vertices_->size(); ++i) {
        auto camera_space_coords = eye_ * (object_ * (*vertices_)[i].homogeneous());
        if (material_->RequireNormals() || material_->RequireTexcoords()) {
            inverse_camera_space_z_[i] = 1 / camera_space_coords.z();
        }
        screen_space_vertices_[i] = (viewport_matrix_ * (projection_ * camera_space_coords)).hnormalized();
    }
    if (material_->RequireNormals()) {
#ifdef _OPENMP
#pragma omp parallel for num_threads(8)
#endif
        for (int i = 0; i < normals_->size(); ++i) {
            transformed_normals_[i] = object_.topLeftCorner(3, 3).transpose().inverse() * (*normals_)[i];
        }
    }
}

renderer::Renderer::VertexAttrs renderer::Renderer::VertexAttrs::Interpolate(const VertexAttrs& a, const VertexAttrs& b,
                                                                             float t, bool interpolate_normals,
                                                                             bool interpolate_texcoords) {
    VertexAttrs res(MathHelpers::Lerp(a.vertex_, b.vertex_, t));
    if (interpolate_normals || interpolate_texcoords) {
        res.inv_z_ = MathHelpers::Lerp(a.inv_z_, b.inv_z_, t);
    }
    if (interpolate_normals) {
        res.normal_ = MathHelpers::PerspInterpolate(a.normal_, a.inv_z_, b.normal_, b.inv_z_, res.inv_z_, t);
    }
    if (interpolate_texcoords) {
        res.texcoord_ = MathHelpers::PerspInterpolate(a.texcoord_, a.inv_z_, b.texcoord_, b.inv_z_, res.inv_z_, t);
    }
    return res;
}

void renderer::Renderer::DrawTriangle(int ind_a, int ind_b, int ind_c) {
    VertexAttrs a(screen_space_vertices_[ind_a]);
    VertexAttrs b(screen_space_vertices_[ind_b]);
    VertexAttrs c(screen_space_vertices_[ind_c]);
    if (!CullFace(a.vertex_, b.vertex_, c.vertex_)) {
        return;
    }
    if (material_->RequireNormals() || material_->RequireTexcoords()) {
        a.inv_z_ = inverse_camera_space_z_[ind_a];
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
    VertexAttrs d = VertexAttrs::Interpolate(a, c, (b.vertex_.y() - a.vertex_.y()) / (c.vertex_.y() - a.vertex_.y()),
                                             material_->RequireNormals(), material_->RequireTexcoords());
    if (d.vertex_.x() < b.vertex_.x()) {
        swap(b, d);
    }
    for (int i = min<int>(a.vertex_.y(), render_target_->y() - 1); i >= b.vertex_.y() && i >= 0; --i) {
        const float ty = (i - a.vertex_.y()) / (b.vertex_.y() - a.vertex_.y());
        VertexAttrs from =
            VertexAttrs::Interpolate(a, b, ty, material_->RequireNormals(), material_->RequireTexcoords());
        VertexAttrs to = VertexAttrs::Interpolate(a, d, ty, material_->RequireNormals(), material_->RequireTexcoords());
        for (int j = max<int>(from.vertex_.x(), 0); j <= to.vertex_.x() && j < render_target_->x(); ++j) {
            const float tx = (j - from.vertex_.x()) / (to.vertex_.x() - from.vertex_.x());
            VertexAttrs attrs =
                VertexAttrs::Interpolate(from, to, tx, material_->RequireNormals(), material_->RequireTexcoords());
            DrawPixel(Vector2i(j, i), attrs.vertex_.z(), attrs.normal_, attrs.texcoord_);
        }
    }
    for (int i = max<int>(ceil(c.vertex_.y()), 0); i <= b.vertex_.y() && i < render_target_->y(); ++i) {
        float ty = (i - c.vertex_.y()) / (b.vertex_.y() - c.vertex_.y());
        VertexAttrs from =
            VertexAttrs::Interpolate(c, b, ty, material_->RequireNormals(), material_->RequireTexcoords());
        VertexAttrs to = VertexAttrs::Interpolate(c, d, ty, material_->RequireNormals(), material_->RequireTexcoords());
        for (int j = max<int>(ceil(from.vertex_.x()), 0); j <= to.vertex_.x() && j < render_target_->x(); ++j) {
            const float tx = (j - from.vertex_.x()) / (to.vertex_.x() - from.vertex_.x());
            VertexAttrs attrs =
                VertexAttrs::Interpolate(from, to, tx, material_->RequireNormals(), material_->RequireTexcoords());
            DrawPixel(Vector2i(j, i), attrs.vertex_.z(), attrs.normal_, attrs.texcoord_);
        }
    }
}
void renderer::Renderer::DrawAllAfterTransformation() {
#ifdef _OPENMP
#pragma omp parallel for num_threads(8)
#endif
    for (int k = 0; k < indices_->size(); k += 3) {
        DrawTriangle((*indices_)[k], (*indices_)[k + 1], (*indices_)[k + 2]);
    }
}

// Coordinates should already be homogenous.
bool renderer::Renderer::CullFace(const Vector3f& a, const Vector3f& b, const Vector3f& c) {
    Vector3f d1 = b - a, d2 = c - a;
    return d1.x() * d2.y() - d2.x() * d1.y() >= 0;
}

void renderer::Renderer::DrawPixel(const Vector2i& window_space_vertex, float depth, const Vector3f& normal,
                                   const Vector2f& texcoord) {
    if (DepthTest(window_space_vertex, depth)) {
        Vector4f color = material_->DrawPixel(window_space_vertex, normal, texcoord);
        (*depth_buffer_)(window_space_vertex.x() , render_target_->y() - 1 - window_space_vertex.y()) = depth;
        for (int i = 0; i < 4; ++i) {
            (*render_target_)(window_space_vertex.x(), render_target_->y() - 1 - window_space_vertex.y()).val[i] =
                static_cast<char>(MathHelpers::Clamp(color[i]) * 255);
        }
    }
}

bool renderer::Renderer::DepthTest(const Vector2i& a, float depth) {
    return depth >= -1.f && depth <= 1.f && depth < (*depth_buffer_)(a.x(), render_target_->y() - 1 -a.y());
}
void renderer::Renderer::Clear(renderer::ColorRGBA32 color) {
    int sz = render_target_->x() * render_target_->y();
    fill(render_target_->data(), render_target_->data() + sz, color);
    fill(depth_buffer_->data(), depth_buffer_->data() + sz, 1);
}
