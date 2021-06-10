#pragma once
#include <Library/Observer/Observer.h>
#include "Keycodes.h"
#include "Kernel.h"

namespace renderer {
class BaseController {
   public:
    BaseController() = delete;
    BaseController(Kernel *kernel, NSLibrary::CObservableData<KeyCode> *observable) : kernel_(kernel) {}
   protected:
    Kernel &GetKernel();
   private:
    Kernel *kernel_;
};
} // renderer