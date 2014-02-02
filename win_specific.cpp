#include <exception>
#include <QString>


#include "raw_input.h"

std::size_t getDevicesNumber() {
    UINT number = 0u;
    auto count = GetRawInputDeviceList(nullptr, &number, sizeof(RAWINPUTDEVICELIST));

    if(count == (UINT)-1) {
       throw std::runtime_error(QString::number(GetLastError()).toStdString());
    }

    return number;
}

std::vector<RAWINPUTDEVICELIST> getDevices(const std::size_t num) {
    std::vector<RAWINPUTDEVICELIST> devices(num);
    UINT number = num;

    auto count = GetRawInputDeviceList(&devices[0], &number, sizeof(RAWINPUTDEVICELIST));

    if(count == (UINT)-1) {
       throw std::runtime_error(QString::number(GetLastError()).toStdString());
    }

    return devices;
}

std::vector<RAWINPUTDEVICELIST> getDevices() {
    return getDevices(getDevicesNumber());
}

QString getDeviceName(HANDLE h) {
   char devname[512] = {0};
   UINT count = 512;

   auto a = GetRawInputDeviceInfoA(h, RIDI_DEVICENAME, devname, &count);

   if(count == (UINT)-1) {
       throw std::runtime_error(QString::number(GetLastError()).toStdString());
    }

   return devname;
}

QString getDeviceInfo(HANDLE h) {
    RID_DEVICE_INFO dev_inf;
    UINT size = sizeof(RID_DEVICE_INFO);

    auto res = GetRawInputDeviceInfoA(h, RIDI_DEVICEINFO, &dev_inf, &size);

    if(res == (UINT)-1) {
       throw std::runtime_error(QString::number(GetLastError()).toStdString());
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

QString devTypeStr(DWORD dwType) {
    return QString(
             dwType == RIM_TYPEHID ? "RIM_TYPEHID(2)" : (
                dwType == RIM_TYPEKEYBOARD ? "RIM_TYPEKEYBOARD(1)" : (
                   dwType == RIM_TYPEMOUSE ? "RIM_MOUSE(0)" : "UNEXPECTED RIM_TYPE"
                )
             )
          );
}

bool registerID(HWND id) {
   HWND hwnd = (HWND)id;

   RAWINPUTDEVICE rd[2];
   rd[0].usUsage = 6;
   rd[0].usUsagePage = 1;
   rd[0].hwndTarget = hwnd;
   rd[0].dwFlags = RIDEV_INPUTSINK;

   rd[1].usUsage = 2;
   rd[1].usUsagePage = 1;
   rd[1].hwndTarget = hwnd;
   rd[1].dwFlags = RIDEV_INPUTSINK;

   auto res = RegisterRawInputDevices(rd, 2, sizeof(RAWINPUTDEVICE));

   if(res == FALSE) {
      throw std::runtime_error((QString("UNKNOWN RegisterRawInputDevices") + QString::number(GetLastError())).toStdString());
   }
   return true;
}
