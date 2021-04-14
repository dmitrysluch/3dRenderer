#pragma once
#include <memory>
#include <Library/Singleton/AnyGlobalAccess.h>
#include "Object.h"
#include "IView.h"
#include "Camera.h"
#include "Renderer.h"

namespace renderer {
class KernelID;
class Kernel {
   public:
    explicit Kernel(unique_ptr<IView> view);
    [[nodiscard]] Object *GetObjectPtr(const string &name);
    [[nodiscard]] const Object *GetObjectPtr(const string &name) const;
    [[nodiscard]] Object &GetObject(const string &name);
    [[nodiscard]] const Object &GetObject(const string &name) const;
    // Objects, Lights and Camera call OnUpdate to cause rerender
   private:
    void OnUpdate();

    /// There is a reasoning behind storing objects, cameras, lights (other scene objects if implement them) as unique_ptrs (not_shared).
    /// These objects call Kernel.OnUpdate() in constructor/getters/setters to cause rerendering.
    /// Thus these objects are likely to cause exception if created before kernel is initialized or destroyed after kernel was destroyed.
    std::vector<unique_ptr<Object>> objects_;
    std::vector<unique_ptr<Camera>> cameras_;
    int active_camera_id_;
    unique_ptr<IView> view_;
    Renderer renderer_;

    friend Object;
    friend Camera;
};
class KernelAccessible : public NSLibrary::CAnyGlobalAccessible<Kernel, KernelID> {};
class KernelInitializer : public NSLibrary::CAnyGlobalInitializer<Kernel, KernelID> {};
class KernelAccessor : public NSLibrary::CAnyGlobalAccess<Kernel, KernelID> {};
}