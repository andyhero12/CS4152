//
//  GLInputController.cpp
//  Dog Lab
//
//  This class buffers in input from the devices and converts it into its
//  semantic meaning. If your game had an option that allows the player to
//  remap the control keys, you would store this information in this class.
//  That way, the main game scene does not have to keep track of the current
//  key mapping.
//
//  Author: Walker M. White
//  Based on original GameX Ship Demo by Rama C. Hoetzlein, 2002
//  Version: 1/20/22
//
#include <cugl/cugl.h>
#include "InputController.h"

using namespace cugl;

/**
 * Creates a new input controller with the default settings
 *
 * This is a very simple class.  It only has the default settings and never
 * needs to attach any custom listeners at initialization.  Therefore, we
 * do not need an init method.  This constructor is sufficient.
 */
InputController::InputController() :
_forward(0),
_turning(0),
_didFire(false) {
}

bool InputController::init() {
    bool contSuccess = Input::activate<GameControllerInput>();

    if (contSuccess) {
        GameControllerInput* controller = Input::get<GameControllerInput>();
        std::vector<std::string> deviceUUIDs = controller->devices();
        if (deviceUUIDs.size() == 0){
            return false;
        }
        _gameContrl = controller -> open(deviceUUIDs[0]);
        std::cout << deviceUUIDs[0] << std::endl;
        directions = { {-1,0},{0,-1},{1,0}, {0,1} }; // left up right down
        return true;
    }

    return false;

}

/**
 * Reads the input for this player and converts the result into game logic.
 *
 * This is an example of polling input.  Instead of registering a listener,
 * we ask the controller about its current state.  When the game is running,
 * it is typically best to poll input instead of using listeners.  Listeners
 * are more appropriate for menus and buttons (like the loading screen).
 */
 /**
  * Reads the input for this player and converts the result into game logic.
  *
  * This is an example of polling input.  Instead of registering a listener,
  * we ask the controller about its current state.  When the game is running,
  * it is typically best to poll input instead of using listeners.  Listeners
  * are more appropriate for menus and buttons (like the loading screen).
  */
void InputController::readInput() {
    // This makes it easier to change the keys later
    KeyCode up = KeyCode::ARROW_UP;
    KeyCode down = KeyCode::ARROW_DOWN;
    KeyCode left = KeyCode::ARROW_LEFT;
    KeyCode right = KeyCode::ARROW_RIGHT;
    KeyCode shoot = KeyCode::SPACE;
    KeyCode reset = KeyCode::R;
    KeyCode mode = KeyCode::F;
    KeyCode special = KeyCode::G;

    // Convert keyboard state into game commands
    //_forward = _turning = 0;
    _UseKeyboard = false;

    // Movement forward/backward
   // std::cout << LR << " " << UD << std::endl;

    Keyboard* keys = Input::get<Keyboard>();


    // Movement left/right
    if (keys->keyDown(left) && !keys->keyDown(right)) {
        _turning = -1;
        _UseKeyboard = true;
    }
    else if (keys->keyDown(right) && !keys->keyDown(left)) {
        _turning = 1;
        _UseKeyboard = true;
    }
    // Shooting
    if (keys->keyDown(shoot)) {
        _didFire = true;
        _UseKeyboard = true;
    }

    // Reset the game
    if (keys->keyDown(reset)) {
        _didReset = true;
        _UseKeyboard = true;
    }
    // Movement forward/backward

    if (keys->keyDown(mode)) {
        _didChangeMode = true;
        _UseKeyboard = true;
    }

    if (keys->keyDown(special)) {
        _didSpecial = true;
        _UseKeyboard = true;
    }

    if (keys->keyDown(up) && !keys->keyDown(down)) {
        _forward = 1;
        _UseKeyboard = true;
    }
    else if (keys->keyDown(down) && !keys->keyDown(up)) {
        _forward = -1;
        _UseKeyboard = true;
    }

}


void InputController::readInput_joystick() {
    cugl::GameController::Axis X_left = cugl::GameController::Axis::LEFT_X;
    cugl::GameController::Axis Y_left = cugl::GameController::Axis::LEFT_Y;
    cugl::GameController::Button A = cugl::GameController::Button::A;
    cugl::GameController::Button X = cugl::GameController::Button::X;
    cugl::GameController::Button B = cugl::GameController::Button::B;
    cugl::GameController::Button Y = cugl::GameController::Button::Y;

    _didFire = false;
    _didReset = false;
    _didChangeMode = false;
    _didSpecial = false;
    _Vel = cugl::Vec2(0, 0);
    _UseJoystick = false;
    // Need for this sprint
    _forward = _turning = 0;
    /* Movement using controller*/
    if (_gameContrl) {
        float LR = _gameContrl->getAxisPosition(X_left);
        float UD = _gameContrl->getAxisPosition(Y_left);

        if (_gameContrl->isButtonDown(A)) {
            _didFire = true;
            _UseJoystick = true;
        }

        if (_gameContrl->isButtonDown(B)) {
            _didSpecial = true;
            _UseJoystick = true;
            std::cout << "didspecial" << std::endl;
        }

        if (_gameContrl->isButtonDown(X)) {
            _didChangeMode = true;
            _UseJoystick = true;
        }

        if (_gameContrl->isButtonDown(Y)) {
            _didReset = true;
            _UseJoystick = true;
        }

        //std::cout << LR << " " << UD << std::endl;

        // Controller
        // Movement 360 degrees
        if (abs(LR) >= 0.2 || abs(UD) >= 0.2) {

            _Vel = cugl::Vec2(LR, -UD);
            _UseJoystick = true;

            if (LR > 0) {
                _turning = 1;
            }
            else if (LR < 0) {
                _turning = -1;
            }

            //int index = 0;
            //cugl::Vec2 curLoc(LR, UD);
            //float dist = curLoc.distance(directions[0]);
            ////std::cout << dist << std::endl;
            //for (int i = 1; i < directions.size(); i++) {
            //    cugl::Vec2& direction = directions[i];
            //    if (curLoc.distance(direction) < dist) {
            //        index = i;
            //        dist = curLoc.distance(direction);
            //    }
            //}
            //std::cout << index << std::endl;
            //if (index == 0) {
            //    _turning = -1;
            //}
            //else if (index == 1) {
            //    
            //    _forward = 1;
            //}
            //else if (index == 2) {
            //    
            //    _turning = 1;
            //}
            //else if (index == 3) {
            //    
            //    _forward = -1;
            //}
            //else {
            //    
            //}
        }
    }
}
