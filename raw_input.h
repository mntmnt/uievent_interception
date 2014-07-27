#pragma once

#include <vector>

#include <QString>

using DevHandle = void*;
enum RimDevType:unsigned {RimUnknown = 0, RimMouse, RimKeyboard, RimHid};

std::size_t getDevicesNumber();

QString getDeviceName(DevHandle);

bool registerID(void*);

struct devinfo{
       QString name;
       QString typeName;
       DevHandle handle;
       RimDevType rimType;
};

typedef std::vector<devinfo> devinfo_vec;

devinfo_vec getAllDevices();

enum ButtonState:unsigned char {KeyUp, KeyDown, KeyNeutral};
enum MouseButtons:unsigned char {LeftKey, MiddleKey, RightKey, Key1, Key2, Key3, Key4, Key5, UnknownKey};
enum WheelState:signed char {ForwardWheel=-120,NeutralWheel=0,BackwardWheel=120};
typedef ButtonState ButtonStates[UnknownKey];

typedef std::function<void(DevHandle, unsigned short vk, bool pressed)> OnKeyboardProc;
typedef std::function<void(DevHandle, const ButtonStates&, WheelState)> OnMouseProc;
typedef std::function<void(DevHandle, bool arrival)> OnDevPlug;


bool processRawInput(void * message, long * result,
  OnKeyboardProc, OnMouseProc, OnDevPlug);

