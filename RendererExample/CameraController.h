#include <BaseController.h>
#include <Camera.h>
#include <Kernel.h>
#include <Keycodes.h>
#include <Library/Observer/Observer.h>

using namespace renderer;
using namespace NSLibrary;

class CameraController : BaseController {
   public:
    CameraController(Kernel *kernel, CObservableData<KeyCode> *observable);
    void OnKeyDown(KeyCode code);

   private:
    unique_ptr<CObserverColdStrict<KeyCode>> observer_;
    Camera *cameras_[2];
    int active_cam_ = 0;
};