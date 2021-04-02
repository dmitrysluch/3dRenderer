//
// Created by Dima on 25.02.2021.
//

#include "include/BilinearSampler.h"
Eigen::Vector4f renderer::BilinearSampler::SamplePixelBilinear(const renderer::Buffer2D<renderer::ColorRGBA32>& text,
                                                               Eigen::Vector2f texcoord) {
    float ti = texcoord.x() * (text.x() - 1);
    int i = floor(ti);
    int ni = min<int>(i + 1, text.x() - 1);
    ti -= i;
    float tj = texcoord.y() * (text.y() - 1);
    int j = floor(tj);
    int nj = min<int>(j + 1, text.y() - 1);
    tj -= j;
    return MathHelpers::Lerp(MathHelpers::Lerp(Color2Vec(text(i, j)), Color2Vec(text(ni, j)), ti),
                             MathHelpers::Lerp(Color2Vec(text(i, nj)), Color2Vec(text(ni, nj)), ti), tj);
}
