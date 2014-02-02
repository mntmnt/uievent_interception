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

   if(a == (UINT)-1) {
       throw std::runtime_error(QString::number(GetLastError()).toStdString());
    }

   return devname;
}

QString getDeviceInfo(HANDLE h) {
    RID_DEVICE_INFO dev_inf;
    UINT size = sizeof(RID_DEVICE_INFO);

    auto res = GetRawInputDeviceInfoA(h, RIDI_DEVICEINFO, &dev_inf, &size);

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

#include <QDebug>

void processMouse(RAWMOUSE mouse) {
DWORD associations[][2] = {
{RI_MOUSE_LEFT_BUTTON_DOWN, RI_MOUSE_RIGHT_BUTTON_UP},
{RI_MOUSE_RIGHT_BUTTON_DOWN, RI_MOUSE_RIGHT_BUTTON_UP},
{RI_MOUSE_MIDDLE_BUTTON_DOWN, RI_MOUSE_MIDDLE_BUTTON_UP},
{RI_MOUSE_BUTTON_1_DOWN, RI_MOUSE_BUTTON_1_UP},
{RI_MOUSE_BUTTON_2_DOWN, RI_MOUSE_BUTTON_2_UP},
{RI_MOUSE_BUTTON_3_DOWN, RI_MOUSE_BUTTON_3_UP},
{RI_MOUSE_BUTTON_4_DOWN, RI_MOUSE_BUTTON_4_UP},
{RI_MOUSE_BUTTON_5_DOWN, RI_MOUSE_BUTTON_5_UP},
};
std::string names[] = {"LEFT", "RIGHT", "MIDDLE", "X1", "X2", "X3", "X4", "X5"};

for(int i = 0; i < 8; ++i) {
    auto kd = associations[i][0], ku = associations[i][1];
    bool is_kd = mouse.usButtonFlags & kd;
    bool is_ku = mouse.usButtonFlags & ku;
    if(is_kd || is_ku) {
        qDebug() << names[i].c_str() << (is_kd ? " .DOWN. " : " ") << (is_ku ? " .UP. " : " ");
    }
}
if(mouse.usButtonFlags & RI_MOUSE_WHEEL) {
   qDebug() << " MIDDLE_MOUSE " << (signed short)mouse.usButtonData;
}
return;
}

bool processRawInput(void * message, long * result) {
     MSG* msg = reinterpret_cast<MSG*>(message);

     if(msg->message == WM_INPUT) {
        UINT sz = sizeof(RAWINPUT);

        RAWINPUT rawd;
        // Load data into the buffer
        auto rst = GetRawInputData((HRAWINPUT)msg->lParam, RID_INPUT, &rawd, &sz, sizeof (RAWINPUTHEADER));

        if(rst == (UINT)-1) {
           return true;
        }
        RAWINPUT * raw = & rawd;
        if(raw->header.dwType == RIM_TYPEMOUSE) {
            processMouse(raw->data.mouse);

        } else if (raw->header.dwType == RIM_TYPEKEYBOARD) {
             USHORT vk = raw->data.keyboard.VKey;
             bool isKeyDown = raw->data.keyboard.Flags & RI_KEY_MAKE ? true : false;
             bool isKeyUp = raw->data.keyboard.Flags & RI_KEY_BREAK ? true : false;
             qDebug() << "0x" << QString::number(vk, 16) << (isKeyDown ? " .KEYDOWN. " : " .?. ") <<
               (isKeyUp ? " .KEYUP. " : " .?. ");
        } else if (raw->header.dwType == RIM_TYPEHID) {
        }
        return true;
     }
     return false;
}
