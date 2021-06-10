#include "include/BaseController.h"

renderer::Kernel& renderer::BaseController::GetKernel() {
    return *kernel_;
}
