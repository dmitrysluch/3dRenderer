#include "AmogusController.h"

#include <AssimpBindings.h>
#include <Phong.h>

#include <fstream>

AmogusController::AmogusController(Kernel *kernel, CObservableData<KeyCode> *observable)
    : BaseController(kernel, observable) {
    observer_ = make_unique<CObserverColdStrict<KeyCode>>([this](KeyCode keyCode) { this->OnKeyDown(keyCode); });
    observable->subscribe(observer_.get());

    Buffer2D<ColorRGBA32> amogus_texture(512, 512);
    ifstream fin("texture.rgba32");
    fin.read((char *)amogus_texture.data(), 512 * 512 * 4);
    fin.close();
    amogus_mesh_ = AssimpBindings::LoadMeshFromFile("amogus.obj");
    amogus_mat_ = MaterialVec({make_shared<Phong>(std::move(amogus_texture), 10.0f, 1.0f)});
    amogus_.resize(3);
    for (int i = 0; i < 3; ++i) {
        amogus_[i] = GetKernel().Instantiate<Object>("Amogus " + to_string(i), amogus_mesh_, amogus_mat_);
        amogus_[i]->TransformProxy().SetPosition(Vector3f(i - 1.0f, 0, 0));
        amogus_[i]->TransformProxy().SetRotation((Quaternionf)AngleAxisf(AI_MATH_PI_F, Vector3f::UnitY()));
    }
}

void AmogusController::OnKeyDown(KeyCode code) {
    auto &curr_amogus = amogus_[active_];
    if (code == RENDR_n) {
        amogus_.push_back(
            GetKernel().Instantiate<Object>("Amogus " + to_string(amogus_.size()), amogus_mesh_, amogus_mat_));
        active_ = amogus_.size() - 1;
    } else if (code == RENDR_UP) {
        curr_amogus->TransformProxy().SetPosition(curr_amogus->GetTransform().position + Vector3f(0, 0, 0.1));
    } else if (code == RENDR_DOWN) {
        curr_amogus->TransformProxy().SetPosition(curr_amogus->GetTransform().position - Vector3f(0, 0, 0.1));
    } else if (code == RENDR_LEFT) {
        curr_amogus->TransformProxy().SetPosition(curr_amogus->GetTransform().position - Vector3f(0.1, 0, 0));
    } else if (code == RENDR_RIGHT) {
        curr_amogus->TransformProxy().SetPosition(curr_amogus->GetTransform().position + Vector3f(0.1, 0, 0));
    } else if (code == RENDR_j) {
        curr_amogus->TransformProxy().SetPosition(curr_amogus->GetTransform().position - Vector3f(0, 0.1, 0));
    } else if (code == RENDR_k) {
        curr_amogus->TransformProxy().SetPosition(curr_amogus->GetTransform().position + Vector3f(0, 0.1, 0));
    } else if (code == RENDR_h) {
        curr_amogus->TransformProxy().SetRotation(curr_amogus->GetTransform().rotation *
                                                  AngleAxisf(AI_MATH_PI_F * 0.2, Vector3f::UnitY()));
    } else if (code == RENDR_l) {
        curr_amogus->TransformProxy().SetRotation(curr_amogus->GetTransform().rotation *
                                                  AngleAxisf(AI_MATH_PI_F * -0.2, Vector3f::UnitY()));
    } else if (code <= RENDR_8 && code >= RENDR_1) {
        active_ = code - RENDR_1;
    }
}