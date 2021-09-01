#include "CameraController.h"

CameraController::CameraController(Kernel *kernel, CObservableData<KeyCode> *observable)
    : BaseController(kernel, observable) {
    observer_ = make_unique<CObserverColdStrict<KeyCode>>([this](KeyCode keyCode) { this->OnKeyDown(keyCode); });
    observable->subscribe(observer_.get());
    cameras_[0] = GetKernel().Instantiate<Camera>("Camera 1");
    cameras_[1] = GetKernel().Instantiate<Camera>("Camera 2");
    for (auto &camera : cameras_) {
        camera->SetNearClipPlane(0.01);
        camera->SetFarClipPlane(100);
        auto [x, y] = GetKernel().GetScreenSize();
        camera->SetAspectRatio(static_cast<float>(x) / y);
    }
    cameras_[0]->SetFovAngle(60.f / 180 * EIGEN_PI);
    cameras_[0]->SetTransform({Vector3f(0, 1, -3), Quaternionf::Identity(), Vector3f(1, 1, 1)});
    cameras_[1]->SetFovAngle(30.f / 180 * EIGEN_PI);
    cameras_[1]->SetTransform(
        {Vector3f(-6, 1, 0), (Quaternionf)AngleAxisf(EIGEN_PI / 2.f, Vector3f::UnitY()), Vector3f(1, 1, 1)});
}

void CameraController::OnKeyDown(KeyCode code) {
    auto &camera = cameras_[active_cam_];
    if (code == RENDR_9) {
        cameras_[0]->Activate();
        active_cam_ = 0;
    } else if (code == RENDR_0) {
        cameras_[1]->Activate();
        active_cam_ = 1;
    } else if (code == RENDR_w) {
        camera->TransformProxy().SetPosition(camera->GetTransform().position + Vector3f(0, 0, 0.3));
    } else if (code == RENDR_s) {
        camera->TransformProxy().SetPosition(camera->GetTransform().position - Vector3f(0, 0, 0.3));
    } else if (code == RENDR_a) {
        camera->TransformProxy().SetPosition(camera->GetTransform().position - Vector3f(0.3, 0, 0));
    } else if (code == RENDR_d) {
        camera->TransformProxy().SetPosition(camera->GetTransform().position + Vector3f(0.3, 0, 0));
    } else if (code == RENDR_r) {
        camera->TransformProxy().SetPosition(camera->GetTransform().position - Vector3f(0, 0.3, 0));
    } else if (code == RENDR_f) {
        camera->TransformProxy().SetPosition(camera->GetTransform().position + Vector3f(0, 0.3, 0));
    } else if (code == RENDR_q) {
        camera->SetFovAngle(camera->GetFovAngle() + 0.05);
    } else if (code == RENDR_e) {
        camera->SetFovAngle(camera->GetFovAngle() - 0.05);
    }
}