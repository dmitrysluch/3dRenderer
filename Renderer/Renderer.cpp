#include "Renderer.h"

using namespace Eigen;

void renderer::IndicedTriangleRenderer::DrawIndexed() {
    TransformVertices();
    DrawPixels();
}

void renderer::IndicedTriangleRenderer::TransformVertices() {
    for (int i = 0; i < vertices_->size(); ++i) {
        window_space_vertices_[i] = (viewport_matrix_ * (projection_ * (eye_ * (object_ * (*vertices_)[i].homogeneous())))).hnormalized();
    }
}
int bad = 0;
void renderer::IndicedTriangleRenderer::DrawPixels() {
    for (int k = 0; k < indices_->size(); k += 3) {
        Vector3f a = window_space_vertices_[(*indices_)[k]], b = window_space_vertices_[(*indices_)[k + 1]],
                 c = window_space_vertices_[(*indices_)[k + 2]];
        if (!CullFace(a, b, c)) {
            continue;
        }
        if (a.y() < b.y()) {
            swap(a, b);
        }
        if (a.y() < c.y()) {
            swap(a, c);
        }
        if (b.y() < c.y()) {
            swap(b, c);
        }
        Vector3f d = Lerp(a, c, (b.y() - a.y()) / (c.y() - a.y()));
        if (d.x() < b.x()) {
            swap(b, d);
        }
        bad = k;
        for (int i = a.y(); i >= b.y(); --i) {
            const float ty = (i - a.y()) / (b.y() - a.y());
            Vector3f from = Lerp(a, b, ty), to = Lerp(a, d, ty);
            for (int j = from.x(); j <= to.x(); ++j) {
                const float tx = (j - from.x()) / (to.x() - from.x());
                DrawPixel(Vector2i(j, i), Lerp(from.z(), to.z(), tx));
            }
        }
        for (int i = c.y(); i <= b.y(); ++i) {
            float ty = (i - c.y()) / (b.y() - c.y());
            Vector3f from = Lerp(c, b, ty), to = Lerp(c, d, ty);
            for (int j = from.x(); j <= to.x(); ++j) {
                const float tx = (j - from.x()) / (to.x() - from.x());
                DrawPixel(Vector2i(j, i), Lerp(from.z(), to.z(), tx));
            }
        }
    }
}

// Coordinates should already be homogenous.
bool renderer::IndicedTriangleRenderer::CullFace(const Vector3f& a, const Vector3f& b, const Vector3f& c) {
    Vector3f d1 = b - a, d2 = c - a;
    return d1.x() * d2.y() - d2.x() * d1.y() >= 0;
}

void renderer::IndicedTriangleRenderer::DrawPixel(const Vector2i &view_port_vertex, float depth) {
    if (DepthAndOwnershipTest(view_port_vertex, depth)) {
        const int offset = (viewport_.y() - view_port_vertex.y()) * viewport_.x() + view_port_vertex.x();
        reinterpret_cast<float *>(depth_buffer_)[offset] = depth;
        render_target_[(offset) * 4] = bad * 10;
        render_target_[(offset) * 4 + 3] = 0xFF;
    }
}

bool renderer::IndicedTriangleRenderer::DepthAndOwnershipTest(const Vector2i& a, float depth) {
    const int offset = (viewport_.y() - a.y()) * viewport_.x() + a.x();
    return a.x() >= 0 && a.x() < viewport_.x() && a.y() >= 0 && a.y() < viewport_.y() && depth >= -1.f && depth <= 1.f && depth < reinterpret_cast<float*>(depth_buffer_)[offset];
}
