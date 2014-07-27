#include "raw_input.h"

#include <exception>
#include <QString>
#include <QDebug>
#include <functional>

#include <windows.h>

std::vector<RAWINPUTDEVICELIST> getDevices(std::size_t);
std::vector<RAWINPUTDEVICELIST> getDevices();

QString getDeviceInfo(DevHandle);
QString devTypeStr(DWORD);
RimDevType getType(DWORD);

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


std::size_t getDevicesNumber() {
    UINT number = 0u;

    if(GetRawInputDeviceList(nullptr, &number, sizeof(RAWINPUTDEVICELIST)) == (UINT)-1) {
       auto le = GetLastError();
       throw std::runtime_error(QString::number(le).toStdString());
    }

    return number;
}

std::vector<RAWINPUTDEVICELIST> getDevices(const std::size_t num) {
    std::vector<RAWINPUTDEVICELIST> devices(num);
    UINT number = num;

    if(GetRawInputDeviceList(&devices[0], &number, sizeof(RAWINPUTDEVICELIST)) == (UINT)-1) {
       auto le = GetLastError();
       throw std::runtime_error(QString::number(le).toStdString());
    }

    return devices;
}

std::vector<RAWINPUTDEVICELIST> getDevices() {
    return getDevices(getDevicesNumber());
}

QString getDeviceName(DevHandle h) {
   char devname[512] = {0};
   UINT count = 512;

   if(GetRawInputDeviceInfoA((HANDLE)h, RIDI_DEVICENAME, devname, &count) == (UINT)-1) {
       auto le = GetLastError();
       throw std::runtime_error(QString::number(le).toStdString());
    }

   return devname;
}

QString getDeviceInfo(DevHandle h) {
    RID_DEVICE_INFO dev_inf;
    UINT size = sizeof(RID_DEVICE_INFO);

    auto res = GetRawInputDeviceInfoA((HANDLE)h, RIDI_DEVICEINFO, &dev_inf, &size);

    if(res == (UINT)-1) {
       throw std::runtime_error(  ("Error GetRawInputDeviceInfoA" + QString::number(GetLastError())).toStdString());
    }

    QString result = "{";

    switch(dev_inf.dwType) {
    case RIM_TYPEMOUSE:
//    For the mouse, the Usage Page is 1 and the Usage is 2
       {RID_DEVICE_INFO_MOUSE m = dev_inf.mouse;
       result += "ID: " + QString::number(m.dwId) + ", ";
       result += "buttons: " + QString::number(m.dwNumberOfButtons) + ", ";
    //SPEC WINVER   //result += "horWh: " + QString::number(m.fHasHorizontalWheel) + ", ";
       result += "rate: " + QString::number(m.dwSampleRate);
       }
       break;
    case RIM_TYPEKEYBOARD:
    //Usage Page is 1 and the Usage is 6. For the keyboard, the
       {RID_DEVICE_INFO_KEYBOARD kb = dev_inf.keyboard;
       result += "dwKeyboardMode: " + QString::number(kb.dwKeyboardMode) + ", ";
       result += "dwNumberOfFunctionKeys: " + QString::number(kb.dwNumberOfFunctionKeys) + ", ";
       result += "dwNumberOfIndicators: " + QString::number(kb.dwNumberOfIndicators) + ", ";
       result += "dwNumberOfKeysTotal: " + QString::number(kb.dwNumberOfKeysTotal) + ", ";
       result += "dwSubType: " + QString::number(kb.dwSubType) + ", ";
       result += "dwType: " + QString::number(kb.dwType) + "";
       }
       break;
    case RIM_TYPEHID:
       {RID_DEVICE_INFO_HID hid = dev_inf.hid;
       result += "dwProductId: " + QString::number(hid.dwProductId) + ", ";
       result += "dwVendorId: " + QString::number(hid.dwVendorId) + ", ";
       result += "dwVersionNumber: " + QString::number(hid.dwVersionNumber) + ", ";
       result += "usUsagePage: " + QString::number(hid.usUsagePage) + ", ";
       result += "usUsage: " + QString::number(hid.usUsage) + "";
       }
       break;
    default:
       throw std::runtime_error((QString("UNKNOWN RIM_TYPE") + QString::number(dev_inf.dwType)).toStdString());
    }
    result += "}";
    return result;
}

devinfo_vec getAllDevices() {
        auto deviced = getDevices();
        devinfo_vec v;
        v.reserve(deviced.size());
        for(RAWINPUTDEVICELIST & dev : deviced) {
           devinfo di;
           di.typeName = devTypeStr(dev.dwType);
           di.name = getDeviceName(dev.hDevice);
           di.handle = dev.hDevice;
           di.rimType = getType(dev.dwType);
           v.push_back(di);
        }
        return v;
}

RimDevType getType(DWORD dwType) {
   return dwType == RIM_TYPEHID ? RimDevType::RimHid : (
                dwType == RIM_TYPEKEYBOARD ? RimDevType::RimKeyboard : (
                   dwType == RIM_TYPEMOUSE ? RimDevType::RimMouse : RimDevType::RimUnknown) );

}

QString devTypeStr(DWORD dwType) {
    QString strs[] = { "UNEXPECTED RIM_TYPE", "RIM_MOUSE(0)", "RIM_TYPEKEYBOARD(1)", "RIM_TYPEHID(2)" };
    return strs[getType(dwType)];
}

