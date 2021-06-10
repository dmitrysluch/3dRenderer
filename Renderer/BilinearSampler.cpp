//
// Created by Dima on 25.02.2021.
//
#include <math.h>
#include "include/MathHelpers.h"
#include "include/BilinearSampler.h"

Eigen::Vector4f renderer::BilinearSampler::SamplePixelBilinear(const renderer::Buffer2D<renderer::ColorRGBA32>& text,
                                                               Eigen::Vector2f texcoord) {
    float ti = texcoord.x();
    ti *= static_cast<float>(text.x() - 1);
    int i = max<int>(static_cast<int>(floor(ti)), 0);
    int ni = min<int>(i + 1, text.x() - 1);
    ti -= static_cast<float>(i);
    float tj = texcoord.y();
    tj *=  static_cast<float>(text.y() - 1);
    int j = max<int>(static_cast<int>(floor(tj)), 0);
    int nj = min<int>(j + 1, text.y() - 1);
    tj -= static_cast<float>(j);
    return MathHelpers::Lerp(MathHelpers::Lerp(Color2Vec(text(i, j)), Color2Vec(text(ni, j)), ti),
                             MathHelpers::Lerp(Color2Vec(text(i, nj)), Color2Vec(text(ni, nj)), ti), tj);
}
