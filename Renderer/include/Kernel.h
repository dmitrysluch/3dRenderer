#pragma once
#include <memory>

#include "Camera.h"
#include "DirectionalLight.h"
#include "IView.h"
#include "Object.h"
#include "Renderer.h"

namespace renderer {
class Kernel {
   public:
    explicit Kernel(unique_ptr<IView> view);
    [[nodiscard]] bool IsActive() const { return active_; }
    void SetActive(bool active);
    template <typename T, typename... TArgs>
    T* Instantiate(TArgs&&... args) {
        return T::InstantiateWithKernel(this, std::forward<TArgs>(args)...);
    }
    template <typename T>
    T* GetComponent(const std::string &str) {
        return T::GetComponentInKernel(*this, str);
    }
    template <typename T>
    const T* GetComponent(const std::string &str) const {
        return T::GetComponentInKernel(*this, str);
    }
    [[nodiscard]] pair<int, int> GetScreenSize() const {
        return {view_->GetRenderTarget().x(), view_->GetRenderTarget().y()};
    }
   private:
    // Objects, Lights and Camera call OnUpdate to cause rerender
    void OnUpdate();
    void Render();
    bool active_ = false;

    // There is a reasoning behind storing objects, cameras, lights (other scene objects if implement them) as
    // unique_ptrs (not_shared). These objects call Kernel.OnUpdate() in constructor/getters/setters to cause
    // rerendering. Thus these objects are likely to cause exception if created before kernel is initialized or
    // destroyed after kernel was destroyed.
    std::vector<unique_ptr<Object>> objects_;
    std::vector<unique_ptr<Camera>> cameras_;
    std::vector<unique_ptr<DirectionalLight>> lights_;
    int active_camera_id_ = 0;
    unique_ptr<IView> view_;
    Renderer renderer_;

    friend class Object;
    friend class Camera;
    friend class DirectionalLight;
};
}  // namespace renderer