bool registerID(void* id) {
   HWND hwnd = reinterpret_cast<HWND>(id);

   RAWINPUTDEVICE rd[2];
   rd[0].usUsage = 6;
   rd[0].usUsagePage = 1;
   rd[0].hwndTarget = hwnd;
   rd[0].dwFlags = RIDEV_INPUTSINK | RIDEV_DEVNOTIFY_redef;

   rd[1].usUsage = 2;
   rd[1].usUsagePage = 1;
   rd[1].hwndTarget = hwnd;
   rd[1].dwFlags = RIDEV_INPUTSINK | RIDEV_DEVNOTIFY_redef;

   if(RegisterRawInputDevices(rd, 2, sizeof(RAWINPUTDEVICE)) == FALSE) {
      auto le = GetLastError();
      throw std::runtime_error((QString("UNKNOWN 5") + QString::number(le)).toStdString());
   }
   return true;
}



void processMouse(RAWINPUT * raw, OnMouseProc proc);
void processKeyboard(RAWINPUT * raw, OnKeyboardProc proc);



bool processRawInput(void * message, long *,
        OnKeyboardProc onKb, OnMouseProc onM, OnDevPlug onPlug) {
     MSG* msg = reinterpret_cast<MSG*>(message);

     if(msg->message == WM_INPUT_DEVICE_CHANGE_redef) {
         QString msg_str = msg->wParam == GIDC_ARRIVAL_redef ? "GIDC_ARRIVAL" :
            (msg->wParam ==  GIDC_REMOVAL_redef ? "GIDC_REMOVAL" : "Uexpected WM_INPUT_DEVICE_CHANGE");
         qDebug() << "PLUG/UNPLUG device" << msg_str;

         onPlug(reinterpret_cast<DevHandle>(msg->lParam), msg->wParam == GIDC_ARRIVAL_redef);

         return false;
     }

     if(msg->message == WM_INPUT) {
        UINT sz = sizeof(RAWINPUT);
        //wParam RIM_INPUT RIM_INPUTSINK

        RAWINPUT rawd;
        // Load data into the buffer
        auto rst = GetRawInputData((HRAWINPUT)msg->lParam, RID_INPUT, &rawd, &sz, sizeof (RAWINPUTHEADER));

        if(rst == (UINT)-1) {
           return true;
        }
        RAWINPUT * raw = & rawd;
        if(raw->header.dwType == RIM_TYPEMOUSE) {
            processMouse(raw, onM);
        } else if (raw->header.dwType == RIM_TYPEKEYBOARD) {
            processKeyboard(raw, onKb);
        } else if (raw->header.dwType == RIM_TYPEHID) {
           qDebug() << "RIM_TYPEHID received. Processing of other controls is not implemented";
        }
        DefRawInputProc(&raw, 1, sizeof(RAWINPUTHEADER));
        return true;
     }
     return false;
}

void processKeyboard(RAWINPUT *raw, OnKeyboardProc onKb) {
    USHORT vk = raw->data.keyboard.VKey;
    bool isKeyDown = raw->data.keyboard.Flags & RI_KEY_MAKE ? true : false;
    bool isKeyUp = raw->data.keyboard.Flags & RI_KEY_BREAK ? true : false;

    if(isKeyDown && isKeyUp) {
       qDebug() << "ERROR KeyDown/KeyUp happened simultaneously";
    }

    onKb(raw->header.hDevice, vk, isKeyDown && !KeyUp);
}

void processMouse(RAWINPUT * raw, OnMouseProc proc) {
    const static DWORD associations[][2] = {
        {RI_MOUSE_LEFT_BUTTON_DOWN, RI_MOUSE_RIGHT_BUTTON_UP},
        {RI_MOUSE_MIDDLE_BUTTON_DOWN, RI_MOUSE_MIDDLE_BUTTON_UP},
        {RI_MOUSE_RIGHT_BUTTON_DOWN, RI_MOUSE_RIGHT_BUTTON_UP},
        {RI_MOUSE_BUTTON_1_DOWN, RI_MOUSE_BUTTON_1_UP},
        {RI_MOUSE_BUTTON_2_DOWN, RI_MOUSE_BUTTON_2_UP},
        {RI_MOUSE_BUTTON_3_DOWN, RI_MOUSE_BUTTON_3_UP},
        {RI_MOUSE_BUTTON_4_DOWN, RI_MOUSE_BUTTON_4_UP},
        {RI_MOUSE_BUTTON_5_DOWN, RI_MOUSE_BUTTON_5_UP},
    };
    const static MouseButtons buttons[] = {LeftKey, MiddleKey, RightKey, Key1, Key2, Key3, Key4, Key5};
    constexpr auto ProcessedButtonsCount = sizeof(associations)/sizeof(*associations);

    static_assert( sizeof(buttons)/sizeof(*buttons) == ProcessedButtonsCount, "expected equal size of arrays");
    static_assert( ProcessedButtonsCount == MouseButtons::UnknownKey, "Expected Mouse Data is not equal to processed");

    RAWMOUSE mouse = raw->data.mouse;
    HANDLE h = raw->header.hDevice;

    ButtonStates bs;

    for(auto i = 0u; i < ProcessedButtonsCount; ++i) {
        auto kd = associations[i][0], ku = associations[i][1];
        bool is_kd = mouse.usButtonFlags & kd;
        bool is_ku = mouse.usButtonFlags & ku;
        bs[buttons[i]] = is_kd ? ButtonState::KeyDown : is_ku ? ButtonState::KeyUp : ButtonState::KeyNeutral;
        if(is_kd && is_ku)
           qDebug() << "Error: KEY_UP/KEY_DOWN both states were occured simultaneously";
    }
    WheelState ws = (mouse.usButtonFlags & RI_MOUSE_WHEEL) ?
                static_cast<WheelState>(mouse.usButtonData) :
                WheelState::NeutralWheel;

    proc(h, bs, ws);
}

