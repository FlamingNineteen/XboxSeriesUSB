#include <malloc.h>
#include <stdbool.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_gamecontroller.h>

#include <wups.h>
#include <wups/config.h>
#include <notifications/notifications.h>

#include <padscore/wpad.h>
#include <padscore/kpad.h>

/**
    Mandatory plugin information.
    If not set correctly, the loader will refuse to use the plugin.
**/
WUPS_PLUGIN_NAME("XboxSeriesUSB");
WUPS_PLUGIN_DESCRIPTION("Allows for connecting Xbox Series controllers with USB.");
WUPS_PLUGIN_VERSION("v0.1");
WUPS_PLUGIN_AUTHOR("Flaming19");
WUPS_PLUGIN_LICENSE("GNU");

WUPS_USE_WUT_DEVOPTAB();             // Use the wut devoptabs
WUPS_USE_STORAGE("xbox_series_usb"); // Unique id for the storage api

// Global scope
static bool pInitA = false;
static bool pInitB = false;
static bool pDeinit = false;

INITIALIZE_PLUGIN() {
    pInitA = true;
}

DEINITIALIZE_PLUGIN() {
    pDeinit = true;
}

void mapController(SDL_GameController* xboxController, WPADStatusProController* proControllerStatus) {
    // Reset buttons
    proControllerStatus->buttons = 0;

    // Buttons
    if (SDL_GameControllerGetButton(xboxController, SDL_CONTROLLER_BUTTON_A)) {
        proControllerStatus->buttons |= WPAD_PRO_BUTTON_A;
    }
    if (SDL_GameControllerGetButton(xboxController, SDL_CONTROLLER_BUTTON_B)) {
        proControllerStatus->buttons |= WPAD_PRO_BUTTON_B;
    }
    if (SDL_GameControllerGetButton(xboxController, SDL_CONTROLLER_BUTTON_X)) {
        proControllerStatus->buttons |= WPAD_PRO_BUTTON_X;
    }
    if (SDL_GameControllerGetButton(xboxController, SDL_CONTROLLER_BUTTON_Y)) {
        proControllerStatus->buttons |= WPAD_PRO_BUTTON_Y;
    }
    if (SDL_GameControllerGetButton(xboxController, SDL_CONTROLLER_BUTTON_DPAD_UP)) {
        proControllerStatus->buttons |= WPAD_PRO_BUTTON_UP;
    }
    if (SDL_GameControllerGetButton(xboxController, SDL_CONTROLLER_BUTTON_DPAD_DOWN)) {
        proControllerStatus->buttons |= WPAD_PRO_BUTTON_DOWN;
    }
    if (SDL_GameControllerGetButton(xboxController, SDL_CONTROLLER_BUTTON_DPAD_LEFT)) {
        proControllerStatus->buttons |= WPAD_PRO_BUTTON_LEFT;
    }
    if (SDL_GameControllerGetButton(xboxController, SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) {
        proControllerStatus->buttons |= WPAD_PRO_BUTTON_RIGHT;
    }
    if (SDL_GameControllerGetButton(xboxController, SDL_CONTROLLER_BUTTON_LEFTSHOULDER)) {
        proControllerStatus->buttons |= WPAD_PRO_TRIGGER_L;
    }
    if (SDL_GameControllerGetButton(xboxController, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)) {
        proControllerStatus->buttons |= WPAD_PRO_TRIGGER_R;
    }
    if (SDL_GameControllerGetButton(xboxController, SDL_CONTROLLER_BUTTON_START)) {
        proControllerStatus->buttons |= WPAD_PRO_BUTTON_PLUS;
    }
    if (SDL_GameControllerGetButton(xboxController, SDL_CONTROLLER_BUTTON_BACK)) {
        proControllerStatus->buttons |= WPAD_PRO_BUTTON_MINUS;
    }
    if (SDL_GameControllerGetButton(xboxController, SDL_CONTROLLER_BUTTON_GUIDE)) {
        proControllerStatus->buttons |= WPAD_PRO_BUTTON_HOME;
    }
    if (SDL_GameControllerGetButton(xboxController, SDL_CONTROLLER_BUTTON_LEFTSTICK)) {
        proControllerStatus->buttons |= WPAD_PRO_BUTTON_STICK_L;
    }
    if (SDL_GameControllerGetButton(xboxController, SDL_CONTROLLER_BUTTON_RIGHTSTICK)) {
        proControllerStatus->buttons |= WPAD_PRO_BUTTON_STICK_R;
    }

    // LT and RT buttons
    if (SDL_GameControllerGetAxis(xboxController, SDL_CONTROLLER_AXIS_TRIGGERLEFT) > 16000) {
        proControllerStatus->buttons |= WPAD_PRO_TRIGGER_ZL;
    }
    if (SDL_GameControllerGetAxis(xboxController, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > 16000) {
        proControllerStatus->buttons |= WPAD_PRO_TRIGGER_ZR;
    }

    // Analog sticks
    proControllerStatus->leftStick.x = SDL_GameControllerGetAxis(xboxController, SDL_CONTROLLER_AXIS_LEFTX);
    proControllerStatus->leftStick.y = SDL_GameControllerGetAxis(xboxController, SDL_CONTROLLER_AXIS_LEFTY);
    proControllerStatus->rightStick.x = SDL_GameControllerGetAxis(xboxController, SDL_CONTROLLER_AXIS_RIGHTX);
    proControllerStatus->rightStick.y = SDL_GameControllerGetAxis(xboxController, SDL_CONTROLLER_AXIS_RIGHTY);
}

void notifyTest() {
    NotificationModuleStatus status = NotificationModule_AddInfoNotificationEx(
        "XboxSeriesUSB: Test Notification",
        2.0f,
        {255, 255, 255, 255},
        {100, 100, 100, 255},
        NULL,
        NULL,
        true
    );
}

void notifyControllerConnected() {
    NotificationModuleStatus status = NotificationModule_AddInfoNotificationEx(
        "XboxSeriesUSB: Controller Connected",
        2.0f,
        {255, 255, 255, 255},
        {100, 100, 100, 255},
        NULL,
        NULL,
        true
    );
}

void notifyControllerDisconnected() {
    NotificationModuleStatus status = NotificationModule_AddInfoNotificationEx(
        "XboxSeriesUSB: Controller Disconnected",
        2.0f,
        {255, 255, 255, 255},
        {100, 100, 100, 255},
        NULL,
        NULL,
        true
    );
}

int main() {
    // Initialize SDL
    while (!pInitA) {/* Until plugin is loaded */}
    while (!pInitB) {
        if (SDL_Init(SDL_INIT_GAMECONTROLLER) == 0) {
            pInitB = true;
    }}

    // Initialize WPAD, SDL, and Notifications
    SDL_GameController* controller;
    SDL_Event event;
    WPADInit();
    WPADStatusProController proControllerStatus;
    NotificationModule_InitLibrary();
    notifyTest();

    // Loop until plugin is deinitialized
    while (!pDeinit) {
        // Poll for events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_CONTROLLERDEVICEADDED) {
                controller = SDL_GameControllerOpen(event.cdevice.which);
                notifyControllerConnected();
            } else if (event.type == SDL_CONTROLLERDEVICEREMOVED) {
                SDL_GameControllerClose(controller);
                notifyControllerDisconnected();
            }
        }

        // Read controller inputs
        if (controller) {
            // Reset Pro Controller status
            proControllerStatus.buttons = 0; // Reset buttons
            proControllerStatus.leftStick.x = 0; // Reset left stick
            proControllerStatus.leftStick.y = 0; // Reset left stick
            proControllerStatus.rightStick.x = 0; // Reset right stick
            proControllerStatus.rightStick.y = 0; // Reset right stick

            // Map Xbox controller inputs to Pro Controller inputs
            mapController(controller, &proControllerStatus);

            // Send the mapped inputs to the Pro Controller
            WPADSetDataFormat(WPAD_CHAN_0, WPAD_FMT_PRO_CONTROLLER);

            // Write Pro Controller inputs
            WPADRead(WPAD_CHAN_0, &proControllerStatus);
        }
    }

    // Cleanup SDL
    if (controller) {
        SDL_GameControllerClose(controller);
    }
    SDL_Quit();
}