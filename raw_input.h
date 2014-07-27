#pragma once

#include <vector>

#include <windows.h>

#include <QString>

#ifndef WM_INPUT_DEVICE_CHANGE
   #define WM_INPUT_DEVICE_CHANGE_redef 0x00FE
#else
   #define WM_INPUT_DEVICE_CHANGE_redef WM_INPUT_DEVICE_CHANGE
#endif

#ifndef RIDEV_DEVNOTIFY
   #define RIDEV_DEVNOTIFY_redef 0x00002000
#else
   #define RIDEV_DEVNOTIFY_redef RIDEV_DEVNOTIFY
#endif

#ifndef GIDC_ARRIVAL
   #define GIDC_ARRIVAL_redef 1
#else
   #define GIDC_ARRIVAL_fedef GIDC_ARRIVAL
#endif

#ifndef GIDC_REMOVAL
   #define GIDC_REMOVAL_redef 2
#else
   #define GIDC_REMOVAL_redef GIDC_REMOVAL
#endif

using DevHandle = void*;
enum RimDevType:unsigned {RimUnknown = 0, RimMouse, RimKeyboard, RimHid};

std::size_t getDevicesNumber();

std::vector<RAWINPUTDEVICELIST> getDevices(std::size_t);
std::vector<RAWINPUTDEVICELIST> getDevices();

QString getDeviceName(HANDLE);
QString devTypeStr(DWORD);
RimDevType getType(DWORD);
QString getDeviceInfo(HANDLE);

bool registerID(HWND);

struct devinfo{
       QString name;
       QString typeName;
       DevHandle handle;
       DWORD type;
       RimDevType rimType;
};

typedef std::vector<devinfo> devinfo_vec;

enum ButtonState:unsigned char {KeyUp, KeyDown, KeyNeutral};
enum MouseButtons:unsigned char {LeftKey, MiddleKey, RightKey, Key1, Key2, Key3, Key4, Key5, UnknownKey};
enum WheelState:signed char {ForwardWheel=-120,NeutralWheel=0,BackwardWheel=120};
typedef ButtonState ButtonStates[UnknownKey];

typedef std::function<void(DevHandle, unsigned short vk, bool pressed)> OnKeyboardProc;
typedef std::function<void(DevHandle, const ButtonStates&, WheelState)> OnMouseProc;
typedef std::function<void(DevHandle, bool arrival)> OnDevPlug;


bool processRawInput(void * message, long * result,
  OnKeyboardProc, OnMouseProc, OnDevPlug);

