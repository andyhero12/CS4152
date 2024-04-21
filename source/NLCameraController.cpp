#include "NLCameraController.h"

using namespace cugl;

#pragma mark -
#pragma mark Initializers

bool CameraController::init(const std::shared_ptr<cugl::scene2::SceneNode> target, const std::shared_ptr<cugl::scene2::ScrollPane> root, std::shared_ptr <cugl::OrthographicCamera> camera, float maxZoom) {
    _target = target;
    _root = root;
    _camera = camera;
    _maxZoom = maxZoom;
    return true;
}

#pragma mark -
#pragma mark Camera Handling

void CameraController::update(float dt) {
//    Vec2 cameraPos = Vec2(_camera->getPosition().x, _camera->getPosition().y);
    
    Vec2 cameraPos = _root->getPaneTransform().getTranslation();
    
    Vec2 target;
    Vec2* dst = new Vec2();
    // Lazily track the target using lerp
    target = Vec2(_target->getWorldPosition().x, _target->getWorldPosition().y);
    Vec2::lerp(cameraPos, target, 0.95 * dt, dst);
    // Make sure the camera never goes outside of the _root node's bounds
    //_camera->translate((*dst).x - cameraPos.x, (*dst).y - cameraPos.y);
    
//    _root->resetPane();
    std::cout << (cameraPos.x) << " "  <<  ((*dst).y - cameraPos.y) << "\n";
    
    _root->applyPan((*dst).x - cameraPos.x, (*dst).y - cameraPos.y);
    delete dst;
    
//    _camera->update();
}

void CameraController::setZoom(float zoom) {
    if (zoom > _maxZoom){
        CULog("ZOOM TOO FAR");
        return;
    }
//    _camera->setZoom(zoom);
}

void CameraController::addZoom(float zoom) {
    float originalZoom = _camera->getZoom();
    // Don't let it be greater than max zoom
    if (originalZoom + zoom > _maxZoom){
        CULog("ZOOM TOO FAR");
        return;
    }
    float truezoom = std::max(originalZoom + zoom, 0.01f);
    _camera->setZoom(truezoom);
}

void CameraController::setTarget(std::shared_ptr<cugl::scene2::SceneNode> target) {
    _target = target;
}
