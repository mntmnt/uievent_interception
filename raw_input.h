#pragma once

#include <vector>

#include <windows.h>

#include <QString>

std::size_t getDevicesNumber();

struct RawIDevice {
    virtual ~RawIDevice(){}
};

std::vector<RAWINPUTDEVICELIST> getDevices(std::size_t);
std::vector<RAWINPUTDEVICELIST> getDevices();

QString getDeviceName(HANDLE);
QString devTypeStr(DWORD);
QString getDeviceInfo(HANDLE);

bool registerID(HWND);

