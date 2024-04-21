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
#include "NLInput.h"

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
_didReset(false),
_didDash(false),
_didChangeMode(false),
_didSpecial(false),
_didDebug(false),
_didExit(false),
_didFire(false),
_didPause(false),
_pause(false),
_updown(0.0),
_Leftright(0.0),
_confirm(false),
_controllerKey(0),
_didPressRight(false),
_didPressLeft(false),
_back(false)
{
}

bool InputController::init() {
    bool contSuccess = Input::activate<GameControllerInput>();

    if (contSuccess) {
        GameControllerInput* controller = Input::get<GameControllerInput>();
        std::vector<std::string> deviceUUIDs = controller->devices();
        if (deviceUUIDs.size() == 0){
            return false;
        }
        _gameContrl = controller -> open(deviceUUIDs.at(0));
        return true;
    }

    return false;

}
bool InputController::init_withlistener() {
    bool contSuccess = Input::activate<GameControllerInput>();
    if (contSuccess) {
        GameControllerInput* controller = Input::get<GameControllerInput>();
        std::vector<std::string> deviceUUIDs = controller->devices();
        if (deviceUUIDs.size() > 0) {
            _gameContrl = controller->open(deviceUUIDs.at(0));
            _controllerKey = controller->acquireKey();
            //std::cout << _controllerKey << std::endl;
            _gameContrl->addAxisListener(_controllerKey, [=](const GameControllerAxisEvent& event, bool focus) {
            this->getAxisAngle(event, focus);
            });
            _gameContrl->addButtonListener(_controllerKey, [=](const GameControllerButtonEvent& event, bool focus) {
            this->getButton(event, focus);
            });
            _state = State::CONTROLLER;
        return true;
        }
        
    }
    _state = State::MOUSE;
    return false;

}


void InputController::update(){
    resetKeys();
    readInput_joystick();
    readInput();
}
void InputController::resetcontroller()
{
    _updown = 0;
    _Leftright = 0;
    _confirm = false;
    _back = false;
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
    KeyCode debug = KeyCode::D;
    KeyCode exit = KeyCode::ESCAPE;
    KeyCode pause = KeyCode::P;
    KeyCode dash = KeyCode::LEFT_SHIFT;
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
    if (keys->keyPressed(right)){
        _UseKeyboard = true;
        _didPressRight = true;
    }
    if (keys->keyPressed(left)){
        _UseKeyboard = true;
        _didPressLeft = true;
    }
    // Shooting
    if (keys->keyPressed(shoot)) {
        _didFire = true;
        _UseKeyboard = true;
    }

    // Reset the game
    if (keys->keyPressed(reset)) {
        _didReset = true;
        _UseKeyboard = true;
    }
    
    // Pause the game
    if (keys->keyPressed(pause)) {
//        CULog(" pressed P before : %d", _pause);
        _pause = !_pause;
//        CULog(" pressed P after : %d", _pause);
        _didPause = true;
        _UseKeyboard = true;
    }
    
    // Movement forward/backward

    if (keys->keyPressed(mode)) {
        _didChangeMode = true;
        _UseKeyboard = true;
    }

    if (keys->keyPressed(special)) {
        _didSpecial = true;
        _UseKeyboard = true;
    }
    
    if (keys->keyPressed(debug)) {
        _didDebug = true;
        _UseKeyboard = true;
    }
    if (keys->keyPressed(exit)) {
        _didExit = true;
        _UseKeyboard = true;
    }
    if (keys->keyPressed(dash)) {
        _didDash = true;
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


void InputController::resetKeys(){
    _didFire = false;
    _didReset = false;
    _didPause = false;
    _didChangeMode = false;
    _didSpecial = false;
    _didDebug = false;
    _didExit = false;
    _didDash = false;
    _UseJoystick = false;
    _UseKeyboard = false;
    _didPressLeft = false;
    _didPressRight = false;
    _forward  = 0;
    _turning = 0;
    _Vel = cugl::Vec2(0, 0);
}

void InputController::readInput_joystick() {
    cugl::GameController::Axis X_left = cugl::GameController::Axis::LEFT_X;
    cugl::GameController::Axis Y_left = cugl::GameController::Axis::LEFT_Y;
    cugl::GameController::Button A = cugl::GameController::Button::A;
    cugl::GameController::Button X = cugl::GameController::Button::X;
    cugl::GameController::Button B = cugl::GameController::Button::B;
    cugl::GameController::Button Y = cugl::GameController::Button::Y;
    cugl::GameController::Button LT = cugl::GameController::Button::LEFT_SHOULDER;
    cugl::GameController::Button RT = cugl::GameController::Button::RIGHT_SHOULDER;
    cugl::GameController::Button Back = cugl::GameController::Button::BACK;
    cugl::GameController::Button Start = cugl::GameController::Button::START;
    /* Movement using controller*/
    if (_gameContrl) {
        float LR = _gameContrl->getAxisPosition(X_left);
        float UD = _gameContrl->getAxisPosition(Y_left);

        if (_gameContrl->isButtonPressed(X)) {
            _didFire = true;
            _UseJoystick = true;
        }

        if (_gameContrl->isButtonPressed(A)) {
            _didSpecial = true;
            _UseJoystick = true;
        }

        if (_gameContrl->isButtonPressed(B)) {
            _didChangeMode = true;
            _UseJoystick = true;
        }

        if (_gameContrl->isButtonPressed(Y)) {
            _didReset = true;
            _UseJoystick = true;
        }
        if (_gameContrl->isButtonPressed(LT)) {
            _didDash = true;
            _UseJoystick = true;
        }
        if (_gameContrl->isButtonPressed(RT)) {
            _didChangeMode = true;
            _UseJoystick = true;
        }
          if (_gameContrl->isButtonPressed(Back)) {
            _didReset = true;
            _UseJoystick = true;
        }
          if (_gameContrl->isButtonPressed(Start)) {
            _pause = !_pause;
            _didPause = true;
            _UseJoystick = true;
        }

        if (abs(LR) >= 0.2 || abs(UD) >= 0.2) {

            _Vel = cugl::Vec2(LR, -UD);
            _UseJoystick = true;

           /* if (LR > 0) {
                _turning = 1;
            }
            else if (LR < 0) {
                _turning = -1;
            }*/
        }
    }
}

void InputController::getAxisAngle(const cugl::GameControllerAxisEvent& event, bool focus) {
    if (event.axis == cugl::GameController::Axis::LEFT_Y || event.axis == cugl::GameController::Axis::RIGHT_Y) {
        float UD = event.value;
        if (UD < -0.2) {
            _updown = 1; //Up
        }
        else if (UD > 0.2) {
            _updown = -1; //down
        }
    }
    else if (event.axis == cugl::GameController::Axis::LEFT_X || event.axis == cugl::GameController::Axis::RIGHT_X) {
        float LR = event.value;
        if (LR < -0.2) {
            _Leftright = -1; //Left
        }
        else if (LR > 0.2) {
            _Leftright = 1; //Right
        }
    }
}


void InputController::getButton(const cugl::GameControllerButtonEvent& event, bool focus) {
    if (event.button == cugl::GameController::Button::A) {
        _confirm = true;
        std::cout << "buttonA" << std::endl;
    }else if(event.button == cugl::GameController::Button::B){
        _back = true;
        std::cout << "buttonB" << std::endl;
    }
}


