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
        screen_space_vertices_[i] = (viewport_matrix_ * (projection_ * camera_space_coords));
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

Vector3f v4f2v3f(const Vector4f &vec) {
    return Vector3f(vec.x(), vec.y(), vec.z());
}

void renderer::Renderer::DrawTriangle(int a, int b, int c) {
    VertexAttrs attrs[6];
    int sz = ClipTriangle(a, b, c, attrs);
    for (int i = 0; i < sz; i += 3) {
        DrawTriangleClipped(attrs[i], attrs[i + 1], attrs[i + 2]);
    }
}
int renderer::Renderer::ClipTriangle(int a, int b, int c, VertexAttrs* attrs) const {
    if (screen_space_vertices_[a].w() < near_plane_ && screen_space_vertices_[b].w() < near_plane_ && screen_space_vertices_[c].w() < near_plane_) {
        return 0;
    }
    bool swapped = false;
    if (screen_space_vertices_[a].w() >= near_plane_ && screen_space_vertices_[b].w() < near_plane_) {
        swap(a, b);
        swapped = !swapped;
    }
    if (screen_space_vertices_[a].w() >= near_plane_ && screen_space_vertices_[c].w() < near_plane_) {
        swap(a, c);
        swapped = !swapped;
    }
    if (screen_space_vertices_[b].w() >= near_plane_ && screen_space_vertices_[c].w() < near_plane_) {
        swap(b, c);
        swapped = !swapped;
    }
    attrs[2].vertex_ = screen_space_vertices_[c].hnormalized();
    if (material_->RequireNormals() || material_->RequireTexcoords()) {
        attrs[2].inv_z_ = 1.f / screen_space_vertices_[c].w();
        if (material_->RequireNormals()) {
            attrs[2].normal_ = transformed_normals_[c];
        }
        if (material_->RequireTexcoords()) {
            attrs[2].texcoord_ = (*texcoords_)[c];
        }
    }
    if (screen_space_vertices_[b].w() < near_plane_) { // a and v are before near plane, clip them
        const float ta = (near_plane_ - screen_space_vertices_[c].w()) / (screen_space_vertices_[a].w() - screen_space_vertices_[c].w());
        attrs[0].vertex_ = MathHelpers::Lerp<Vector4f>(screen_space_vertices_[c], screen_space_vertices_[a], ta).hnormalized();
        const float tb = (near_plane_ - screen_space_vertices_[c].w()) / (screen_space_vertices_[b].w() - screen_space_vertices_[c].w());
        attrs[1].vertex_ = MathHelpers::Lerp<Vector4f>(screen_space_vertices_[c], screen_space_vertices_[b], tb).hnormalized();
        if (material_->RequireNormals() || material_->RequireTexcoords()) {
            attrs[0].inv_z_ = 1.f / near_plane_;
            attrs[1].inv_z_ = 1.f / near_plane_;
            if (material_->RequireNormals()) {
                // Not perspective correct interpolation because t is computed in Camera space (w in HOS is z in Camera S)
                attrs[0].normal_ = MathHelpers::Lerp(transformed_normals_[c], transformed_normals_[a], ta);
                attrs[1].normal_ = MathHelpers::Lerp(transformed_normals_[c], transformed_normals_[b], tb);
            }
            if (material_->RequireTexcoords()) {
                attrs[0].texcoord_ = MathHelpers::Lerp((*texcoords_)[c], (*texcoords_)[a], ta);
                attrs[1].texcoord_ = MathHelpers::Lerp((*texcoords_)[c], (*texcoords_)[b], tb);
            }
        }
        if (swapped) {
            swap(attrs[1], attrs[2]);
        }
        return 3;
    }
    if (screen_space_vertices_[a].w() < near_plane_) {
        const float tc = (near_plane_ - screen_space_vertices_[c].w()) / (screen_space_vertices_[a].w() - screen_space_vertices_[c].w());
        attrs[0].vertex_ = MathHelpers::Lerp(screen_space_vertices_[c], screen_space_vertices_[a], tc ).hnormalized();
        attrs[1].vertex_ = screen_space_vertices_[b].hnormalized();
        const float tb = (near_plane_ - screen_space_vertices_[b].w()) / (screen_space_vertices_[a].w() - screen_space_vertices_[b].w());
        attrs[3].vertex_ = MathHelpers::Lerp(screen_space_vertices_[b], screen_space_vertices_[a], tb).hnormalized();
        if (material_->RequireNormals() || material_->RequireTexcoords()) {
            attrs[0].inv_z_ = 1.f / near_plane_;
            attrs[1].inv_z_ = 1.f / screen_space_vertices_[b].w();
            attrs[3].inv_z_ = 1.f / near_plane_;
            if (material_->RequireNormals()) {
                // Not perspective correct interpolation because t is computed in Camera space (w in HOS is z in Camera S)
                attrs[0].normal_ = MathHelpers::Lerp(transformed_normals_[c], transformed_normals_[a], tc);
                attrs[1].normal_ = transformed_normals_[b];
                attrs[3].normal_ = MathHelpers::Lerp(transformed_normals_[b], transformed_normals_[a], tb);
            }
            if (material_->RequireTexcoords()) {
                attrs[0].texcoord_ = MathHelpers::Lerp((*texcoords_)[c], (*texcoords_)[a], tc);
                attrs[1].texcoord_ = (*texcoords_)[b];
                attrs[3].texcoord_ = MathHelpers::Lerp((*texcoords_)[b], (*texcoords_)[a], tb);
            }
        }
        attrs[4] = attrs[1];
        attrs[5] = attrs[0];
        if (swapped) {
            swap(attrs[1], attrs[2]);
            swap(attrs[4], attrs[5]);
        }
        return 6;
    }
    attrs[0].vertex_ = screen_space_vertices_[a].hnormalized();
    attrs[1].vertex_ = screen_space_vertices_[b].hnormalized();
    if (material_->RequireNormals() || material_->RequireTexcoords()) {
        attrs[0].inv_z_ = 1.f / screen_space_vertices_[a].w();
        attrs[1].inv_z_ = 1.f / screen_space_vertices_[b].w();
        if (material_->RequireNormals()) {
            attrs[0].normal_ = transformed_normals_[a];
            attrs[1].normal_ = transformed_normals_[b];
        }
        if (material_->RequireTexcoords()) {
            attrs[0].texcoord_ = (*texcoords_)[a];
            attrs[1].texcoord_ = (*texcoords_)[b];
        }
    }
    if (swapped) {
        swap(attrs[1], attrs[2]);
    }
    return 3;
}
void renderer::Renderer::DrawTriangleClipped(VertexAttrs &a, VertexAttrs &b, VertexAttrs &c) {
    if (!CullFace(a.vertex_, b.vertex_, c.vertex_)) {
        return;
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
        for (int j = max<int>(ceil(from.vertex_.x()), 0); j <= to.vertex_.x() && j < render_target_->x(); ++j) {
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
    return d1.x() * d2.y() - d2.x() * d1.y() <= 0;
}

void renderer::Renderer::DrawPixel(const Vector2i& window_space_vertex, float depth, const Vector3f& normal,
                                   const Vector2f& texcoord) {
    if (DepthTest(window_space_vertex, depth)) {
        Vector4f color = material_->DrawPixel(window_space_vertex, normal, texcoord);
        (*depth_buffer_)(window_space_vertex.x() , window_space_vertex.y()) = depth;
        for (int i = 0; i < 4; ++i) {
            (*render_target_)(window_space_vertex.x(), window_space_vertex.y()).val[i] =
                static_cast<char>(MathHelpers::Clamp(color[i]) * 255);
        }
    }
}

bool renderer::Renderer::DepthTest(const Vector2i& a, float depth) {
    return depth >= -1.f && depth <= 1.f && depth < (*depth_buffer_)(a.x(), a.y());
}
void renderer::Renderer::Clear(renderer::ColorRGBA32 color) {
    int sz = render_target_->x() * render_target_->y();
    fill(render_target_->data(), render_target_->data() + sz, color);
    fill(depth_buffer_->data(), depth_buffer_->data() + sz, 1);
}
