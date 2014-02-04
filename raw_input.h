#pragma once

#include <vector>

#include <windows.h>

#include <QString>

std::size_t getDevicesNumber();

std::vector<RAWINPUTDEVICELIST> getDevices(std::size_t);
std::vector<RAWINPUTDEVICELIST> getDevices();

QString getDeviceName(HANDLE);
QString devTypeStr(DWORD);
QString getDeviceInfo(HANDLE);

bool registerID(HWND);

bool processRawInput(void * message, long * result,
  std::function<void(void *, unsigned short vk, bool pressed)> onKb);


// intermediate

struct devinfo{
       QString name;
       QString typeName;
       void * handle;
       unsigned short type;
};

typedef std::vector<devinfo> devinfo_vec;

